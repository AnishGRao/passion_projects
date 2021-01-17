#include <bits/stdc++.h>
#include <cstdlib>
#include <curses.h>
#include <iostream>
#include <utility>
#include <pthread.h>
#include <chrono>
#include <unistd.h>


char GLOBAL_CHAR = 'e';


void * threaded_portion(void *) {
    char ch;
    while (true) {
        if (GLOBAL_CHAR = getch() != ERR) {
            GLOBAL_CHAR = 'A';
            break;
        }
        printw("%c", GLOBAL_CHAR);
    }
}

void test_1() {
    pthread_t checkChar;
    pthread_create(&checkChar, NULL, &threaded_portion, NULL);

    while (GLOBAL_CHAR != 'A') {
        printw("waiting...\n");
        erase();
    }
    printw("Succeeded in finding user input.");
    void * ender;
    pthread_join(checkChar, &ender);
}


int main() {
    initscr();
    start_color();
    nodelay(stdscr, TRUE);
    cbreak();
    noecho();	 // Just the usual
    curs_set(0); // Disables displaying the cursor
    test_1();
    endwin();
}