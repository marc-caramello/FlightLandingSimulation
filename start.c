#include "start.h"

int main() {
    /*initscr();
    curs_set(0);
    noecho();
    cbreak();
    keypad(stdscr, true);
    
    print_runwayAnimation();
    getch();
    clear();

    endwin();*/
    //enterAirportCode_and_downloadJsonFile();
    FlightInfo *allFlights = parse_json("C:\\Users\\marcc\\OneDrive\\Desktop\\CodingProjects\\Cursor\\FlightLandingSimulation\\temp.json");
    
    for(int i = 0; i < MAX_FLIGHTS_TO_STORE; i++) {
        printf("%s %s\n", allFlights[i].flight_iata, allFlights[i].arr_estimated_utc);
    }
    return 0;
}

// --------------------------------------------------

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

// --------------------------------------------------

void enterAirportCode_and_downloadJsonFile()
{
    char finalInput[3];
    do {
        printf("Please enter the IATA airport code: ");
        char userInput[100];  // Increase the size to accommodate 3 characters and the null terminator
        fgets(userInput, sizeof(userInput), stdin);  // Limit the number of characters read
        userInput[strcspn(userInput, "\n")] = 0; 

        bool allLettersAreAlpha = true;

        for(int i = 0; i < strlen(userInput); i++) {
            userInput[i] = toupper(userInput[i]);

            if(!isalpha(userInput[i])) {
                allLettersAreAlpha = false;
            }
        }
        if(strlen(userInput) == 3 && allLettersAreAlpha) {
            strcpy(finalInput, userInput);
            break;
        }
        else {
            printf("\"%s\" is not an acceptable IATA airport code\n", userInput);
            printf("\n");
        }
    } while(true);
    char currentDirectory[MAX_PATH];
    if (GetCurrentDirectory(MAX_PATH, currentDirectory) != 0) {
        const char url_part1[] = "https://airlabs.co/api/v9/schedules?arr_iata=";
        const char url_part2[] = "&api_key=7fd7db1a-b297-413f-b998-817aa63226d7";
        int finalUrl_size = strlen(url_part1) + strlen(url_part2) + strlen(finalInput) + 1;
        char finalUrl_narrow[finalUrl_size];
        strcpy(finalUrl_narrow, url_part1);
        strcat(finalUrl_narrow, finalInput);
        strcat(finalUrl_narrow, url_part2);

        // Specify the path where you want to save the downloaded file
        char filePath[MAX_PATH];
        snprintf(filePath, MAX_PATH, "%s\\temp.json", currentDirectory);

        // Use URLDownloadToFile to download the file
        HRESULT result = URLDownloadToFileA(NULL, finalUrl_narrow, filePath, 0, NULL);

        if (result == S_OK) {
            printf("File downloaded successfully.\n");
        }
        else {
            printf("Failed to download the file. Error code: 0x%08x\n", result);
        }
    }
    else {
        printf("Failed to get the current directory.\n");
    }
}

FlightInfo* parse_json(const char *filename) {
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

    // Get the "response" object from the parsed JSON
    cJSON *response = cJSON_GetObjectItem(json, "response");
    if (response == NULL) {
        cJSON_Delete(json);
        free(data);
        return NULL;
    }

    int arraySize = cJSON_GetArraySize(response);
    int numFlights = arraySize > MAX_FLIGHTS_TO_STORE ? MAX_FLIGHTS_TO_STORE : arraySize;

    FlightInfo *flights = (FlightInfo*)malloc(numFlights * sizeof(FlightInfo));

    for (int i = 0; i < numFlights; i++) {
        cJSON *item = cJSON_GetArrayItem(response, i);
        cJSON *flightIata = cJSON_GetObjectItem(item, "flight_iata");
        cJSON *arrEstimatedUtc = cJSON_GetObjectItem(item, "arr_estimated_utc");

        //printf("%s\n", flightIata->valuestring);

        if (flightIata != NULL && arrEstimatedUtc != NULL) {
            strncpy(flights[i].flight_iata, flightIata->valuestring, sizeof(flights[i].flight_iata));
            strncpy(flights[i].arr_estimated_utc, arrEstimatedUtc->valuestring, sizeof(flights[i].arr_estimated_utc));
        }
    }
    cJSON_Delete(json);
    free(data);
    return flights;
}