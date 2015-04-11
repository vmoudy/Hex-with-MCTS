#include <iostream>
#include <random>
#include <iomanip>
#include <stack>
#include "hex_game.h"


default_random_engine generater(static_cast<int>(time(nullptr)));

//distributer for numbers from 1 to board size
uniform_int_distribution<int> tile_distributor(1, 11);

//return an int for tile selection
inline int which_tile(){
	return (1 * tile_distributor(generater));
}

hex_game::hex_game(int a) : size(a+1){

	// = 11, actual board size since the incoming size is + 1 to avoid (0,0) in the matrix
	int y = size - 1; 

	// 11*11 = 121
	int s = static_cast<int>(pow(y,2));

	hex_board.resize(size, vector<char>(size, '-'));//board, '-' = open, 'r' or 'b' = closed
	hex_graph.resize(s+1, vector<int>(s+1, 0)); // s by size matrix 
												  // representing the nodes and connectivity,
	                                              // 1=connected, 0 = not connected
	//make the graph, code too large, made seperate fcn
	make_graph();
}

//just for good measure
hex_game::~hex_game(){
	//cout << "destroyed" << endl;
}

//make the matrix graph for nodes 1-size^2
void hex_game::make_graph(){

	// = 10
	int x = size - 2; 

	// = 11, actual board size since the incoming size is + 1 to avoid (0,0) in the matrix
	int y = size - 1; 

	// 11*11 = 121
	int s = static_cast<int>(pow(y,2));
	//this forms the 121 by 121 matrix of nodes with their connectivity
	for(int i = 1; i <= s; i++){
		//middle nodes on the board, all degree 6
		if((i % y != 1 && i % y != 0) && (i > y && i < s - x)){ 
			hex_graph[i][i-x] = 1; //top left node
			hex_graph[i][i-y] = 1; //top right node
			hex_graph[i][i-1] = 1; //left node
			hex_graph[i][i+1] = 1; //right node
			hex_graph[i][i+x] = 1; //bottom left node
			hex_graph[i][i+y] = 1; //bottom right node
		}
		// top row nodes w/o corners, all degree 4
		else if(i > 1 && i < y){ 
			hex_graph[i][i-1] = 1; //left node
			hex_graph[i][i+1] = 1; //right node
			hex_graph[i][i+x] = 1; //bottom left node
			hex_graph[i][i+y] = 1; //bottom right node
		}
		// bottom row nodes w/o corners, all degree 4
		else if((i > s - y) && (i < s)){ 
			hex_graph[i][i-1] = 1; //left node
			hex_graph[i][i+1] = 1; //right node
			hex_graph[i][i-x] = 1; //top left node
			hex_graph[i][i-y] = 1; //top right node
		}
		// left column nodes w/o corners, all degree 4
		else if((i % y == 1) && ((i != 1) && (i != s - y))){
			hex_graph[i][i-y] = 1; // top left node
			hex_graph[i][i-x] = 1; //top right node
			hex_graph[i][i+1] = 1; //right node
			hex_graph[i][i+x] = 1; //bottom right node
		}
		//right column w/o corners, all degree 4
		else if((i % y == 0) && (i != y && i!= s)){
			hex_graph[i][i-y] = 1; // top left node
			hex_graph[i][i-1] = 1; // left node
			hex_graph[i][i+y] = 1; // bottom right node
			hex_graph[i][i+x] = 1; // bottom left node
		}
		//a corner
		else{
			//top left corner, degree 2
			if(i == 1){
				hex_graph[i][i+1] = 1; //right node
				hex_graph[i][i+y] = 1; //bottom right node
			}
			//bottom right node, degree 2
			else if(i == s){
				hex_graph[i][i-y] = 1; //top left node
				hex_graph[i][i-1] = 1; //left node
			}
			//bottom left corner, degree 3
			else if(i % y == 1){
				hex_graph[i][i-y] = 1;//top left node
				hex_graph[i][i-x] = 1;//top right node
				hex_graph[i][i+1] = 1;//right node
			}
			//and finally, top right node, degree 3
			else if(i == y){
				hex_graph[i][i-1] = 1;//left node
				hex_graph[i][i+x] = 1;//bottom left node
				hex_graph[i][i+y] = 1;//bottom right node
			}
		}
	}
}

