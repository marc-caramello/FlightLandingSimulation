#include "animation.h"
#include "start.h"

int main() {
    /*print_thankYouMessage();
    enterAirportCode_and_downloadJsonFile();*/
    int arraySize;
    FlightInfo *allFlights = parse_json("C:\\Users\\marcc\\OneDrive\\Desktop\\CodingProjects\\Cursor\\FlightLandingSimulation\\temp.json", &arraySize);
    startNCurses();

    char currentTime[30];
    strcpy(currentTime, allFlights[0].arr_estimated_utc);
    int currentIndex = 0;
    do {
        executeEachNewMinute(allFlights, currentTime, &currentIndex);
    } while(currentIndex < arraySize);

    //print_runwayAnimation();
    
    return 0;
}

// --------------------------------------------------

void print_thankYouMessage()
{
    system("color 0b");
    
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

void enterAirportCode_and_downloadJsonFile()
{
    char finalInput[4];
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
            strcpy(finalInput, userInput);
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
    char currentDirectory[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, currentDirectory);

    printf("Connecting to external API\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("Please wait 5 seconds for this screen to exit");
    Sleep(5000);
    system("cls");

    const char url_part1[] = "https://airlabs.co/api/v9/schedules?arr_iata=";
    const char url_part2[] = "&api_key=7fd7db1a-b297-413f-b998-817aa63226d7";
    int finalUrl_size = strlen(url_part1) + strlen(finalInput) + strlen(url_part2) + 1;
    char finalUrl_narrow[finalUrl_size];
    strcpy(finalUrl_narrow, url_part1);
    strcat(finalUrl_narrow, finalInput);
    strcat(finalUrl_narrow, url_part2);

    // Specify the path where you want to save the downloaded file
    char filePath[MAX_PATH];
    snprintf(filePath, MAX_PATH, "%s\\temp.json", currentDirectory);

    printf("Downloading json file from external API\n");
    printf("\n");
    printf("\n");
    printf("\n");
    printf("Please wait 5 seconds for this screen to exit");
    Sleep(5000);
    system("cls");

    // Use URLDownloadToFile to download the file
    //URLDownloadToFileA(NULL, finalUrl_narrow, filePath, 0, NULL);
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

            strncpy(flights[i].flight_iata, flight_iata->valuestring, sizeof(flights[i].flight_iata));
            strncpy(flights[i].dep_iata, dep_iata->valuestring, sizeof(flights[i].dep_iata));
            strncpy(flights[i].arr_estimated_utc, arrivalTimeInUtc, sizeof(flights[i].arr_estimated_utc));
        }
    }
    cJSON_Delete(json);
    return flights;
}

void startNCurses()
{
    initscr();                  // Initialize NCurses
    cbreak();                   // Disable line buffering
    noecho();                   // Disable echoing of characters
    keypad(stdscr, TRUE);       // Enable special key capture
}

void executeEachNewMinute(FlightInfo *allFlights, char currentTime[], int* currentIndex) {
    clear();
    strcpy(currentTime, allFlights[*currentIndex].arr_estimated_utc);

    mvprintw(1, 10, "All flights that are landing right now in %s", "ATL");
    mvprintw(2, 10, "                                            ");
    mvprintw(3, 10, "           Current time = %s                ", currentTime);
    refresh();
    Sleep(3000);

    for(int i = *currentIndex; true; i++) {
        if(strcmp(currentTime, allFlights[i].arr_estimated_utc) != 0) {
            return;
        }
        else {
            mvprintw(14, 20, "%s is arriving from %s", allFlights[i].flight_iata, allFlights[i].dep_iata);
            print_runwayAnimation();
            (*currentIndex)++;
        }
    }
}