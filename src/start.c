#include "../include/animation.h"
#include "../include/start.h"

int main() {
    // UNCOMMENT THIS AFTER TESTING, START
    //print_thankYouMessage();
    // UNCOMMENT THIS AFTER TESTING, END
    
    char* iataAirportCode;
    enterAirportCode(iataAirportCode);

    do{
        startNCurses();
        downloadJsonFile(iataAirportCode);
        int arraySize;
        FlightInfo *allFlights = parse_json("C:\\Users\\marcc\\OneDrive\\Desktop\\CodingProjects\\Cursor\\FlightLandingSimulation\\temp.json", &arraySize);
        
        char* currentTime = malloc(10);
        strcpy(currentTime, getCurrentTimeInUtc());
        
        //int currentIndex = getStartingIndex(allFlights, currentTime);
        int currentIndex = 0;
        do {
            executeEachNewMinute(allFlights, iataAirportCode, &currentIndex);
        } while(currentIndex < arraySize - 3);
        print_messageBeforeLoopingAgain();
    } while(true);
    return 0;
}

// --------------------------------------------------

void print_thankYouMessage()
{
    system("color 0f");
    
    printf("Dear STARS team,\n");
    printf("Thank you so much for inviting me to join the team.\n");
    printf("I am so excited about this opportunity of a lifetime.\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("Please wait 10 seconds for this screen to exit");

    Sleep(10000);
    system("cls");
}

void enterAirportCode(char* iataAirportCode)
{
    do {
        printf("Please enter the IATA airport code: ");
        char userInput[100];
        fgets(userInput, sizeof(userInput), stdin);
        userInput[strcspn(userInput, "\n")] = 0; 

        bool allLettersAreAlpha = true;

        for(int i = 0; i < strlen(userInput); i++) {
            userInput[i] = toupper(userInput[i]);

            if(!isalpha(userInput[i])) {
                allLettersAreAlpha = false;
            }
        }
        if(strlen(userInput) == 3 && allLettersAreAlpha) {
            strcpy(iataAirportCode, userInput);
            system("cls");
            break;
        }
        else {
            printf("\"%s\" is not an acceptable IATA airport code\n", userInput);
            printf("\n");
            printf("\n");
            printf("\n");
            printf("Please wait 5 seconds for this screen to exit");
            Sleep(5000);
            system("cls");
        }
    } while(true);
}

void downloadJsonFile(char* finalInput)
{
    char currentDirectory[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDirectory);

    printw("Connecting to external API\n");
    printw("\n");
    printw("\n");
    printw("\n");
    printw("Please wait 5 seconds for this screen to exit");
    refresh();
    Sleep(5000);

    clear();
    const char url_part1[] = "https://airlabs.co/api/v9/schedules?arr_iata=";
    const char url_part2[] = "&_fields=arr_estimated_utc,dep_iata,flight_iata,&api_key=7fd7db1a-b297-413f-b998-817aa63226d7";
    int finalUrl_size = strlen(url_part1) + strlen(finalInput) + strlen(url_part2) + 1;
    char finalUrl_narrow[finalUrl_size];
    strcpy(finalUrl_narrow, url_part1);
    strcat(finalUrl_narrow, finalInput);
    strcat(finalUrl_narrow, url_part2);

    char filePath[MAX_PATH];
    snprintf(filePath, MAX_PATH, "%s\\temp.json", currentDirectory);

    printw("Downloading json file from external API\n");
    printw("\n");
    printw("\n");
    printw("\n");
    printw("Please wait 5 seconds for this screen to exit");
    refresh();
    Sleep(5000);

    URLDownloadToFileA(NULL, finalUrl_narrow, filePath, 0, NULL);
}

FlightInfo* parse_json(const char *filename, int* arraySize) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) return NULL;

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *data = (char*)malloc(length + 1);
    fread(data, 1, length, file);
    fclose(file);
    data[length] = '\0';

    cJSON *json = cJSON_Parse(data);
    if (json == NULL) {
        free(data);
        return NULL;
    }
    cJSON *response = cJSON_GetObjectItem(json, "response");
    if (response == NULL) {
        cJSON_Delete(json);
        free(data);
        return NULL;
    }
    free(data);

    *arraySize = cJSON_GetArraySize(response);
    int numFlights = (int)(*arraySize) > MAX_FLIGHTS_TO_STORE ? MAX_FLIGHTS_TO_STORE : (int)(*arraySize);
    FlightInfo *flights = (FlightInfo*)malloc(numFlights * sizeof(FlightInfo));

    for (int i = 0; i < numFlights; i++) {
        cJSON *item = cJSON_GetArrayItem(response, i);

        cJSON *flight_iata = cJSON_GetObjectItem(item, "flight_iata");
        cJSON *dep_iata = cJSON_GetObjectItem(item, "dep_iata");
        cJSON *arr_estimated_utc = cJSON_GetObjectItem(item, "arr_estimated_utc");

        if (flight_iata != NULL && dep_iata != NULL && arr_estimated_utc != NULL) {
            char arrivalTimeInUtc[20];
            for(int j = 0; j < 5; j++) {
                arrivalTimeInUtc[j] = arr_estimated_utc->valuestring[j + 11];
            }
            arrivalTimeInUtc[5] = '\0';
            strcat(arrivalTimeInUtc, " UTC");

            strncpy(flights[i].arr_estimated_utc, arrivalTimeInUtc, sizeof(flights[i].arr_estimated_utc));
            strncpy(flights[i].dep_iata, dep_iata->valuestring, sizeof(flights[i].dep_iata));
            strncpy(flights[i].flight_iata, flight_iata->valuestring, sizeof(flights[i].flight_iata));
        }
    }
    cJSON_Delete(json);
    return flights;
}

