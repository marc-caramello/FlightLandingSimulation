#ifndef START_H
#define START_H

#include <cjson/cJSON.h>
#include <ncurses/ncurses.h>

#include <stdio.h>
#include <string.h>
#include <urlmon.h>
#include <windows.h>

#define MAX_FLIGHTS_TO_STORE 100

typedef struct {
    char flight_iata[7];
    char dep_iata[4];
    char arr_estimated_utc[30];
} FlightInfo;

void print_thankYouMessage();
void enterAirportCode_and_downloadJsonFile();
FlightInfo* parse_json(const char *filename, int* arraySize);
void startNCurses();
void executeEachNewMinute(FlightInfo *allFlights, char currentTime[], int* currentIndex);

#endif