ostream &operator<<(ostream &out, const hex_game &board){
		out << setw(2);
		for(int i = 1; i < board.size; i++)
			out << " " << static_cast<char>(64 + i) << " ";
	out << endl;
	for(int i = 1; i < board.size; i++){
		out << setw(i+4);
		for(int j = 1; j < board.size; j++){
			out << "/\\ "; // double \\ becuase it has to ignore itself
		}
		out << endl;
		out << setw(i);
		out << i;
		for(int k = 1; k < board.size; k++){
			out <<  " " << board.hex_board[i][k] << " ";
		}
		out << endl;
		out << setw(i+4);
		for(int m = 1; m < board.size; m++){
			out << "\\/ ";
		}
		if(i == board.size / 2)
			out << "  BLUE";
		if(i == board.size - 1)
			out << endl << setw(i*2+5) << "RED";
		out << endl;
	}

	return out;
}

//filling board with random generator
void hex_game::fill_board(){

	int count = 0; // to count moves played
	int count_red = 0, count_blue = 0;
	int s = static_cast<int>(pow(size-1, 2));
	bool player1 = true;
	int row, col;
	//location is a vector that holds the nodes location, ie (1,1) is node 1
	// and (11,11) is node 121
	vector<vector<int>> location;
	location.resize(size, vector<int>(size, 0));
	for(int i = 1; i < size; i++){
		for(int j = 1; j < size; j++)
			location[i][j] = ++count;
	}
	count = 0;
	//v will help determine a winner...i think...
	vector<char> v;
	v.resize(s + 1, '-');
	//fill board with random moves
	while(count < s){
		do{
			row = which_tile();
			col = which_tile();
		}while(hex_board[row][col] != '-');

		if(player1){
			hex_board[row][col] = 'r';
			v[location[row][col]] = 'r';
			player1 = false;
			count++;
			count_red++;
		}
		else{
			hex_board[row][col] = 'b';
			v[location[row][col]] = 'b';
			player1 = true;
			count_blue++;
			count++;
		}
	}
	//find if player one has won
	if(find_winner(v, location))
		cout << endl <<"Red player wins!!" << endl;
	else
		cout << endl << "Blue player wins!!" << endl;
	//display moves 
	cout << endl << "Moves" << endl << "Total = " << count << endl
		 << "Red = " << count_red << endl
		 << "Blue = " << count_blue << endl;

	//test to see if v was loaded right
	//for(int i = 1; i <= s; i++)
		//cout << v[i] << endl;

}

//finding winner
bool hex_game::find_winner(const vector<char> &v, const vector<vector<int>> &location){
	int s = size - 1;
	int a = static_cast<int>(pow(s,2));
	bool winner = false;
	stack<char> stk;
	vector<bool> visited((s*s + 1), false);

	//checking for win for player 1, 'r' color, top down
	//checks for 'r' on first row
	for(int i = 1; i < size; i++){
		if(hex_board[1][i] == 'r'){
			stk.push(i);
			visited[i] = true;
		}
	}
	//while plaer one hasn't won yet and nodes to preocess
	while(!stk.empty() && !winner){
		int top = stk.top();
		//checks if current node has reached the bottom, if so, winner == true
		if(top <= a && top > (a - s)){
			winner = true;
			break;
		}
		//loop through the nodes edges for connectivity and color value
		for(int i = 1; i <= a; i++){
			//if node i == is connected to top node and it is r and hasn't been pushed to stk
			if((hex_graph[top][i] == 1 && v[i] == 'r') && (!visited[i])){
				stk.push(i);
				visited[i] = true;
			}
		}
		//no new nodes to push, therefore no connecvitiy, check other nodes for a path.
		if(top == stk.top())
			stk.pop();
	}
	//player one has won
	if(winner)
		return true;
	//player two has won
	return false;
}