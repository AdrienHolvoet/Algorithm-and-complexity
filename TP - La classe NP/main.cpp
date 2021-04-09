#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <cstring>

using namespace std;

//CLI:
//command to compile this file: g++ -std=c++11 main.cpp -o NomExe
//to run exe: ./NomExe nomFichierDeDonneesTest -option (int)

//exemple chemin de fichier : donneesTestNP/donnee3
//-options possibles: -verif -nondet -exhaust
//int: donnée D du probléme JSP (pas obligatoire: si rien D=0)
//pour tester avec des données de partition mettre -partition à la place de la valeur de D à la fin


//Task
struct Task {
	int a;// arrival date in seconds
    int t;// duration

	Task(int arrivalTime, int duration) {
		a = arrivalTime;
        t = duration;
	}
};


//represent the data
struct JSP {
	int m;// number of machine available
    int n;// number of tasks
    vector<Task> tasks;
    int D;

	JSP(int numberOfDevice, int numberOfTasks, vector<Task> allTasks, int delay) {
		m = numberOfDevice;
        n = numberOfTasks;
        tasks = allTasks;
        D = delay;
	}

    void printJSP(){
        cout << "nombre de machines = " << m << endl;
        cout << "nombre de tâches = " << n << endl;
        cout << "délai d'attente max = " << D << endl;
        cout << "Tâches:" << endl;
        for(int i=0; i<tasks.size(); i++) {
            cout << "tâches " << i << " arrive a " << tasks[i].a << " et dure " << tasks[i].t << endl;
        }
    }
};

void print(const std::vector<int>& v)
{
    for (int e : v) {
        std::cout << " " << e;
    }
    std::cout << std::endl;
}

bool verificationAlgorithm(JSP instance, vector <int> certificat) {
    vector<Task> m;
    for(int i = 0; i<instance.m ; i++) {
        m.push_back(instance.tasks[certificat[i]]);
    }

    for(int i = instance.m ; i< instance.n;i++) { // (n - m) loop
        int busyMachine = 0;
        bool alreadyChange = false;
       for(int j = 0 ; j < m.size() ; j++) { // m(=number of machine) loop
           int duration = m[j].a + m[j].t;
            if(duration <= instance.tasks[certificat[i]].a + instance.D ) {
                if(alreadyChange==false) {
                    if(instance.tasks[certificat[i]].a < duration) {
                    instance.tasks[certificat[i]].a = duration;
                    }
                m[j] = instance.tasks[certificat[i]];
                alreadyChange = true;
                }
            }else {
                busyMachine++;
            }
       }

       if(busyMachine==instance.m) { // if the number of busy machine is == at m means that the certificat is false
           return false;
       }
       alreadyChange = false;
    }
    return true;
}

//Q 3.2. Enumération de tous les certificats ou l’algorithme du British Museum
vector<int> generateAllCertificats(JSP instance) {
    vector<int> v;
    bool isValid = false;

    for(int i = 0; i < instance.n; i++) {
    v.push_back(i);
    }
    do {
        isValid = verificationAlgorithm(instance,v);// to replace by the verif algo
        if(isValid) {
            return v;
        }
    } while (next_permutation(v.begin(), v.end()) );

    v.clear();
    return v;
}

// Q 2.1.Génération aléatoire d’un certificat
vector<int> generationAleatoireDeCertificat(JSP instance) {
    vector<int> certificat;

    while(certificat.size() < instance.n) {
        int nb = rand() % instance.n; //on choisit un nombre aleatoire qui est compris entre 0 et n
        vector<int>::iterator it = find(certificat.begin(), certificat.end(), nb);
        if(it == certificat.end()) { // on vérifie que ce numéro n'est pas déjà dans le certificat
            //(on ne veut pas avoir deux fois la même tâche)
            certificat.push_back(nb); //on place le numéro dans le certificat
        }
    }
    return certificat;
}

