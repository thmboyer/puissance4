#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
using namespace std;

//Récompenses en puissance de 2 pour par exemple que win ne puisse pas se faire rattraper par la somme des autres
int win = 100000;
int stopWin = 20000;
int dontGiveWin = 5000;
int forceWin = 1000;
int coupDuTraitre = 1000;
int stopForceWin = 500;
int stopCoupDuTraitre = 500;
int dontGiveForceWin = 300;
int connect3WithPotential = 80;
int coeffCentre = 7;
int playCenter = 4;
int stopConnect3WithPotential = 40;
int connect2WithPotential = 8;

int possibilitesWin[6][7] =	{
								{3,4,5,7,5,4,3},
								{4,6,10,15,10,6,4},
								{5,8,20,25,20,8,5},
								{5,8,20,25,20,8,5},
								{4,6,8,10,8,6,4},
								{2,2,5,7,5,2,2}
							};

void Afficher(vector<vector<int>> & board);
int check_N_horizontal(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N);
int check_N_vertical(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N);
int check_N_diagonale_pente_positive(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N);
int check_N_diagonale_pente_negative(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N);
int potentiel_N_horizontal(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N);
int potentiel_N_vertical(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N);
int potentiel_N_diagonale_pente_positive(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N);
int potentiel_N_diagonale_pente_negative(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N);
int saleTraitre(vector<vector<int>> & board, int x, int y, int S, int W, int H);

int fallHeight(vector<vector<int>> & board, int y, int H){
	int x = H;
	while(x > 0 && board[x-1][y] == 0){
		x -= 1;
	}
	return x;
}

