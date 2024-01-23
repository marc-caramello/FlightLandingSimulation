#include "animation.h"

void print_runwayAnimation() {
    print_entireRunway();
    struct Pair prevPos, newPos;

    // Prints: top_left ---> bottom_right
    for (int i = 0; i <= 10; i++) {
        prevPos = newPos;
        newPos = (struct Pair){(2 * i) + 1, i};
        print_eachFrame(prevPos, newPos);
    }
    // Prints: left ---> right
    for(int i = 22; i <= 42; i++) {
        prevPos = newPos;
        newPos = (struct Pair){i, 10};
        print_eachFrame(prevPos, newPos);
    }
}

void print_entireRunway() {
    mvprintw(8,  18, " ________________________ ");
    mvprintw(9,  18, "|                        |");
    mvprintw(10, 18, "|                        |");
    mvprintw(11, 18, "|________________________|");
}

void print_eachFrame(struct Pair prevPos, struct Pair newPos) {
    mvprintw(prevPos.vert, prevPos.horiz, " ");
    mvprintw(newPos.vert, newPos.horiz, "-");
    refresh();
    Sleep(75);
}