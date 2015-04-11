#include <iostream>
#include "hex_game.h"

using namespace std;

int main(){
	char again;
	do{
		hex_game game(11);
		cout << game << endl;
		game.fill_board();
		cout << "Filling board..." << endl << endl;
		cout << game << endl;
		cout << "Play again? Y or N" << endl;
		cin >> again;
		again = toupper(again);
	}while(again == 'Y');
	cout << "Goodbye!" << endl;
	cin.ignore();
	cin.get();
}