int reward(vector<vector<int>> & board, int y, int S, int W, int H){
	//calcul de x 
	int x = fallHeight(board, y, H);
	//cas coup illégal
	if(x == H){
		return -10000;
	}
	//set up des variables
	int milieu = W/2;
	int reward = 0;
	int adversaire = S % 2 + 1;
	int nombre;
	
	// cout << " x : " << x << " y : " << y << endl;	

	//Win la partie
	if(check_N_horizontal(board, x, y, S, W, H, 4) || check_N_vertical(board, x, y, S, W, H, 4) || check_N_diagonale_pente_positive(board, x, y, S, W, H, 4) || check_N_diagonale_pente_negative(board, x, y, S, W, H, 4)){
		// cout << "win" << endl;
		reward += win;
	}
	
	//Empêcher une win
	if(check_N_horizontal(board, x, y, adversaire, W, H, 4) || check_N_vertical(board, x, y, adversaire, W, H, 4) || check_N_diagonale_pente_positive(board, x, y, adversaire, W, H, 4) || check_N_diagonale_pente_positive(board, x, y, adversaire, W, H, 4) || check_N_diagonale_pente_negative(board, x, y, adversaire, W, H, 4)){
		// cout << "stopWin" << endl;
		reward += stopWin;
	}

	//Ne pas donner la victoire
	nombre = 1;
	board[x][y] = S;
	if(x < H-1){
		if(check_N_horizontal(board, x + 1, y, adversaire, W, H, 4) || check_N_diagonale_pente_positive(board, x + 1, y, adversaire, W, H, 4) || check_N_diagonale_pente_negative(board, x + 1, y, adversaire, W, H, 4)){
			nombre = 0;
		}
	}
	reward += nombre * dontGiveWin;
	board[x][y] = 0;

	//Repérer une force win
	if(x < H - 2){
		nombre = 0;
		board[x][y] = S;
		if( check_N_horizontal(board, x+1, y, S, W, H, 4) || check_N_vertical(board, x+1, y, S, W, H, 4) || check_N_diagonale_pente_positive(board, x+1, y, S, W, H, 4) || check_N_diagonale_pente_negative(board, x+1, y, S, W, H, 4)){
			board[x+1][y] = S;
			if(check_N_horizontal(board, x+2, y, S, W, H, 4) || check_N_vertical(board, x+2, y, S, W, H, 4) || check_N_diagonale_pente_positive(board, x+2, y, S, W, H, 4) || check_N_diagonale_pente_negative(board, x+2, y, S, W, H, 4)){
				nombre = 1;
			}
			board[x+1][y] = 0;
		}
		board[x][y] = 0;
		reward += nombre * forceWin;
	}
	//Empêcher une force win
	
	//Ne pas donner une force win
	if(x < H - 2){
		nombre = 1;
		board[x][y] = S;
		if( check_N_horizontal(board, x+1, y, adversaire, W, H, 4) || check_N_vertical(board, x+1, y, adversaire, W, H, 4) || check_N_diagonale_pente_positive(board, x+1, y, adversaire, W, H, 4) || check_N_diagonale_pente_negative(board, x+1, y, adversaire, W, H, 4)){
			board[x+1][y] = adversaire;
			if(check_N_horizontal(board, x+2, y, adversaire, W, H, 4) || check_N_vertical(board, x+2, y, adversaire, W, H, 4) || check_N_diagonale_pente_positive(board, x+2, y, adversaire, W, H, 4) || check_N_diagonale_pente_negative(board, x+2, y, adversaire, W, H, 4)){
				nombre = 0;
			}
			board[x+1][y] = 0;
		}
		board[x][y] = 0;
		reward += nombre * dontGiveForceWin;
	}

	//Coup du traitre
	if(saleTraitre(board, x, y, S, W, H)){
		reward += coupDuTraitre;
	}


	//Stop coup du traitre
	if(saleTraitre(board, x, y, adversaire, W, H)){
		reward += stopCoupDuTraitre;
	}
	
	//Connecter trois pions qui ont du potentiel
	//TODO si on a du potentiel dans deux sens différents, nombre doit être incrémenté de deux en pas 1
	nombre = 0;
	if(check_N_horizontal(board, x, y, S, W, H, 3)){
		nombre += potentiel_N_horizontal(board, x, y, S, W, H, 3);
		// cout << "connect 3 with potential horizontaly : " << nombre << endl;
	}
	if(check_N_vertical(board, x, y, S, W, H, 3)){
		nombre += potentiel_N_vertical(board, x, y, S, W, H, 3);
		// cout << "connect 3 with potential verticaly : " << nombre << endl;
	}
	if(check_N_diagonale_pente_positive(board, x, y, S, W, H, 3)){
		nombre += potentiel_N_diagonale_pente_positive(board, x, y, S, W, H, 3);
		// cout << "connect 3 with potential diagonale pente positive : " << nombre << endl;
	}
	if(check_N_diagonale_pente_negative(board, x, y, S, W, H, 3)){
		nombre += potentiel_N_diagonale_pente_negative(board, x, y, S, W, H, 3);
		// cout << "connect 3 with potential diagonale pente negative : " << nombre << endl;
	}

	reward += nombre * connect3WithPotential;

	//Jouer au centre
	if(W == 7 && H == 6){
		reward += coeffCentre * possibilitesWin[x][y];
	} else {
		reward += playCenter - abs(milieu - y)*2;
	}

	//Empêcher de connecter trois pions qui ont du potentiel
	nombre = 0;
	if(check_N_horizontal(board, x, y, adversaire, W, H, 3)){
		nombre += potentiel_N_horizontal(board, x, y, adversaire, W, H, 3);
		// cout << "stopConnect3 horizontaly nombre : " << nombre << endl;
	}
	if(check_N_vertical(board, x, y, adversaire, W, H, 3)){
		nombre += potentiel_N_vertical(board, x, y, adversaire, W, H, 3);
		// cout << "stopConnect3 verticaly nombre : " << nombre << endl;
	}
	if(check_N_diagonale_pente_positive(board, x, y, adversaire, W, H, 3)){
		nombre += potentiel_N_diagonale_pente_positive(board, x, y, adversaire, W, H, 3);
		// cout << "stopConnect3 diagonale pente positive : " << nombre << endl;
	}
	if(check_N_diagonale_pente_negative(board, x, y, adversaire, W, H, 3)){
		nombre += potentiel_N_diagonale_pente_negative(board, x, y, adversaire, W, H, 3);
		// cout << "stopConnect3 diagonale pente negative : " << nombre << endl;
	}
	reward += nombre * stopConnect3WithPotential;
	
	//Connecter deux pions qui ont du potentiel
	nombre = 0;
	if(check_N_horizontal(board, x, y, S, W, H, 2)){
		nombre += potentiel_N_horizontal(board, x, y, S, W, H, 2);
		// cout << "connect2 horizontaly : " << nombre << endl;
	}
	if(check_N_vertical(board, x, y, S, W, H, 2)){
		nombre += potentiel_N_vertical(board, x, y, S, W, H, 2);
		// cout << "connect2 verticaly : " << nombre << endl;
	}
	if(check_N_diagonale_pente_positive(board, x, y, S, W, H, 2)){
		nombre += potentiel_N_diagonale_pente_positive(board, x, y, S, W, H, 2);
		// cout << "connect2 diagonale pente positive : " << nombre << endl;
	}
	if(check_N_diagonale_pente_negative(board, x, y, S, W, H, 2)){
		nombre += potentiel_N_diagonale_pente_negative(board, x, y, S, W, H, 2);
		// cout << "connect2 diagonale pente negative : " << nombre << endl;
	}
	reward += nombre * connect2WithPotential;
	
	return reward;
}
int strategy(vector<vector<int>> & board, int S, int W, int H){
	int maxPoints = reward(board, 0, S, W, H);
	int y = 0;
	int points;
	for(int i = 1; i < W; ++i){
		points = reward(board, i, S, W, H);
		if(points>maxPoints){
			maxPoints = points;
			y = i;
		}
	}	
	return y;
}


