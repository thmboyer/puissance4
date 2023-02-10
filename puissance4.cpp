#include <iostream>
#include <vector>
using namespace std;

int main(){
	//Récupération des valeurs du cin et création du plateau
	int W,H,N,S,i;
	cin >> W >> H >> N >>S;
	vector<vector<int>> board;
	for(i = 0; i < H; ++i){
		vector<int> myRow(W, 0);
		board.push_back(myRow);
	}
}
