#include <ncurses/ncurses.h>
#include <stdio.h>
#include <windows.h>

struct Pair {
    int horiz;
    int vert;
};

void print_entireRunway();
void print_runwayAnimation();
void print_eachFrame(struct Pair prevPos, struct Pair newPos);