#include <bits/stdc++.h>
#include <cstdlib>
#include <curses.h>
#include <unistd.h>

// initial board state
std::vector<std::vector<bool>> filled(11, std::vector<bool>(7, 0));
char present = 'X', blank = ' ';
int cRow = 10;
int sCol = 0;
bool lost = 0;
int stackSize = 3;
int direction = 1;
int speed = 1000000;
//print out the board, based on filled items in the "filled" vector
void print_board()
{
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(3));
	for (int i = 0; i < filled[0].size(); i++)
	{
		printw(" ---");
	}
	printw("\n");
	for (auto i = filled.begin(); i != filled.end(); i++)
	{
		printw("| ");
		for (auto j = (*i).begin(); j != (*i).end(); j++)
		{
			if (*j)
			{
				attron(COLOR_PAIR(1));
				printw("%c", present);
			}
			else
			{
				attron(COLOR_PAIR(2));
				printw("%c", blank);
			}
			attron(COLOR_PAIR(3));
			printw(" | ");
		}
		attron(COLOR_PAIR(3));
		printw("\n");
		for (int j = 0; j < filled[0].size(); j++)
		{
			printw(" ---");
		}
		printw("\n");
	}
	printw("\n");
	refresh();
}

//non-blocking check on the keyboard press
int kbhit(void)
{
	int ch = getch();

	if (ch != ERR) {
		ungetch(ch);
		return 1;
	}
	else {
		return 0;
	}
}
bool clicked = false;
void game_loop() {
	//lets optimize!
	nodelay(stdscr, TRUE);
	//pthread_t user_input;
	//pthread_create(&user_input, NULL, &input_checker, NULL);
	char ch;
	int k = 0;
	do {

		for (int i = 0; i < stackSize; i++)
			filled[cRow][sCol + i] = 1;
		print_board();
		usleep(speed);
		ch = getch();
		if (ch != ERR) {
			cRow--, sCol = 0;
			erase();
			switch (cRow) {
			case 7: stackSize = 2; break;
			case 1: stackSize = 1; break;
			default: break;
			}
			speed -= 90000;
			continue;
		}
		for (int i = 0; i < stackSize; i++)
			filled[cRow][sCol + i] = 0;
		if (sCol + stackSize * direction == 7) {
			direction = -1;
		}
		else if ((sCol + stackSize - 1) + stackSize * direction == -1) {
			direction = 1;
		}
		sCol += direction;
		erase();
		k++;
	}
	while (true);
	//void * stop;
	//pthread_join(user_input, &stop);
}




int main()
{
	initscr();
	start_color();
	nodelay(stdscr, TRUE);
	cbreak();
	noecho();	 // Just the usual
	curs_set(0); // Disables displaying the cursor
	game_loop();
	endwin();
}