#ifndef ANIMATION_H
#define ANIMATION_H

#include <ncurses/ncurses.h>
#include <windows.h>

struct Pair {
    int horiz;
    int vert;
};

void print_runwayAnimation();
void print_entireRunway();
void print_eachFrame(struct Pair prevPos, struct Pair newPos);

#endif