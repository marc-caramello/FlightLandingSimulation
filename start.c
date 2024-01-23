#include "start.h"

int main() {
    enterAirportCode_and_downloadJsonFile();
    FlightInfo *allFlights = parse_json("C:\\Users\\marcc\\OneDrive\\Desktop\\CodingProjects\\Cursor\\FlightLandingSimulation\\temp.json");
    startNCurses();
    
    

    //print_runwayAnimation();
    
    return 0;
}

// --------------------------------------------------

void startNCurses()
{
    initscr();                  // Initialize NCurses
    cbreak();                   // Disable line buffering
    noecho();                   // Disable echoing of characters
    keypad(stdscr, TRUE);       // Enable special key capture
}

void enterAirportCode_and_downloadJsonFile()
{
    char finalInput[3];
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
            break;
        }
        else {
            printf("\"%s\" is not an acceptable IATA airport code\n", userInput);
            printf("\n");
            printf("Please wait 3 seconds for this screen to exit");
            Sleep(3000);
            system("cls");
        }
    } while(true);
    /*char currentDirectory[MAX_PATH];
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
    }*/
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

        char arrivalTimeInUtc[20];
        int nextIndex = 0;
        for(int i = 0; i < 5; i++) {
            arrivalTimeInUtc[i] = arrEstimatedUtc->valuestring[i + 11];
        }
        strcat(arrivalTimeInUtc, " UTC");

        if (flightIata != NULL && arrEstimatedUtc != NULL) {
            strncpy(flights[i].flight_iata, flightIata->valuestring, sizeof(flights[i].flight_iata));
            strncpy(flights[i].arr_estimated_utc, arrivalTimeInUtc, sizeof(flights[i].arr_estimated_utc));
        }
    }
    cJSON_Delete(json);
    free(data);
    return flights;
}