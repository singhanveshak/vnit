/*This project is part of DSPD assignemnt 1, 2023.
Submitted by: Anveshak Singh (BT22CSE081), Gaurang Jadhav (BT22CSE099): VNIT, nagpur
Database created using array of structs.*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
const int n_games = 3, n_records = 6;

struct ground
{
    char name[30];
    char city[15];
    char state[15];
    struct games
    {
        char game_name[20];  // name of game
        int available[5][4]; // list of {dates, month, year, time} spots available for the game
        float rate;
        int filled[5][4]; // list of {dates, month, year, time} spots filled for the game
    } game[3];
};

void swap(int arr[30][3], int i, int j)
{
    int temp[3];
    for (int k = 0; k < 3; k++)
    {
        temp[k] = arr[i][k];
        arr[i][k] = arr[j][k];
        arr[j][k] = temp[k];
    }
}

void sortDates(int dates[30][3])
{
    for (int i = 0; i < 30 - 1; i++)
    {
        for (int j = 0; j < 30 - i - 1; j++)
        {
            if (dates[j][2] > dates[j + 1][2])
            {
                swap(dates, j, j + 1);
            }
            else if (dates[j][2] == dates[j + 1][2] && dates[j][1] > dates[j + 1][1])
            {
                swap(dates, j, j + 1);
            }
            else if (dates[j][2] == dates[j + 1][2] && dates[j][1] == dates[j + 1][1] && dates[j][0] > dates[j + 1][0])
            {
                swap(dates, j, j + 1);
            }
        }
    }
}

int partition(int arr[], int low, int high) {  
    int pivot = arr[high];  
    int i = (low - 1);  
    int t;
    for (int j = low; j <= high - 1; j++) {  
        if (arr[j] < pivot) {  
            i++;  
            t=arr[i];
            arr[i]=arr[j];
            arr[j]=t;  
        }  
    }  
    t=arr[i+1];
    arr[i+1]=arr[high];
    arr[high]=t;  
    return (i + 1);  
}  

void quickSort(int arr[], int low, int high) {  
    if (low < high) {  
        int pi = partition(arr, low, high);  
        quickSort(arr, low, pi - 1);  
        quickSort(arr, pi + 1, high);  
    }  
}

void printDates(int dates[30][3])
{
    for (int i = 0; i < 30; i++)
    {
        printf("%02d-%02d-%d\n", dates[i][0], dates[i][1], dates[i][2]);
    }
}

int compareDates(int d1, int m1, int y1, int d2, int m2, int y2, int d, int m, int y)
{
    if (y > y1 && y < y2)
    {
        return 1;
    }
    else if (y == y1 && y == y2)
    {
        if (m > m1 && m < m2)
        {
            return 1;
        }
        else if (m == m1 && m == m2)
        {
            if (d >= d1 && d <= d2)
            {
                return 1;
            }
        }
        else if (m == m1 && m < m2)
        {
            if (d >= d1)
            {
                return 1;
            }
        }
        else if (m > m1 && m == m2)
        {
            if (d <= d2)
            {
                return 1;
            }
        }
    }
    else if (y == y1 && y < y2)
    {
        if (m > m1)
        {
            return 1;
        }
        else if (m == m1)
        {
            if (d >= d1)
            {
                return 1;
            }
        }
    }
    else if (y > y1 && y == y2)
    {
        if (m < m2)
        {
            return 1;
        }
        else if (m == m2)
        {
            if (d <= d2)
            {
                return 1;
            }
        }
    }
    return 0;
}

int is_city_valid(char c[],char valid_cities[][20])
{
    for(int i=0;i<5;i++)
    {
        if(strcmp(c,valid_cities[i])==0)
            return 1;
    }
    return 0; 
}

int is_date_valid(int d, int m, int y)
{
    if(d<1 || d>31 || m<1 || m>12 || y<2000 || y>2025)
        return 0;
    return 1;
}

int is_game_valid(char c[],char valid_games[][20])
{
    for(int i=0;i<5;i++)
    {
        if(strcmp(c,valid_games[i])==0)
            return 1;
    }
    return 0; 
}
//-------------------------------------------------------------------------------------------------------------------------------

// TODO apply sorting
void getFixturesinCity(char city[], int d1, int m1, int y1, int d2, int m2, int y2, struct ground arr[])
{ /*
 FOR EACH GROUND IN CITY-> FOR EACH GAME IN THAT GROUND-> CHECK IF IT LIES IN DATE RANGE
 */
    struct ground g;
    int i = 0, j = 0, k = 0, date = 0, month = 0, year = 0, time = 0;
    printf("getFixturesinCity()....\n");
    printf("Games happening in %s between dates %d/%d/%d and %d/%d/%d are:\n", city, d1, m1, y1, d2, m2, y2);
    for (i = 0; i < n_records; i++)
    {
        g = arr[i];
        if (strcmp(g.city, city) == 0)
        { // for every ground in city
            printf("\n\t%s:\n", g.name);
            for (j = 0; j < n_games; j++)
            { // for every game in ground
                if (g.game[j].filled[0][0] != 0)
                { // for every game in ground
                    printf("\t\t%s:\n\t\t\tdates: ", g.game[j].game_name);
                    k = 0;
                    while (g.game[j].filled[k][0] != '\0')
                    { // for every happening date of game-> print it
                        date = g.game[j].filled[k][0];
                        month = g.game[j].filled[k][1];
                        year = g.game[j].filled[k][2], time = g.game[j].filled[k][3];
                        if (compareDates(d1, m1, y1, d2, m2, y2, date, month, year))
                            printf("%d/%d/%d at %d:00 Hours,  ", date, month, year, time);
                        k++;
                    }
                    // print dates for that game
                    printf("\n");
                }
            }
        }
    }
    printf("\n--------------------------------------------------------------------------------------------------\n");
}

