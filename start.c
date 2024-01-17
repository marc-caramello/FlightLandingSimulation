#include "start.h"

int main() {
    initscr();
    curs_set(0);
    noecho();
    cbreak();
    keypad(stdscr, true);
    
    print_runwayAnimation();
    getch();
    clear();

    endwin();
    return 0;    
}

// --------------------------------------------------

void print_runwayAnimation() {
    print_entireRunway();
    struct Pair prevPos, newPos;

    // Prints: top_left ---> bottom_right
    /*for (int i = 0; i <= 5; i++) {
        prevPos = newPos;
        newPos = (struct Pair){i, (2 * i) + 1};
        print_eachFrame(prevPos, newPos);
    }
    // Prints: left ---> right
    for(int i = 13; i <= 38; i++) {
        prevPos = newPos;
        newPos = (struct Pair){6, i};
        print_eachFrame(prevPos, newPos);
    }*/
    print_thankYou();
}

void print_entireRunway() {
    printw("                                              \n");
    printw("                                              \n");
    printw("       ________________________________       \n");
    printw("      |                                |      \n");
    printw("      |                                |      \n");
    printw("      |                                |      \n");
    printw("      |                                |      \n");
    printw("      |                                |      \n");
    printw("      |                                |      \n");
    printw("      |________________________________|      \n");
}

void print_eachFrame(struct Pair prevPos, struct Pair newPos) {
    mvprintw(prevPos.horiz, prevPos.vert, " ");
    mvprintw(newPos.horiz, newPos.vert, "-");
    refresh();
    Sleep(500);
}

void print_thankYou() {
    mvprintw(13, 0, "");
    printw("       Dear,                                         \n");
    printw("                                                     \n");
    printw("       Mr. Jost,                                     \n");
    printw("       Mr. Parsons,                                  \n");
    printw("       Mr. Petrucci,                                 \n");
    printw("                                                     \n");
    printw("       Thank you for giving me the opportunity to interview for this amazing position at RTX.\n");
}