int main(){
	//Récupération des valeurs du cin et création du plateau
	int W,H,N,S,i;
	cin >> W >> H >> N >>S;
	vector<vector<int>> board;
	for(i = 0; i < H; ++i){
		vector<int> myRow(W, 0);
		board.push_back(myRow);
	}

	//Gestion du jeu
	int player = 0;
	int x, y;

	while(1){
		player = player % N + 1;
		if(player == S){
			y = strategy(board, S, W, H); 
			cout << y << endl;
		} else {
			cin >> y;
		}
		x = fallHeight(board, y, H);
		board[x][y] = player;
		// Afficher(board);
	}	
}

void Afficher(vector<vector<int>> & board){
	for(int i = board.size()-1; i >= 0; --i){
		for(int j = 0; j < board[0].size(); ++j){
			if(board[i][j]){
				cout << board[i][j] << " ";
			} else {
				cout << ". ";
			}
		}
		cout << endl;
	}
	cout << "--------------------------------" << endl;
}

int check_N_horizontal(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N){
	// int adversaire = S % 2 + 1;
	// //On vérifie l'horizontale
	// vector<int>::iterator it = board[x].begin();
	// vector<int>::iterator debut = it;
	// vector<int>::iterator fin = board[x].end();
	// --fin;
	// int posFin = W - 1;
	// for(int i = 0; i < y; ++i){
	// 	if( *it == adversaire || *it == 0){
	// 		debut = it;
	// 		++it;
	// 	} else {
	// 		++it;
	// 	}
	// }
	// it = fin;
	// for(int i = W - 1; i > y; --i){
	// 	if(*it == adversaire || *it == 0){
	// 		fin = it;
	// 		posFin = i;
	// 		--it;
	// 	} else {
	// 		--it;
	// 	}
	// }
	// //cout << "appel pour N = " << N << " last : " << *fin  << " posfin : " << posFin << endl;
	// board[x][y] = S;
	// vector<int>::iterator it2 = search_n(debut, fin, N, S);
	// if(it2 != fin){
	// 	board[x][y] = 0;
	// 	return 1;
	// } else {
	// 	board[x][y] = 0;
	// }
	// return 0;
	
	int serieTrouvee = 0;
	int yprime = 0;
	int compteur = 0;
	int debutSerie;
	int finSerie;
	board[x][y] = S;
	while(yprime < W){
		while( yprime < W && board[x][yprime] != S){
			yprime++;
		}
		// cout << "yprime trouve : " << yprime << endl;
		debutSerie = yprime;
		compteur = 1;
		finSerie = debutSerie;
		yprime++;
		while(yprime < W && board[x][yprime] == S){
			finSerie++;
			compteur++;
			yprime++;
		}
		if(compteur >= N && debutSerie <= y && finSerie >= y){
			serieTrouvee = 1;
			break;
		}
	}
	board[x][y] = 0;
	// cout << "oki" << endl;
	return serieTrouvee;
}

