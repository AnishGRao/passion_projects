#include <bits/stdc++.h>
#include <curses.h>
#include <unistd.h>
#include <pthread.h>
#include <cstdlib>
// single square Class declaration
class cell;
int board_size = 30;
int sleeptime;
bool grid = 0;
// board dec/init (resize possible in inputfile) object
std::vector<std::vector<cell>> old_board(board_size, std::vector<cell>(board_size)), new_board(board_size, std::vector<cell>(board_size));
// single square Class initialization
class cell {
public:
	int row, col;
	bool alive = 0;
	// maybe switch
	char img[1];
	cell() { ; }
	// what people do not mention about conway's GOL, is the order of operations.
	// If you were to follow instructions exactly, it would be flawed.
	// You must first do the dead cell operation, then proceed to the living cell
	// operations, all on a board copy which does not change
	void execute() {
		// check all neighbors for num_neighbors
		int num_alive_neighbors = 0;
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
				num_alive_neighbors +=
				(i == j && i == 0) ? 0 : old_board[row + i][col + j].alive;

		// game of life rules.
		if (num_alive_neighbors < 2)
			new_board[row][col].alive = 0;
		else if (num_alive_neighbors == 3 || (num_alive_neighbors == 2 && old_board[row][col].alive))
			new_board[row][col].alive = 1;
		else if (num_alive_neighbors > 3)
			new_board[row][col].alive = 0;

		new_board[row][col].img[0] = new_board[row][col].alive ? 'X' : ' ';
	}
};

void preload(char * argv) {
	// small ver only with 50X50
	// infile which gives two nums per line, row and col
	FILE * src = fopen(argv, "r");
	char buf[100];
	char t1[10], t2[10];
	while (fgets(buf, sizeof buf, src)) {
		if (sscanf(buf, "%s %s", t1, t2) < 2)
			break;
		old_board[std::stoi(t1)][std::stoi(t2)].alive = true;
	}
	for (int i = 0; i < board_size; i++)
		for (int j = 0; j < board_size; j++)
			new_board[i][j].row = old_board[i][j].row = i,
			new_board[i][j].col = old_board[i][j].col = j,
			old_board[i][j].img[0] = old_board[i][j].alive ? 'X' : ' ';
}

void print_game() {
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(3));
	if (grid) {
		for (int i = 1; i < board_size - 1; i++) {
			printw(" ---");
		}
		printw("\n");
	}
	char temp[1];
	for (auto i = old_board.begin() + 1; i != old_board.begin() + old_board.size() - 1; i++) {
		if (grid) {
			printw("| ");
		}
		for (auto j = (*i).begin() + 1; j != i->begin() + old_board[0].size() - 1; j++) {
			temp[0] = j->img[0];
			if (j->img[0] == 'X') {
				attron(COLOR_PAIR(1));
				printw("%s", temp);
			}
			else {
				attron(COLOR_PAIR(2));
				printw("%s", temp);
			}
			attron(COLOR_PAIR(3));
			if (grid) {
				printw(" | ");
			}
		}
		attron(COLOR_PAIR(3));
		if (grid) {
			printw("\n");
			for (int j = 1; j < board_size - 1; j++) {
				printw(" ---");
			}
		}
		printw("\n");
	}
	printw("\n");
	refresh();
}

void row_operation(int rownum) {
	//int rownum = *((int *) (&_rownum));
	for (int i = 1; i < board_size - 1; i++) {
		old_board[rownum][i].execute();
	}
}

void start_game(char * argv) {
	int iter_num = atoi(argv);

	std::vector<std::thread> thread_vec;

	for (int j = 0; j < (iter_num ? : 10); j++) {

		thread_vec.reserve(board_size - 2);

		for (int i = 1; i < board_size - 1; i++)
			thread_vec.emplace_back(row_operation, i);

		for (auto & th : thread_vec)
			if (th.joinable())
				th.join();

		old_board = new_board;
		usleep(100000);
		erase();
		// clear();
		print_game();

		thread_vec.clear();
	}
}

int main(int argc, char ** argv) {
	WINDOW * win;
	win = initscr();
	// wresize(win, 10000, 10000);
	// resize_term(10000, 10000);
	refresh();
	start_color();
	cbreak();
	noecho();    // Just the usual
	curs_set(0); // Disables displaying the cursor
	if (argc == 4)
		grid = true;
	preload(argv[1]);
	(void) noecho();
	print_game();
	sleep(1);
	start_game(argv[2]);
	sleep(1);
	endwin();
}