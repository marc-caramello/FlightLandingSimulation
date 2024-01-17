#include <ncurses/ncurses.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>

/*
void printRunway() {
    printw(" -                                      \n");
    printw("   -                                    \n");
    printw("     - ________________________________ \n");
    printw("      |-                               |\n");
    printw("      |  -                             |\n");
    printw("      |    -                           |\n");
    printw("      |      --------------------------|\n");
    printw("      |                                |\n");
    printw("      |                                |\n");
    printw("      |________________________________|\n");
}
*/

void printRunway() {
    printw("                                        \n");
    printw("                                        \n");
    printw("       ________________________________ \n");
    printw("      |                                |\n");
    printw("      |                                |\n");
    printw("      |                                |\n");
    printw("      |                                |\n");
    printw("      |                                |\n");
    printw("      |                                |\n");
    printw("      |________________________________|\n");
}

int main() {
    srand(time(0));

    initscr();
    curs_set(0);
    noecho();
    cbreak();
    keypad(stdscr, true);
    
    printRunway();
    getch();
    clear();

    endwin();
    return 0;    
}