int check_N_vertical(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N){
	//On vérifie la verticale.
	if(x >= N-1){
		int xprime = x - 1;
		int res = 1;
		while(xprime > x - N){
			if(board[xprime][y] != S){
				res = 0;
				break;
			}
			--xprime;
		}
		if(res){
			return 1;
		}
	}
	return 0;
}

int check_N_diagonale_pente_positive(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N){
	//On vérifie la diagonale pente positive
	int nbPions = 1;
	int xprime = x;
	int yprime = y;
	while(xprime > 0 && yprime > 0){
		if(board[xprime-1][yprime-1] == S){
			nbPions++;
		} else {
			break;
		}
		--xprime;
		--yprime;
	}
	xprime = x;
	yprime = y;
	while(xprime < H - 1 && yprime < W - 1){
		if(board[xprime+1][yprime+1] == S){
			nbPions++;
		} else {
			break;
		}
		++xprime;
		++yprime;
	}
	if(nbPions >= N){
		return 1;
	}
	return 0;
}

int check_N_diagonale_pente_negative(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N){
	//On vérifie la diagonale pente négative
	int nbPions = 1;
	int xprime = x;
	int yprime = y;
	while(xprime < H - 1 && yprime > 0){
		if(board[xprime+1][yprime-1] == S){
			nbPions++;
		} else {
			break;
		}
		++xprime;
		--yprime;
	}
	xprime = x;
	yprime = y;
	while(xprime > 0 && yprime < W - 1){
		if(board[xprime-1][yprime+1] == S){
			nbPions++;
		} else {
			break;
		}
		--xprime;
		++yprime;
	}
	if(nbPions >= N){
		return 1;
	}
	
	return 0;
}

int potentiel_N_horizontal(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N){
	// int adversaire = S % 2 + 1;
	// int res = 0;
	// int compteur;
	// //Horizontal
	// board[x][y] = S;
	// vector<int>::iterator itDebut;
	// vector<int>::iterator itFin;
	// vector<int>::iterator it = board[x].begin();
	// vector<int>::iterator it2;
	// vector<int>::iterator debut = board[x].begin();
	// vector<int>::iterator fin = board[x].end();
	// --fin;
	// int posFin = W - 1;
	// for(int i = 0; i < y; ++i){
	// 	if( *it == adversaire || *it == 0){
	// 		debut = it;
	// 		++it;
	// 	} else {
	// 		++it;
	// 	}
	// }
	// it = fin;
	// for(int i = W - 1; i > y; --i){
	// 	if(*it == adversaire || *it == 0){
	// 		fin = it;
	// 		posFin = i;
	// 		--it;
	// 	} else {
	// 		--it;
	// 	}
	// }
	// itDebut = search_n(debut, fin, N, S);
	// itFin = itDebut;
	// for(int i = 0; i < N-1; ++i){
	// 	++itFin;
	// }
	// compteur = N;
	// it = itDebut;
	// it2 = itFin;
	// ++it2;
	// if(it != board[0].begin()){
	// 	--it;
	// }
	// while(it != board[x].begin()){
	// 	if(*it == 0 || *it == S){
	// 		compteur++;
	// 		--it;
	// 	} else {
	// 		break;
	// 	}
	// }
	// if(it == board[x].begin()){
	// 	if(*it == 0 || *it == S){
	// 		compteur++;
	// 	}
	// }
	// while(it2 != board[x].end()){
	// 	if(*it2 == 0 || *it2 == S){
	// 		compteur++;
	// 		++it2;
	// 	} else {
	// 		break;
	// 	}
	// }
	// if(compteur >= 4){
	// 	res++;
	// }
	// board[x][y] = 0;
	// return res;
	
	int serieTrouvee = 0;
	int yprime = 0;
	int compteur = 0;
	int debutSerie;
	int finSerie;
	board[x][y] = S;
	while(yprime < W){
		while(yprime < W && board[x][yprime] != S){
			yprime++;
		}
		debutSerie = yprime;
		compteur = 1;
		finSerie = debutSerie;
		yprime++;
		while(yprime < W && board[x][yprime] == S){
			finSerie++;
			compteur++;
			yprime++;
		}
		if(compteur >= N && debutSerie <= y && finSerie >= y){
			serieTrouvee = 1;
			break;
		}
	}
	board[x][y] = 0;
	if(serieTrouvee){
		debutSerie--;
		finSerie++;
		while(debutSerie >= 0 && board[x][debutSerie] == 0){
			compteur++;
			debutSerie--;
		}
		while(finSerie < W && board[x][finSerie] == 0){
			compteur++;
			finSerie++;
		}
		if(compteur >= 4){
			return 1;
		} else {
			return 0;
		}
	}
	// cout << "oki2" << endl;
	return 0;
}

