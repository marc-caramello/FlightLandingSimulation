#include "animation.h"

void print_runwayAnimation() {
    print_entireRunway();
    struct Pair prevPos, newPos;

    // Prints: top_left ---> bottom_right
    for (int i = 0; i <= 6; i++) {
        prevPos = newPos;
        newPos = (struct Pair){i, (2 * i) + 1};
        print_eachFrame(prevPos, newPos);
    }
    // Prints: left ---> right
    for(int i = 14; i <= 40; i++) {
        prevPos = newPos;
        newPos = (struct Pair){6, i};
        print_eachFrame(prevPos, newPos);
    }
    print_thankYou();
}

void print_entireRunway() {
    printw("                                                    \n");
    printw("                                                    \n");
    printw("                                                    \n");
    printw("                                                    \n");
    printw("           ______________________________           \n");
    printw("          |                              |          \n");
    printw("          |                              |          \n");
    printw("          |______________________________|          \n");
}

void print_eachFrame(struct Pair prevPos, struct Pair newPos) {
    mvprintw(prevPos.horiz, prevPos.vert, " ");
    mvprintw(newPos.horiz, newPos.vert, "-");
    refresh();
    Sleep(200);
}

void print_thankYou() {
    mvprintw(6, 40, " ");
    mvprintw(9, 0, "");
    printw("           The plane successfully landed!           \n");
    refresh();
}
