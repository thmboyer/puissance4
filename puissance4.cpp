#include <iostream>
#include <vector>
using namespace std;

int reward(vector<vector<int>> & board, int y){
	return y;
}
int strategy(vector<vector<int>> & board){
	int maxPoints = reward(board, 0);
	int y = 0;
	int points;
	for(int i = 1; i < board[0].size(); ++i){
		points = reward(board, i);
		if(points>maxPoints){
			maxPoints = points;
			y = i;
		}
	}	
	return y;
}

int fallHeight(vector<vector<int>> & board, int y){
	int x = board.size();
	while(x > 0 && board[x-1][y] == 0){
		x -= 1;
	}
	return x;
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
			y = strategy(board); 
			cout << y << endl;
		} else {
			cin >> y;
		}
		y = fallHeight(board, y);
		board[x][y] = player;
	}	
}
