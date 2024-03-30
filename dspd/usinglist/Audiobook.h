/*ANVESHAK SINGH 13/3/2024*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 30
typedef enum{true,false} bool;
typedef unsigned short HASHTYPE;


typedef struct Audiobook
{
    HASHTYPE id;
    char title[SIZE];
    char author[SIZE];
    char narrator[SIZE];
    char genre[SIZE];
    short min, total_listening_time;
    short rating, num_ratings;
    struct Audiobook *next;
} Audiobook;

typedef struct user
{
    HASHTYPE uid;
    char name[SIZE];
    char mail[SIZE];
    struct mybook *mybooks;
    struct user *next;
} user;

typedef struct mybook
{
    HASHTYPE id;
    char title[SIZE];
    char author[SIZE];
    char narrator[SIZE];
    char genre[SIZE];
    short min;
    float percent;
    short uRating;
    struct mybook *next;
} mybook;


user *Add_user(char name[], char mail[]);
void Add_to_user_library(HASHTYPE id, HASHTYPE bookid);
Audiobook *Add_audiobook(short min,  char tit[], char aut[], char nar[], char gen[]);
void Delete_audiobook(char tit[], char aut[]);
void Search_audiobook();
void Listening_progress(HASHTYPE id, HASHTYPE bookid, short m);
void Display_user_lib(HASHTYPE id);
void Rate_Audiobook(HASHTYPE id, HASHTYPE bookid,short r);
void Most_poplular_audiobook();
void Listening_history_report_user(HASHTYPE id);
void Merge_user_libraries(HASHTYPE uid1, HASHTYPE uid2);
void Display_audiobook_rating(Audiobook *after_edit);
void Listening_history_report_audiobook(HASHTYPE bookid);

/*Helper functions*/
void DEBUG();
HASHTYPE LOG_USER(bool log);
void FILEREADER();
void FILEWRITER();
user *create_user_node(HASHTYPE id, char name[], char mail[]);
Audiobook *create_book_node(HASHTYPE id, short min, char title[], char author[], char narrator[], char genre[], short r, short n, short t);
mybook *create_mybook_node(HASHTYPE id, short min, char tit[], char aut[], char nar[], char gen[], float p, short r);
HASHTYPE HASH(unsigned char *str);
void update_audiobook_rating(HASHTYPE bookid, short r);
void update_Audiobook_totaltime(HASHTYPE bookid, short old, short new);
Audiobook *mergesorted(Audiobook *lptr1,Audiobook *lptr2);
Audiobook *msort(Audiobook*lptr);
void sortedInsert( Audiobook** sorted, Audiobook* p);
Audiobook* insertionSort(Audiobook* head);
void sortedInsert_userbook( mybook** sorted, mybook* p);
mybook* insertionSort_userbook(mybook* head);
