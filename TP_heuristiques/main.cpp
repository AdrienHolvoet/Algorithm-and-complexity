#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <cstdlib>

//CLI:
//command to compile this file: g++ -std=c++11 main.cpp -o NomExe
//to run exe: ./NomExe nomFichierDeDonneesTest valeurOptimale facteurDApproximation

//Les arguments valeurOptimale et facteurDApproximation sont des paramètres optionels.

using namespace std;

struct Job {
	int p;
	int w;
	int d;
	int ratioDelayTimeWeight;

	Job(int pi, int wi, int di) {
		p = pi;
		w = wi;
		d = di;
	}

	void printJob() {
	    cout << p << " " << w << " " << d << " "<< endl;
	}
};

struct Instance { //structure representant une instance du problème d'ordonnancement
	vector<Job> jobsList;

	Instance(vector<Job> j) {
		jobsList = j;
	}
};

struct Ordonancement { //certificat
	vector<Job> jobsSequence; //jobs dans leur ordre d'execution
	vector<int> completion; //temps de completion de chaque tâche

	Ordonancement(vector<Job> j) {
		jobsSequence = j;
	}
	Ordonancement() {}

	void printOrdo() {
		for(int i=0; i<jobsSequence.size(); i++) {
			cout << "job number " << i << ": p = " << jobsSequence[i].p << " w = " << jobsSequence[i].w << " d = " << jobsSequence[i].d << endl;
		}
	}
};

struct ExperimationResult { //Structure pour la campagne d'expérimentation
	int qualite;
	int nombreEval;
	Ordonancement ordonancement;
	chrono::duration<double> elapsed_seconds;

	ExperimationResult(int q, int evalNumber, Ordonancement ordo, chrono::duration<double> elapsed_time) {
		ordonancement = ordo;
		qualite = q;
		nombreEval = evalNumber;
		elapsed_seconds = elapsed_time;
	}
};


//Q1.3 heuristique constructive 1, tri selon la limite à  laquelle l'exécution de la tâche doit  être terminée
struct tri_heuristique_constructive_1
{
    inline bool operator() (const Job& job1, const Job& job2)
    {
        return (job1.d < job2.d);
    }
};

//Q1.3 heuristique constructive 2 tri en fonction du ratio d/p
struct tri_heuristique_constructive_2
{
    inline bool operator() (const Job& job1, const Job& job2)
 	{
        return (job1.ratioDelayTimeWeight < job2.ratioDelayTimeWeight);
    }
};

//Q1.3 heuristique constructive 3
int modifiedDeadline(int completion, Job job) {
    return max(completion + job.p, job.d);
}

vector<Job> sortedJobsBasedOnModifiedDeadline(Instance instance) {
	vector<Job> unsortedJobs = instance.jobsList;
	vector<Job> sortedJobs;
	int completion = 0;
	while (unsortedJobs.size() != 0) {
		Job bestJob = unsortedJobs[0];
		int bestModifiedDeadline = modifiedDeadline(completion,bestJob);
		int indexBestJob = 0;
		for(int i = 0; i< unsortedJobs.size(); i++) {
			int currentModifiedDeadline = modifiedDeadline(completion,unsortedJobs[i]);
			if(currentModifiedDeadline < bestModifiedDeadline) {
				bestModifiedDeadline = currentModifiedDeadline;
				bestJob = unsortedJobs[i];
				indexBestJob = i;
			}
		}
		sortedJobs.push_back(bestJob);
		unsortedJobs.erase(unsortedJobs.begin()+ indexBestJob);
		completion += bestJob.p;
	}
	return sortedJobs;
}


//Q1.1 ecrire un programme qui permet d’evaluer la qualite d’une solution donnée en argument
int evaluationQualite(Ordonancement o) {
	int qualite = 0;

	//calcul de completion (C dans le TP)
	int c = 0;
	for(int i=0; i<o.jobsSequence.size(); i++) {
		c+=o.jobsSequence[i].p;
		o.completion.push_back(c);
	}

	for(int i=0; i<o.jobsSequence.size(); i++) {
		int T = max((o.completion[i]-o.jobsSequence[i].d), 0);
		qualite+=(T*o.jobsSequence[i].w);
	}
	return qualite;
}



