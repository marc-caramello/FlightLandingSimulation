#include <cjson/cJSON.h>
#include <ncurses/ncurses.h>

#include <stdio.h>
#include <string.h>
#include <urlmon.h>
#include <windows.h>

#define MAX_FLIGHTS_TO_STORE 50

struct Pair {
    int horiz;
    int vert;
};

typedef struct {
    char flight_iata[7];
    char arr_estimated_utc[30];
} FlightInfo;

void enterAirportCode_and_downloadJsonFile();
FlightInfo* parse_json(const char *filename);

// --------------------------------------------------

void print_runwayAnimation();
void print_entireRunway();
void print_eachFrame(struct Pair prevPos, struct Pair newPos);
void print_thankYou();