// TODO apply sorting
void isAvailable(char game_name[], int d, int m, int y, int t, struct ground arr[])
{ /*
 FOR EACH GROUND -> FOR EACH GAME IN THAT GROUND-> FOR EVERY DATE IN ITS FILLED LIST->CHECK IF IT HOLDS THE GAME ON DATE, TIME
 */
    struct ground g;
    int i = 0, j = 0, k = 0, date = 0, time = 0, month = 0, year = 0;
    printf("isAvailable()....\n");
    printf("%s is available at: \n", game_name);
    for (i = 0; i < n_records; i++)
    { // for every ground
        g = arr[i];
        for (j = 0; j < n_games; j++)
        { // for every game in ground
            if (strcmp(game_name, g.game[j].game_name) == 0)
            { // if game == game_name
                k = 0;
                while (g.game[j].available[k][0] != '\0')
                { // for every happening date of game-> check availability
                    date = g.game[j].available[k][0];
                    month = g.game[j].available[k][1];
                    year = g.game[j].available[k][2];
                    time = g.game[j].available[k][3];
                    if (date == d && month == m && year == y && abs(t - time) <= 5)
                        printf("\t%s in %s on %d/%d/%d at %d Hours\n", g.name, g.city, date, month, year, time);
                    k++;
                }
            }
        }
    }
    printf("\n--------------------------------------------------------------------------------------------------\n");
}

void UniqueSports(char ground_name[], char city_name[], struct ground arr[])
{
    struct ground g, g1;
    int i = 0, j = 0, k = 0, flag = 0, isLonely = 1;
    printf("UniqueSports()....\n");
    for (i = 0; i < n_records && flag != 1; i++)
    { // search the given ground
        g = arr[i];
        if (strcmp(g.name, ground_name) == 0)
        { // ground found
            flag = 1;
            for (j = 0; j < n_records; j++)
            { // then for every other ground in city_name
                g1 = arr[j];
                if (g.name != g1.name && g1.city == city_name)
                { // then for every other ground in city
                    isLonely = 0;
                    for (k = 0; k < n_games; k++)
                    {
                        if (g.game[k].game_name != NULL && g.game[k].game_name != g1.game[0].game_name && g.game[k].game_name != g1.game[1].game_name && g.game[k].game_name != g1.game[2].game_name)
                        { // if game is unique-> print it
                            printf("\n\t%s is unique in %s", g.game[0].game_name, ground_name);
                        }
                    }
                }
            }
            if (isLonely == 1)
                printf("\n\t%s, %s, %s is unique in %s", g.game[0].game_name, g.game[1].game_name, g.game[2].game_name, ground_name);
        }
    }
    printf("\n--------------------------------------------------------------------------------------------------\n");
}