int potentiel_N_vertical(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N){
	int res = 0;
	int compteur;
	//vertical
	if(x >= N-1){
		int xprime = x - 1;
		int res2 = 1;
		while(xprime > x - N){
			if(board[xprime][y] != S){
				res2 = 0;
				break;
			}
			--xprime;
		}
		if(res2){
			if( x + (4-N) < H){
				++res;
			}
		}
	}
	return res;
}

int potentiel_N_diagonale_pente_positive(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N){
	int res = 0;
	int compteur;
	//Diagonale pente positive
	compteur = 1;
	int xprime = x;
	int yprime = y;
	while(xprime > 0 && yprime > 0){
		if(board[xprime-1][yprime-1] == S || board[xprime-1][yprime-1] == 0){
			compteur++;
		} else {
			break;
		}
		--xprime;
		--yprime;
	}
	xprime = x;
	yprime = y;
	while(xprime < H - 1 && yprime < W - 1){
		if(board[xprime+1][yprime+1] == S || board[xprime+1][yprime+1] == 0){
			compteur++;
		} else {
			break;
		}
		++xprime;
		++yprime;
	}
	if(compteur >= 4){
		res++;
	}
	return res;
}

int potentiel_N_diagonale_pente_negative(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N){
	int res = 0;
	int compteur;
	//Diagonale pente négative
	compteur = 1;
	int xprime = x;
	int yprime = y;
	while(xprime < H - 1 && yprime > 0){
		if(board[xprime+1][yprime-1] == S || board[xprime+1][yprime-1] == 0){
			compteur++;
		} else {
			break;
		}
		++xprime;
		--yprime;
	}
	xprime = x;
	yprime = y;
	while(xprime > 0 && yprime < W - 1){
		if(board[xprime-1][yprime+1] == S || board[xprime-1][yprime+1] == 0){
			compteur++;
		} else {
			break;
		}
		--xprime;
		++yprime;
	}
	if(compteur >= 4){
		res++;
	}
	return res;
}

int saleTraitre(vector<vector<int>> & board, int x, int y, int S, int W, int H){

	int traitrise = 0;
	int serieTrouvee = 0;
	int yprime = 0;
	int compteur = 0;
	int debutSerie;
	int finSerie;
	board[x][y] = S;
	while(yprime < W){
		while( yprime < W && board[x][yprime] != S){
			yprime++;
		}
		// cout << "yprime trouve : " << yprime << endl;
		debutSerie = yprime;
		compteur = 1;
		finSerie = debutSerie;
		yprime++;
		while(yprime < W && board[x][yprime] == S){
			finSerie++;
			compteur++;
			yprime++;
		}
		if(compteur >= 3 && debutSerie <= y && finSerie >= y){
			serieTrouvee = 1;
			break;
		}
	}
	board[x][y] = 0;
	if(serieTrouvee){
		if(finSerie < W-1){
			if(debutSerie > 0){
				if( board[x][debutSerie-1] == 0 && board[x][finSerie+1] == 0){
					if(x == 0 || (x > 0 && board[x-1][debutSerie-1] > 0 && board[x-1][finSerie+1] > 0)){
						traitrise = 1;
					}
				}
			}
		}
	}
	return traitrise;
}
