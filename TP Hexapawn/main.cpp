#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

unordered_map<string, int> configsStorage;

//structure de configuration qui comprend qui est le joueur à qui est le tour et un plateau avec une position des pions
struct Configuration {

	char joueurAQuiEstLeTour;
	unsigned int n, m;
	vector<vector<char> > plateau;

	Configuration(vector<vector<char>> p, unsigned int l, unsigned int c, char pion) {
		joueurAQuiEstLeTour = pion; //P = blanc et p = noir
		plateau = p;
		n = l;
		m = c;
	}
};

//convert the configuration into a string that will be stored as the key in the unordered_map
string convertConfigToString(Configuration c)
{
    string key = "";
    key+=c.joueurAQuiEstLeTour;
	for(unsigned int i = 0; i < c.n; i++) {
		for(unsigned int j = 0; j < c.m; j++) {
			key+=c.plateau[i][j];
		}
	}
	return key;
}

//fonction pour voir si le joueur courant a perdu face à son adversaire
bool aPerduContreLadversaire(Configuration c) {
	if (c.joueurAQuiEstLeTour == 'P') {
		for (unsigned int i = 0; i < c.m; i++) {
			if (c.plateau[c.n - 1][i] == 'p') {
				return true;
			}
		}
	}
	else {
		for (unsigned int i = 0; i < c.m; i++) {
			if (c.plateau[0][i] == 'P') {
				return true;
			}
		}
	}
	return false;
}

//fonction qui calcule le meilleur successeur d'une configuration
int meilleurSuccesseur(vector<int> successeurs) {
	bool negative = false;
	int meilleurSuccesseur = successeurs[0];
	for (int i = 0; i < successeurs.size(); i++) {
		if (!negative) {
			if (successeurs[i] <= 0) { 
				meilleurSuccesseur = successeurs[i];
				negative = true;
			} else if (successeurs[i] > meilleurSuccesseur) { 
				meilleurSuccesseur = successeurs[i];
			}
		} else { 
			if (successeurs[i] <= 0) { 
				if (successeurs[i] > meilleurSuccesseur)
					successeurs[i] = meilleurSuccesseur;
			}
		}
	}
	if (negative) {
		return (meilleurSuccesseur - 1) * -1;
	} else {
		return (meilleurSuccesseur + 1) * -1;
	}
}

//fonction recursive qui créer les nouvelles configurations possibles et retourne la meilleure evaluation calculée par la fonction optimal
int calculConfiguration(Configuration c) {

    string configKey =  convertConfigToString(c);
	unordered_map<string,int>::const_iterator it = configsStorage.find(configKey);
	if (it != configsStorage.end()) { //si la config est dans configsStorage on return la valeur de l'eval correspondante
		return it->second;
	}

	int meilleureEval, e1, e2, e3;
	vector<int> successeurs;

	// cas de base : si le joueur adverse se trouve dans sa base
	if (aPerduContreLadversaire(c)) {
		return 0;
	}


	for (unsigned int i = 0; i < c.n; i++) {
		for (unsigned int j = 0; j < c.m; j++) {
			if (c.joueurAQuiEstLeTour == 'P') { //si c'est aux blancs de jouer
				if (c.plateau[i][j] == 'P') {
					if (i > 0 && c.plateau[i - 1][j] == ' ') { //config où le pion blanc avance tout droit
						vector<vector<char> > p1 = c.plateau;
						p1[i][j] = ' ';
						p1[i - 1][j] = 'P';
						Configuration c1(p1, c.n, c.m, 'p');
						e1 = calculConfiguration(c1);
						if (e1 == 0) {
							return 1;
						}
						successeurs.push_back(e1);
					}
					if (i > 0 && j < c.m - 1 && c.plateau[i - 1][j + 1] == 'p') { //config où le pion blanc prend un pion noir à droite
						vector<vector<char> > p2 = c.plateau;
						p2[i][j] = ' ';
						p2[i - 1][j + 1] = 'P';
						Configuration c2(p2, c.n, c.m, 'p');
						e2 = calculConfiguration(c2);
						if (e2 == 0) {
							return 1;
						}
						successeurs.push_back(e2);
					}
					if (i > 0 && j > 0 && c.plateau[i - 1][j - 1] == 'p') { //config où le pion blanc prend un pion noir à gauche
						vector<vector<char> > p3 = c.plateau;
						p3[i][j] = ' ';
						p3[i - 1][j - 1] = 'P';
						Configuration c3(p3, c.n, c.m, 'p');
						e3 = calculConfiguration(c3);
						if (e3 == 0) {
							return 1;
						}
						successeurs.push_back(e3);
					}
				}
			} else { //si c'est aux noirs de jouer
				if (c.plateau[i][j] == 'p') {
					if (i < c.n - 1 && c.plateau[i + 1][j] == ' ') { //config où le pion noir avance tout droit
						vector<vector<char> > p1 = c.plateau;
						p1[i][j] = ' ';
						p1[i + 1][j] = 'p';
						Configuration c1(p1, c.n, c.m, 'P');
						e1 = calculConfiguration(c1);
						if (e1 == 0) {
							return 1;
						}
						successeurs.push_back(e1);
					}
					if (i < c.n - 1 && j < c.m - 1 && c.plateau[i + 1][j + 1] == 'P') { //config où le pion blanc prend un pion noir à droite
						vector<vector<char> > p2 = c.plateau;
						p2[i][j] = ' ';
						p2[i + 1][j + 1] = 'p';
						Configuration c2(p2, c.n, c.m, 'P');
						e2 = calculConfiguration(c2);
						if (e2 == 0) {
							return 1;
						}
						successeurs.push_back(e2);
					}
					if (i < c.n - 1 && j > 0 && c.plateau[i + 1][j - 1] == 'P') { //config où le pion blanc prend un pion noir à gauche
						vector<vector<char> > p3 = c.plateau;
						p3[i][j] = ' ';
						p3[i + 1][j - 1] = 'p';
						Configuration c3(p3, c.n, c.m, 'P');
						e3 = calculConfiguration(c3);
						if (e3 == 0) {
							return 1;
						}
						successeurs.push_back(e3);
					}
				}
			}
		}
	}

	if (successeurs.size() == 0) { // si le joueur est bloqué 
		configsStorage.insert(make_pair(configKey, 0));
		return 0;
	}

	meilleureEval = meilleurSuccesseur(successeurs);
	configsStorage.insert(make_pair(configKey, meilleureEval));

	return meilleureEval;
}

int main() {

	  unsigned int n, m;
	  cin >> n;
	  cin >> m;
      cin.ignore();
	  vector<vector<char> > p;

	  string str="";
	  for(unsigned int i=0; i<n; i++) {
	  		vector<char> line;
	 		getline(cin, str);
	  		for(unsigned int k=0; k<str.length(); k++) {
	  		line.push_back(str.at(k));
	  		}

	  	str="";
	 	p.push_back(line);
	  }

	Configuration config(p, n, m, 'P');

	int eval = calculConfiguration(config);

	cout << eval << endl;

	return 0;

}