void FindLocationsForGameInTimeRange(char game_name[], int d1, int m1, int y1, int d2, int m2, int y2, int t1, int t2, struct ground arr[])
{
    struct ground g;
    int i = 0, j = 0, k = 0, date = 0, month = 0, year = 0, time = 0;
    printf("FindLocationsForGameInTimeRange()....\n");
    printf("%s (in the given time range) is available at: \n", game_name);
    for (i = 0; i < n_records; i++)
    { // for each ground
        g = arr[i];
        for (j = 0; j < n_games; j++)
        { // for each sport in ground, check if holds game_name
            if (strcmp(g.game[j].game_name, game_name))
            { // if it does, go through each avaialble spots (date,time) and check if it lies in given date,time range; if it does, print it
                k = 0;
                int count = 0;
                while (g.game[j].available[k][0] != '\0')
                {
                    count++;
                    date = g.game[j].available[k][0];
                    month = g.game[j].available[k][1];
                    year = g.game[j].available[k][2];
                    time = g.game[j].available[k][3];
                    if (compareDates(d1, m1, y1, d2, m2, y2, date, month, year) && time <= t2 && time >= t1) // change it
                        printf("\n\t%s in %s, %s on %d/%d/%d at %d:00 Hours", g.name, g.city, g.state, date, month, year, time);
                    k++;
                }
            }
        }
    }
    printf("\n--------------------------------------------------------------------------------------------------\n");
}

// TODO find clash with any game on ground not just with the passed game
void ClashOfMatches(int n_ground_list, struct ground arr[], int date, int month, int year, char game_name[])
{
    struct ground g;
    int i = 0, j = 0, k = 0, g_date = 0;
    printf("ClashOfMatches()....\n");
    for (i = 0; i < n_ground_list; i++)
    { // for each ground in ground_list
        g = arr[i];
        j = 0;
        while (g.game[j].available[0][0] != 0)
        { // go through every game
            k = 0;
            while (g.game[j].filled[k][0] != 0)
            {                                                                                                            // go through game's dates
                if (g.game[j].filled[k][0] == date && g.game[j].filled[k][1] == month && g.game[j].filled[k][2] == year) // if any date of game clashes print it
                    printf("\n\t%s clashes with %s on date %d/%d/%d at %s at %d:00 Hours", game_name, g.game[j].game_name, date, month, year, g.name, g.game[j].filled[k][3]);
                k++;
            }
            j++;
        }
    }
    printf("\n--------------------------------------------------------------------------------------------------\n");
}

void GiveListOfGroundsHavingAtLeastKMatches(char game_list[][20], int date, int month, char city[], int K, struct ground arr[])
{
    // FOR EACH GROUND IN CITY -> GO THROUGH ITS GAMES -> GO THROUGH ITS FILLED DATES LIST -> IF THERE ARE MORE K MATCHES FOR THAT GAME ON THAT DATE, PRINT THE GROUND
    struct ground g;
    int i = 0, j = 0, k = 0;
    printf("GiveListOfGroundsHavingAtLeastKMatches()....\n");
    printf("grounds having atleast %d matches of one of the provided games on date %d/%d are: \n", K, date, month);
    for (i = 0; i < n_records; i++)
    { // for each ground
        g = arr[i];
        if (strcmp(g.city, city) == 0)
        { // for each ground in city
            j = 0;
            while (g.game[j].available[0][0] != 0)
            { // for each of its games
                k = 0;
                int count = 0;
                while (g.game[j].filled[k][0] != '\0' && count < K)
                { // for each of the filled date list
                    if (g.game[j].filled[k][0] == date && g.game[j].filled[k][1] == month)
                        count++;
                    if (count >= K)
                        printf("\n\t%s (for %s)", g.name, g.game[j].game_name);
                    k++;
                }
                j++;
            }
        }
    }
    printf("\n--------------------------------------------------------------------------------------------------\n");
}