char* getCurrentTimeInUtc() {
    time_t rawtime;
    struct tm *timeinfo;
    
    // Get current time
    time(&rawtime);

    // Convert to UTC
    timeinfo = gmtime(&rawtime);

    // Allocate memory for the return string
    // Format "HH:MM UTC" needs 10 characters including the null terminator
    char* time_string = malloc(10);
    if (time_string == NULL) {
        return NULL; // Memory allocation failed
    }

    // Format the time as "HH:MM UTC"
    strftime(time_string, 10, "%H:%M UTC", timeinfo);

    return time_string;
}

void startNCurses()
{
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
}

int getStartingIndex(FlightInfo *allFlights, char* currentTime)
{
    // TEST CODE START
    clear();
    // TEST CODE END

    for(int i = 0; true; i++) {
        if(strcmp(allFlights[i].arr_estimated_utc, currentTime) == 0) {
            return i;
        }
        // TEST CODE START
        else {
            if(i % 10 == 0) {
                clear();
            }
            printw("%s =/= %s\n", allFlights[i].arr_estimated_utc, currentTime);
            refresh();
            Sleep(100);
        }
        // TEST CODE END
    }
    return -1;
}

void executeEachNewMinute(FlightInfo *allFlights, char* iataAirportCode, int* currentIndex) {
    clear();
    char* currentTime = malloc(10);
    strcpy(currentTime, allFlights[*currentIndex].arr_estimated_utc);

    mvprintw(1, 10, "All flights that are landing right now in %s", iataAirportCode);
    mvprintw(2, 10, "                                            ");
    refresh();
    Sleep(1000);

    mvprintw(3, 10, "           Current time = %s                ", currentTime);
    refresh();
    Sleep(2000);

    for(int i = *currentIndex; true; i++) {
        if(strcmp(currentTime, allFlights[i].arr_estimated_utc) != 0) {
            refresh();
            return;
        }
        else {
            mvprintw(14, 20, "%s is arriving from %s", allFlights[i].flight_iata, allFlights[i].dep_iata);
            print_runwayAnimation();
            Sleep(1000);
            (*currentIndex)++;
        }
    }
}

void print_messageBeforeLoopingAgain()
{
    clear();

    printw("The current json file is now empty.\n");
    printw("\n");
    printw("\n");
    printw("\n");
    printw("Please wait 5 seconds for this screen to exit");
    refresh();
    Sleep(5000);

    printw("The program will now make another API request to download the most-recent arrival data.\n");
    printw("\n");
    printw("\n");
    printw("\n");
    printw("Please wait 5 seconds for this screen to exit");
    refresh();
    Sleep(5000);
}