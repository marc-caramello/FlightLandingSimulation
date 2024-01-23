#include "../include/animation.h"

void print_runwayAnimation() {
    print_entireRunway();
    struct Pair prevPos, newPos;

    // Prints: top_left ---> bottom_right
    for (int i = 0; i <= 11; i++) {
        prevPos = newPos;
        newPos = (struct Pair){(2 * i) + 1, i};
        print_eachFrame(prevPos, newPos);
    }
    // Prints: left ---> right
    for(int i = 24; i <= 42; i++) {
        prevPos = newPos;
        newPos = (struct Pair){i, 11};
        print_eachFrame(prevPos, newPos);
    }
}

void print_entireRunway() {
    mvprintw(9,  20, " ________________________ ");
    mvprintw(10, 20, "|                        |");
    mvprintw(11, 20, "|                        |");
    mvprintw(12, 20, "|________________________|");
}

void print_eachFrame(struct Pair prevPos, struct Pair newPos) {
    mvprintw(prevPos.vert, prevPos.horiz, "  ");
    mvprintw(newPos.vert, newPos.horiz, "-|");
    refresh();
    Sleep(50);
}