void getFixturesSortedonDate(struct ground arr[])
{ // FIRST COLLECT ALL DATES (SORT THEM)-> THEN FOR EACH DATE (IN SORTED DATE ARRAY) -> GO THROUGH ALL GROUNDS->GAMES->ALL AVAILABLE DATES-> IF THAT DATE MATCHES->PRINT IT IMMEDIATELY
    struct ground g;
    int i = 0, j = 0, k = 0, c = 0, x = 0, flag = 0, dates[30][3];
    // collecting all dates
    for (i = 0; i < n_records; i++)
    { // for all grounds
        for (j = 0; j < n_games; j++)
        { // for all games in grounds
            k = 0;
            while (arr[i].game[j].available[k][0] != 0)
            { // printf("\ni=%d j=%d k=%d",i,j,k);
                // if date is not already in date array
                flag = 0;   //if flag=0 means the date is not found in the array and is unique, we have to store it 
                for (x = 0; x < 30; x++)
                {
                    if (dates[x][0] == arr[i].game[j].available[k][0] && dates[x][1] == arr[i].game[j].available[k][1] && dates[x][2] == arr[i].game[j].available[k][2])
                    {
                        flag = 1;
                    }
                }
                if (flag == 0)
                {
                    dates[c][0] = arr[i].game[j].available[k][0];
                    dates[c][1] = arr[i].game[j].available[k][1];
                    dates[c][2] = arr[i].game[j].available[k][2];
                    c++;
                }
                k++;
            }
        }
    }
//delete all garabage values
    for(int x=c; x<30;x++)
    {
         dates[x][0] = 0;
         dates[x][1] = 0;
        dates[x][2] = 0;
    }
    sortDates(dates);
    printf("sortDates()....\n");
    // for each date, go through grounds, games, dates, if the date is in the array print it immediately
    x = 0;
    while (x<30)
    {
        if(dates[x][0]!=0 && dates[x][1]!=0 && dates[x][2]!=0)
            printf("\n\t%d/%d/%d",dates[x][0],dates[x][1],dates[x][2]);
        for (i = 0; i < n_games; i++)
        { // for every ground
            g = arr[i];
            for (j = 0; j < n_games; j++)
            { // for every game
                k = 0;
                
                while (g.game[j].available[k][0] != 0)
                {   
                    if (g.game[j].available[k][0] == dates[x][0] && g.game[j].available[k][1] == dates[x][1] && g.game[j].available[k][2] == dates[x][2])
                    {
                        printf("\n\t\t%s, %s at %d:00 Hours", g.name, g.game[j].game_name,g.game[j].available[k][3]);
                    }
                    k++;
                }
            }
        }
    x++;
    }
    printf("\n--------------------------------------------------------------------------------------------------\n");
}

void SortOnPriceForAGame(char game_name[], int d, int m, int y, int t, struct ground arr[])
{
    struct ground g;
    int i=0,j = 0, k = 0, date = 0;int rates[10];
    printf("SortOnPriceForAGame()....\n");
    for(i=0 ;i < n_records ; i++)
    {g=arr[i];      
    // printf("%s has these prices for this game: \n",g.name);
        for(j=0 ; j < n_games; j++)
        {
            if (strcmp(g.game[j].game_name,game_name)==0)
            {   
                k=0;
                while(g.game[j].available[k][0] !=0)
                {   
                    if(d==g.game[j].available[k][0] && m==g.game[j].available[k][1] && y==g.game[j].available[k][2])
                    {rates[k]=g.game[j].rate;}
                    k++;
                }
            }
        }  
    }
    quickSort(rates,0,k-1);
    for(int x=0; x<k;x++){printf("\n%d",rates[x]);}
}