//hill climing méthode inversion avec best improvment
Ordonancement localSearchHillClimbing(Instance instance) {
	//Solution initial
	Ordonancement o(instance.jobsList);
	//évaluation de la qualité de l'état initial:
	int eval = evaluationQualite(o);
	int evalInit;

	//création de tous les voisins:
	do {
		Ordonancement init = o;
		evalInit = eval;
		for(int i=0; i<o.jobsSequence.size()-1; i++) {
			Ordonancement o_voisin = init;
			Job job = o_voisin.jobsSequence[i];
			o_voisin.jobsSequence[i] = o_voisin.jobsSequence[i+1];
			o_voisin.jobsSequence[i+1] = job;
			int tempoEval = evaluationQualite(o_voisin);
			if(tempoEval < eval ) {
				eval = tempoEval;
				o = o_voisin;
			}
		}
	}
	while(evalInit != eval);
	return o;
}

Ordonancement disruptsScheduling(Ordonancement ordonancement) {

	int randomdisturbances =  (rand() % 10) + 3 ;

	Ordonancement disruptedScheduling = ordonancement;

	for(int i = 0 ; i < randomdisturbances; i++) {
		int indexFirstJob = (rand() % 40);
		int indexSecondJob = (rand() % 40);
		Job job = disruptedScheduling.jobsSequence[indexFirstJob];
		disruptedScheduling.jobsSequence[indexFirstJob] = disruptedScheduling.jobsSequence[indexSecondJob];
		disruptedScheduling.jobsSequence[indexSecondJob] = job;
	}

	return disruptedScheduling;
}

//recherche Locale ILS with random pertubation
ExperimationResult localSearchILS(Instance initSolution, int optimal, double facteurApproximation) {
	//Solution initiale
	auto begin = chrono::high_resolution_clock::now(); //start calcul temps CPU
	Ordonancement initScheduling = localSearchHillClimbing(initSolution);
	int initEval = evaluationQualite(initScheduling);
	int nombreEval = 0;
	chrono::time_point<std::chrono::system_clock> m_StartTime = chrono::system_clock::now();
    chrono::time_point<std::chrono::system_clock> m_EndTime;
	
	do
	{
		m_EndTime = chrono::system_clock::now();
		Ordonancement disruptedScheduling = disruptsScheduling(initScheduling);
		nombreEval++;
		Instance newInstance(disruptedScheduling.jobsSequence);
		Ordonancement newOrdonnacement = localSearchHillClimbing(newInstance);
		int tempoEval = evaluationQualite(newOrdonnacement);
		if(tempoEval < initEval ) {
			initEval = tempoEval;
			initScheduling = newOrdonnacement;
		}
	} while(chrono::duration_cast<std::chrono::milliseconds>(m_EndTime - m_StartTime).count()/1000 <3* initScheduling.jobsSequence.size() && initEval > (optimal*facteurApproximation));
	auto end = chrono::high_resolution_clock::now(); //end calcul temps CPU
	chrono::duration<double> elapsed_seconds = end - begin ;
	
	return ExperimationResult(initEval, nombreEval, initScheduling,elapsed_seconds);
}

Ordonancement generationAleatoireDOrdonnancement(Instance instance) {
    vector<Job> sequence;
    int size = instance.jobsList.size();

    while(sequence.size() < size) {
    	// on choisit un indice aléatoire qui est compris entre 0 et le nombre de jobs de l'instance:
        int ind = rand() % instance.jobsList.size();
        // on prend le job à l'indice aléatoire:
        Job j = instance.jobsList[ind];
        // on retire ce job de l'instance pour ne pas le réutiliser dans le certificat:
        instance.jobsList.erase (instance.jobsList.begin()+ind);
		sequence.push_back(j);
    }
    Ordonancement o = Ordonancement(sequence);
    return o;
}

