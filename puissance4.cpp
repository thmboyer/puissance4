#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
using namespace std;

//Récompenses en puissance de 2 pour par exemple que win ne puisse pas se faire rattraper par la somme des autres
int win = 128;
int stopWin = 64;
int forceWin = 32;
int stopForceWin = 16;
int connect3WithPotential = 8;
int playCenter = 4;
int stopConnect3WithPotential = 2;
int connect2WithPotential = 1;

void Afficher(vector<vector<int>> & board);
int check_N(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N);

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
	
	//Win la partie
	if(check_N(board, x, y, S, W, H, 4)){
		reward += win;
	}
	
	//Empêcher une win
	int adversaire = S % 2 + 1;
	if(check_N(board, x, y, adversaire, W, H, 4)){
		reward += stopWin;
	}

	//Force win
	
	//Empêcher une force win
	
	//Connecter trois pions qui ont du potentiel
	
	//Jouer au centre

	//Par rapport au milieu
	reward += playCenter - abs(milieu - y);
	
	//Connecter deux pions qui ont du potentiel
	
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
			cout << board[i][j];
		}
		cout << endl;
	}
}

int check_N(vector<vector<int>> & board, int x, int y, int S, int W, int H, int N){
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
	//On vérifie l'horizontale
	vector<int>::iterator it;
	board[x][y] = S;
	it = search_n(board[x].begin(), board[x].end(), N, S);
	if(it != board[x].end()){
		board[x][y] = 0;
		return 1;
	} else {
		board[x][y] = 0;
	}
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
	//On vérifie la diagonale pente négative
	nbPions = 1;
	xprime = x;
	yprime = y;
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