void main()
{
    int choice = 1, flag=0, d1, m1, y1, d2, m2, y2, t1, t2;   char city[20], game_name[20], ground_name[30], game_list[3][20], valid_cities[5][20]={"Ahemadabad","Hyderabad","Kanpur","Kolkata"}, valid_games[3][20]={"volleyball", "cricket", "football"};
    struct ground SPS = {.name = "Sardar Patel Stadium", .city = "Ahemadabad", .state = "Gujarat", .game = {{.game_name = "cricket", .rate = 5070.5, .available = {{19, 2, 2024, 5}, {2, 3, 2024, 13}, {9, 12, 2023, 15}}, .filled = {{4, 11, 2023, 14}, {5, 3, 2024, 16}}}}};
    struct ground NMS = {.name = "Narendra Modi Stadium", .city = "Ahemadabad", .state = "Gujarat", .game = {{.game_name = "football", .rate = 4040.5, .available = {{1, 6, 2024, 4}, {2, 11, 2023, 4}}, .filled = {{4, 11, 2023, 14}}}}};
    struct ground RGS = {.name = "Rajiv Gandhi Stadium", .city = "Hyderabad", .state = "Telangana", .game = {{.game_name = "volleyball", .rate = 1100.5, .available = {{9, 12, 2023, 15}, {1, 12, 2023, 19}}, .filled = {{8, 11, 2023, 14}}}, {.game_name = "cricket", .rate = 10040.5, .available = {{10, 10, 2024, 16}, {20, 11, 2023, 4}}, .filled = {{3, 1, 2024, 10}, {3, 1, 2024}, {10, 4, 2024, 19}, {10, 4, 2024, 7}}}}};
    struct ground EG = {.name = "Eden Gardens", .city = "Kolkata", .state = "West Bengal", .game = {{.game_name = "football", .rate = 7000.0, .available = {{1, 2, 2024, 17}, {2, 2, 2024, 18}}, .filled = {{20, 2, 2024, 16}, {10, 4, 2024, 19}, {10, 4, 2024, 7}}}}};
    struct ground MK = {.name = "Maidan Kolkata", .city = "Kolkata", .state = "West Bengal", .game = {{.game_name = "cricket", .rate = 4040.5, .available = {{19, 2, 2024, 4}, {12, 11, 2023, 18}}, .filled = {{13, 3, 2024, 20}, {10, 4, 2023, 11}, {20, 3, 2024, 11}}}}};
    struct ground GPS = {.name = "Green Park Stadium", .city = "Kanpur", .state = "Uttar Pradesh", .game = {{.game_name = "cricket", .rate = 4040.5, .available = {{19, 2, 2024, 4}, {12, 4, 2024, 18}}, .filled = {{13, 12, 2023, 20}, {13, 12, 2023, 11}}}, {.game_name = "football", .rate = 4040.5, .available = {{1, 3, 2024, 4}, {2, 4, 2024, 13}}, .filled = {{4, 5, 2024, 14}, {13, 12, 2023, 16}}}, {.game_name = "volleyball", .rate = 1100.5, .available = {{9, 3, 2024, 15}, {1, 12, 2023, 19}}, .filled = {{8, 12, 2023, 14}, {8, 12, 2023, 19}}}}};
    struct ground arr[6] = {SPS, NMS, RGS, EG, MK, GPS};

    
    while(choice!=0)
    {
        
        printf("\nPlease select your seach query: PRESS [0] TO  EXIT\n\n");
        printf("getFixturesinCity (1)\nisAvailable (2)\nFindLocationsForGameInTimeRange (3)\nUniqueSports (4)\nClashOfMatches (5)\nGiveListOfGroundsHavingAtLeastKMatches (6)\ngetFixturesSortedonDate (7)\nSortOnPriceForAGame (8)");
        scanf("%d",&choice);
        switch(choice)
        {
            case 1:
                city[0]='\0'; d1 =1; m1=1; y1=2023; d2=30; m2=4; y2=2024;
                printf("\n\tEnter city name (Kolkata)");   scanf("%s",city);    printf("\tEnter end date (10/4/2023) "); scanf("%d/%d/%d",&d1,&m1,&y1);    printf("\tEnter start date (10/4/2023) "); scanf("%d/%d/%d",&d2,&m2,&y2);
                if(is_city_valid(city,valid_cities) && is_date_valid(d1, m1, y1) && is_date_valid(d2, m2, y2))
                    {   printf("\n--------------------------------------------------------------------------------------------------\n");
                        getFixturesinCity(city,d1,m2,y1,d2,m2,y2,arr);}
                else
                    printf("\n\t\tWARNING: ONE OR MORE INVALID INPUTS!");
                break;
            case 2:
                game_name[0]='\0'; d1 =1; m1=1;y1=2023; t1=14;
                printf("\n\tEnter game name (cricket) ");   scanf("%s",game_name);    printf("\tEnter date (19/2/2024) "); scanf("%d/%d/%d",&d1,&m1,&y1); printf("\tEnter time 24h format(5:00) "); scanf("%d:00",&t1);    
                if(is_game_valid(game_name,valid_games) && is_date_valid(d1, m1, y1))
                    {   printf("\n--------------------------------------------------------------------------------------------------\n");
                        isAvailable(game_name,d1,m1,y1,t1,arr);}
                else
                    printf("\n\t\tWARNING: ONE OR MORE INVALID INPUTS!");
                break;
            case 3:
                game_name[0]='\0'; d1 =1; m1=1; y1=2023; d2=30; m2=4; y2=2024; t1=14; t2=20;
                printf("\n\tEnter game_name (cricket) ");   scanf("%s",city);    printf("\tEnter end date (10/4/2023) "); scanf("%d/%d/%d",&d1,&m1,&y1);  printf("\tEnter end time 24h format(5:00) "); scanf("%d:00",&t1);  printf("\tEnter start date (10/4/2023) "); scanf("%d/%d/%d",&d2,&m2,&y2); printf("\tEnter start time 24h format(16:00) "); scanf("%d:00",&t1);  
                if(is_game_valid(game_name,valid_games) && is_date_valid(d1, m1, y1) && is_date_valid(d2, m2, y2))
                    {   printf("\n--------------------------------------------------------------------------------------------------\n");
                        FindLocationsForGameInTimeRange(game_name,d1,m2,y1,d2,m2,y2,t1,t2,arr);}
                else
                    printf("\n\t\tWARNING: ONE OR MORE INVALID INPUTS!");
                break;
            case 4:
                ground_name[0]='\0'; city[0]='\0';
                printf("\n\tEnter ground name ");   scanf("%s",ground_name);    printf("\n\tEnter city name (Kanpur) ");   scanf("%s",city);   
                if(is_city_valid(city,valid_cities))
                    {printf("\n--------------------------------------------------------------------------------------------------\n");
                        UniqueSports(ground_name,city,arr);}
                else
                    printf("\n\t\tWARNING: ONE OR MORE INVALID INPUTS!");
                break;
            case 5:
                game_name[0]='\0'; d1 =1; m1=1; y1=2023;
                printf("\n\tEnter game name (cricket) ");   scanf("%s",game_name);    printf("\tEnter date (13/12/2023, 10/4/2024) "); scanf("%d/%d/%d",&d1,&m1,&y1);   
                if(is_game_valid(game_name,valid_games) && is_date_valid(d1, m1, y1))
                    {   printf("\n--------------------------------------------------------------------------------------------------\n");
                        ClashOfMatches(n_records,arr,d1,m1,y1,game_name);}
                else
                    printf("\n\t\tWARNING: ONE OR MORE INVALID INPUTS!");
                break;
            case 6:
                city[0]='\0'; d1 =1; m1=1; y1=2023;
                printf("\n\tEnter city name (Kolkata) ");   scanf("%s",city);    printf("\tEnter date (10/4/2024) "); scanf("%d/%d/%d",&d1,&m1,&y1); printf("\tEnter K ");  scanf("%d",&m2); printf("\thow many games do you want to enter? "); scanf("%d",&y2);
                for(int i=0;i<y2;i++){printf("\t\tEnter sport name: ");scanf("%s",game_list[i]);}
                if(is_city_valid(city,valid_cities)  && is_date_valid(d1, m1, y1))
                    {printf("\n--------------------------------------------------------------------------------------------------\n");
                        GiveListOfGroundsHavingAtLeastKMatches(game_list,d1,m1,city,m2,arr);}
                else
                    printf("\n\t\tWARNING: ONE OR MORE INVALID INPUTS!");
                break;
            case 7:
                printf("\n--------------------------------------------------------------------------------------------------\n");
                getFixturesSortedonDate(arr);
                break;
            case 8:
                game_name[0]='\0'; d1 =1; m1=1; y1=2023; t1=6;
                printf("\n\tEnter game name (cricket) ");   scanf("%s",game_name);    printf("\tEnter date (13/12/2023, 10/4/2024) "); scanf("%d/%d/%d",&d1,&m1,&y1); printf("\n\tEnter time(13:00) ");   scanf("%d:00",&t1);   
                if(is_game_valid(game_name,valid_games) && is_date_valid(d1, m1, y1))
                    {printf("\n--------------------------------------------------------------------------------------------------\n");
                        SortOnPriceForAGame(game_name,d1,m1,y1,t1,arr);}
                else
                    printf("\n\t\tWARNING: ONE OR MORE INVALID INPUTS!");
                break;
            case 0:
                printf("\nExiting......\n");
                break;
            default:
                printf("\n\t\t  WRONG CHOICE!");
                break;
        }
    }
}

    //  UniqueSports("Green Park Stadium", "Kanpur", arr);
    // getFixturesinCity("Ahemadabad", 1, 10, 2023, 30, 4, 2024, arr);
    //  FindLocationsForGameInTimeRange("cricket",1,2,2023,30,2,2024,5,18,arr);
    // printf("%d",arr[0].game[1].filled[0][0]);
    //  ClashOfMatches(6,arr,10,4,2024,"football");

    // char c[15]="cricket";
    //  isAvailable(c,19,2,2024,5,arr);
    // char garr[3][15]={"cricket","football","volleyball"};
    //  GiveListOfGroundsHavingAtLeastKMatches(garr,10,4,"Kolkata",1,arr);
    // getFixturesSortedonDate(arr);
