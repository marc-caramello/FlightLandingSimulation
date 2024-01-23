#ifndef START_H
#define START_H

#include <cjson/cJSON.h>
#include <ncurses/ncurses.h>

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <urlmon.h>
#include <windows.h>

#define MAX_FLIGHTS_TO_STORE 5000

typedef struct {
    char arr_estimated_utc[30];
    char dep_iata[4];
    char flight_iata[7];
} FlightInfo;

void print_thankYouMessage();
void enterAirportCode(char* iataAirportCode);
void downloadJsonFile(char* iataAirportCode);
FlightInfo* parse_json(const char *filename, int* arraySize);
char* getCurrentTimeInUtc();
void startNCurses();
int getStartingIndex(FlightInfo *allFlights, char* currentTime);
void executeEachNewMinute(FlightInfo *allFlights, char* iataAirportCode, int* currentIndex);
void print_messageBeforeLoopingAgain();

#endif