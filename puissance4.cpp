#include <iostream>
#include <cstdlib>
#include <array>
using namespace std;

array<array<int, 7>, 6> grille;
void Affichage();
void Poser(int colonne, int joueur);

int main(){
	int numeroProgramme;
	int numeroJoueur;
	int jeux = 1;
	int tour = 1;
	int coupJoue;

	cout << "Entrez le numero du programme (1 ou 2) : ";
	cin >> numeroProgramme;
	cout << endl << "Merci !" << endl;

	if(numeroProgramme == 1)
		numeroJoueur = 2;
	else
		numeroJoueur = 1;

	while(jeux){
		if(tour == numeroJoueur){
			cout << "Entrez le numéro de la colonne (de 0 à 6), dans laquelle vous voulez jouer" << endl;
			cin >> coupJoue;
			Poser(coupJoue, tour);
		} else {
			coupJoue = rand()%7;
			Poser(coupJoue, tour);
		}
		Affichage();
		if(tour == 1){
			tour = 2;
		} else {
			tour = 1;
		}
	}
	return 0;
}

void Affichage(){
	char barreVerticale = '|';
	char barreHorizontale = '-';
	char coinHautGauche = '.';
	char coinHautDroit = '.';
	char coinBasGauche = '.';
	char coinBasDroit = '.';
	int i,j;

	cout << coinHautGauche;
	for(i = 0; i < 15; ++i){
		cout << barreHorizontale;
	}
	cout << coinHautDroit << endl;

	for(i = 0; i < 6; ++i){
		cout << barreVerticale;
		cout << ' ';
		for(j = 0; j < 7; ++j){
			cout << grille[i][j] << ' ';
		}
		cout << barreVerticale;
		cout << endl;
	}

	cout << coinBasGauche;
	for(i = 0; i < 15; ++i){
		cout << barreHorizontale;
	}
	cout << coinHautDroit << ' ' << endl;
}

void Poser(int colonne, int joueur){
	int correct = 0;
	if(colonne < 0 || colonne > 6){
		cout << "Coup invalide" << endl;
	}
	for(int i = 5; i >= 0; i--){
		if(!grille[i][colonne]){
			grille[i][colonne] = joueur;
			correct = 1;
			break;
		}
	}

	if(!correct){
		cout << "colonne pleine, fin du programme, t'as perdu" << endl;
		exit(0);
	}
}