int main(int argc, char ** argv) {

	srand (time(NULL));

	double facteurApproximation = 0;
	int optimal = 0;

	if(argc > 2 ) {
		if(argv[2]) {
			// Optimal du problème
		 	optimal = stoi(argv[2]);
	 	}
	 	if(argv[3]) {
		 	//facteur d’approximation
		 	facteurApproximation = atof(argv[3]) + 1;
			
	 	}
	}

    std::ifstream file(argv[1]);
    if(file) {
        string s;
        vector<Job> jobsList;
        while (std::getline(file, s)) {
       		string delimiter = " ";
        	size_t pos = 0;
            int p = 0;
            int w = 0;
        	string token;
			bool swap = true;
			while ((pos = s.find(delimiter)) != std::string::npos) {
				token = s.substr(0, pos);
    			if(swap) {
        			p =  stoi(token);;
   				}
    			else {
       				w =  stoi(token);;
    			}

    			s.erase(0, pos + delimiter.length());
   				swap = false;
			}
			swap = true;

	   		Job job(p,w,stoi(s));
	  		jobsList.push_back(job);
		}

		// Instance from files
        Instance instance(jobsList);


		// Q1.2 Ecrire un programme qui permet de générer une solution aléatoire et d'évaluer sa qualité.
		cout << "Test qualité avec un ordonancement aléatoire : " << endl;
        Ordonancement ordonancement_test2 = generationAleatoireDOrdonnancement(instance);
        cout << "Qualité = " << evaluationQualite(ordonancement_test2) << endl;
        cout << endl;


		// Q1.3 Proposer et implémenter une (ou plusieurs) heuristiques constructives.
		cout << "TEST HEURISTIQUE CONSTRUCTIVE 1 : sort in ascending order according to d : " << endl;
		std::vector<Job> listOfJobs = instance.jobsList;
		sort(listOfJobs.begin(), listOfJobs.end(), tri_heuristique_constructive_1());
		Ordonancement ordonancement_test3(listOfJobs);
        cout << "Qualité = " << evaluationQualite(ordonancement_test3) << endl;
        cout << endl;


		cout << "TEST: HEURISTIQUE CONSTRUCTIVE 2 : sort in ascending order according to d/w: " << endl;
		std::vector<Job> listOfJobs1 = instance.jobsList;
		//calculation of the ratio d/w
		for (int i = 0; i < listOfJobs1.size();i++) {
			listOfJobs1[i].ratioDelayTimeWeight = listOfJobs1[i].d/listOfJobs1[i].w;
		}
		//sort in ascending order according to d/w
		sort(listOfJobs1.begin(), listOfJobs1.end(), tri_heuristique_constructive_2());
		Ordonancement ordonancement_test4(listOfJobs1);
        cout << "Qualité = " << evaluationQualite(ordonancement_test4) << endl;
        cout << endl;


		cout << "TEST: HEURISTIQUE CONSTRUCTIVE 3, the best one: " << endl;
		vector <Job> sortedJobs = sortedJobsBasedOnModifiedDeadline(instance);
		Ordonancement ordonancement_test5(sortedJobs);
        cout << "Qualité = " << evaluationQualite(ordonancement_test5) << endl;
        cout << endl;


		// Q1.4 Proposer et implémenter un voisinage pour la conception d’heuristiques par recherche locale.
		// test un voisinage hill climbing avec pour voisin le premier job qui a un weight supérieur a celui du job courant
		cout << "TEST: Hill climbing" << endl;
		//Solution initiale correspond to heuristique constructive 3
		Instance initSolution = sortedJobsBasedOnModifiedDeadline(instance);
		Ordonancement ordonancement_test6 = localSearchHillClimbing(initSolution);
        cout << "Qualité = " << evaluationQualite(ordonancement_test6) << endl;
        cout << endl;


        // Q1.5 Proposer et implémenter une recherche locale itérée, de type ILS. 
		cout << "TEST ILS avec Facteur d'approximation : " << facteurApproximation << endl;
		//solution initiale correspond to heuristique constructive 3
		std::vector<Job> init_solution = sortedJobsBasedOnModifiedDeadline(instance);

		ExperimationResult result = localSearchILS(init_solution, optimal, facteurApproximation); 
		
		cout << "Valeur optimal = " << argv[2] << endl;
        cout << "Qualité = " << result.qualite << endl;
		cout << "Nombre d'évaluation(s) = " << result.nombreEval << endl;
    	cout << "elapsed time: " << result.elapsed_seconds.count() << "s" << endl;
        cout << endl;

    } else {
        cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
        exit(1);
    }
}
