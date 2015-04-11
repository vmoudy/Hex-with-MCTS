#ifndef hex_game_H
#define hex_game_H

#include <vector>
#include <cmath>
#include <limits>
using namespace std;

class hex_game{
	public:
		hex_game(int a);
		~hex_game();
		friend ostream &operator<<(ostream &out, const hex_game &board);
		void fill_board();
	private:
		const int size;
		void make_graph();
		bool find_winner(const vector<char> &v, const vector<vector<int>> &location);
		vector<vector<char>> hex_board;
		vector<vector<int>> hex_graph;
};
#endif // !hex_H