int main(int argc, char ** argv) {

    srand (time(NULL));

    //tableau qui va contenir une instance donnee par le fichier en argument
    std::vector<char> dataJSP;
    std::vector<string> dataPartition;
    //lit l’instance du problème dans un fichier
    std::ifstream file(argv[1]);
    if(file) {
        if(!strcmp(argv[3], "-partition")){
            string str; 
            while (std::getline(file, str)) {
              dataPartition.push_back(str);
            }
        } else {
            char ch;
            while (file >> ch) {
            	dataJSP.push_back(ch);
            }
        }
    } else {
        cout << "ERREUR: Impossible d'ouvrir le fichier en lecture." << endl;
        exit(1);
    }

    int n, m, D = 0;
    vector<Task> tasksList;

    if(!strcmp(argv[3], "-partition")) { //si on test des données de Partition avec les algo de JSP
    //on prend m=2 et on construit toutes les tâches avec ai=0 et on prend D=somme totale des xi/2
        m = 2;
        n = stoi(dataPartition[0]);
        for(unsigned int i=1; i<=n; i++) {
            int t = stoi(dataPartition[i]);
            Task task(0, t);
            tasksList.push_back(task);
            D+=t;
        }
        D=D/2;
        //ajout des deux tâches en plus avec ai=0 et ti=D:
        n+=2;
        Task task(0, D);
        tasksList.push_back(task);
        tasksList.push_back(task);

    } else { //si on test des données de JSP
        m = dataJSP[0] - '0';
        n = dataJSP[1] - '0';
        for(unsigned int i=2; i<=n*2; i++) {
            int a = dataJSP[i] - '0';
            int t = dataJSP[i+1] - '0';
        	Task task(a, t);
        	tasksList.push_back(task);
            i++;
        }

        if(argv[3] != nullptr) {
        	char* c = argv[3];
        	D = *c - '0';
        }
    }

    JSP instance(m, n, tasksList, D);

    if (!strcmp(argv[2], "-verif"))
    {
        // propose à l’utilisateur de saisir un certificat et vérifie sa validité
        vector<int> certificat;

        cout << "entrez un certificat: pour les données suivantes:" << endl;

        instance.printJSP();
        cout << endl;

        cout << "indiquez le numéro des tâches dans l'ordre dans lequel les taches doivent s'executer" << endl;

        for(unsigned int i=0; i<tasksList.size(); i++) {
        	cout << "entrez le numéro d'une tâche" << endl;
        	int ind;
        	cin >> ind;
        	certificat.push_back(ind);
        }
        cout << endl;
        if(verificationAlgorithm(instance, certificat)) {
            cout << "OUI le certificat est valide" << endl;
        } else {
            cout << "NON le certificat n'est pas valide" << endl;
        }
    }
    else if (!strcmp(argv[2], "-nondet"))
    {
        //génère aléatoirement un certificat, le teste et retourne Faux si il n’est pas
        //valide, “Vrai” sinon avec la valeur du certificat
        cout << "Les données:" << endl;
        instance.printJSP();

        vector<int> certificat = generationAleatoireDeCertificat(instance);
        cout << endl;
        cout << "certificat généré aléatoirement:" << endl;
        //affiche le certificat généré aléatoirement
        print(certificat);

        cout << endl;

        if(verificationAlgorithm(instance, certificat)) {
        	cout << "VRAI le certificat est valide" << endl;
        } else {
        	cout << "FAUX le certificat n'est pas valide" << endl;
        }
    }
    else if (!strcmp(argv[2], "-exhaust"))
    {
        //génère tous les certificats jusqu’à en trouver un valide s'il en existe un
        //et retourne Faux si il n’en existe pas de valide (la propriété n’est donc pas vérifiée)
        // “Vrai” sinon avec la valeur du certificat trouvé
        vector<int> v = generateAllCertificats(instance);
        if(v.size()==0) {
           	cout << "Faux il n'existe pas de certificat valide" << endl;
        }
        else {
            cout << "Vrai ce certificat est valide : ";
            print(v);
        }
    }
    else
    {
        std::cout << "ERROR: Unknown option: " << argv[2] << std::endl;
        exit (1);
    }
}


//Partition
struct Partition {
	int n;
    vector <int> L; 

	Partition(int number, vector <int> list ) {
		n = number;
        L = list;
	}
};

// Reduction Partition into JSP
JSP partitionIntoJsp(Partition partition) {
    int m = 2;
    int n = partition.n + 2;
    vector <Task> tasksList;
    int D = 0;
    for(unsigned int i=1; i<=n; i++) {
        int t = partition.L[i];
        Task task(0, t);
        tasksList.push_back(task);
        D+=t;
    }

    D=D/2;
    Task task(0, D);
    //ajout des deux tâches en plus avec ai=0 ti=D:
    tasksList.push_back(task);
    tasksList.push_back(task);
    
    JSP instance(m,n,tasksList,D);

    return instance;
}

//Reduction SUM to Partition, Params corresponds to a instance of SUM
void sumIntoPartition(int n, vector <int> L, int s) {
    int sum = 0;
    for(int i = 0; i < L.size(); i++) {
        sum += L[i];
    }

    vector <int> newL = L;
    newL.push_back(sum + s);
    newL.push_back(2*sum - s);

    Partition partition(n+2, newL);

    partitionIntoJsp(partition);
}

