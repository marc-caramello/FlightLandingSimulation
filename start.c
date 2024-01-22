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

    // TEMP CODE START
    enterAirportCode_and_downloadJsonFile();
    // TEMP CODE END
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
    const char url_part1[] = "https://airlabs.co/api/v9/schedules?arr_iata=";
    const char url_part2[] = "&api_key=7fd7db1a-b297-413f-b998-817aa63226d7";
    int finalUrl_size = strlen(url_part1) + strlen(url_part2) + strlen(finalInput) + 1;
    char finalUrl[finalUrl_size];
    strcpy(finalUrl, url_part1);
    strcat(finalUrl, finalInput);
    strcat(finalUrl, url_part2);
}

/*
void downloadJsonFile()
{
    DWORD length = GetCurrentDirectoryW(0, NULL);
    wchar_t* pathToCurrentFolder = malloc(length * sizeof(wchar_t));
    DWORD result = GetCurrentDirectoryW(length, pathToCurrentFolder);

    const wchar_t* url = L"https://airlabs.co/api/v9/schedules?arr_iata=ATL&api_key=7fd7db1a-b297-413f-b998-817aa63226d7";
    URLDownloadToFile(NULL, url, pathToCurrentFolder, 0, NULL);
}
*/