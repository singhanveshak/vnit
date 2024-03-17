/*driver code.
FRIEND FILES=audiobook.h, audiobook.c*/
#include "audiobook.h"

Audiobook *Library = NULL;
user *uList = NULL;

void main()
{
    char s1[SIZE], s2[SIZE], s3[SIZE], s4[SIZE];
    short id, bookid, var;
    int choice = -1;

    /*USER INTERFACE*/
    printf("\n-----------------------------------\n\tLOADING FILES......\n-----------------------------------");
    FILEREADER();
    while (choice != 0)
    {
        // debug();
        printf("\n\tSPECIAL OPTIONS\n\t-1. LOG IN\n\t-2. LOG OUT\n\t-3. SHOW_DATABASE\n\t-4. SAVE AGAIN\n\t0. EXIT\n\n1. Add Book\n2. Search Book\n3. Delete Book\n4. Add User\n5. My Library\n6. New Time Stamp\n7. Rate Book\n8. Add to My Library\n9. My Listening History\n10. Ppopular book\n11. Show Sorted Books\n12. Show Book Listening History\n13. Merge Libraries\n");
        scanf("%d", &choice);
        switch (choice)
        {
        case 0:
            printf("\n-----------------------------------\n\tSAVING FILES......\n-----------------------------------");
            FILEWRITER();
            printf("\n\tEXITING.....\n");
            break;
        case 1:
            printf("\n\t TITLE:");
            getchar();
            fgets(s1, SIZE, stdin);

            printf("\n\t AUTHOR:");
            fgets(s2, SIZE, stdin);

            printf("\n\t NARRATOR:");
            fgets(s3, SIZE, stdin);

            printf("\n\t GENRE:");
            fgets(s4, SIZE, stdin);

            s1[strcspn(s1, "\n")] = '\0';
            s2[strcspn(s2, "\n")] = '\0';
            s3[strcspn(s3, "\n")] = '\0';
            s4[strcspn(s4, "\n")] = '\0';

            printf("\n\t LISTENING LENGTH: ");
            scanf("%hu", &var);

            Library = Add_audiobook(var, s1, s2, s3, s4);
            break;
        case 2:
            Search_audiobook();
            break;
        case 3:
            printf("\n\t TITLE ");
            getchar();
            fgets(s1, SIZE, stdin);

            printf("\n\t AUTHOR ");
            fgets(s2, SIZE, stdin);

            s1[strcspn(s1, "\n")] = '\0';
            s2[strcspn(s2, "\n")] = '\0';

            Delete_audiobook(s1, s2);
            break;
        case 4:
            printf("\n\t NAME ");
            getchar();
            fgets(s1, SIZE, stdin);

            printf("\n\t EMAIL ");
            scanf("%s", s2);

            s1[strcspn(s1, "\n")] = '\0';
            s2[strcspn(s2, "\n")] = '\0';

            uList = Add_user(s1, s2);
            break;
        case 5:
        id=LOG_USER(true);
        Display_user_lib(id);
            break;
        case 6:
            id = LOG_USER(true);

            printf("\n\t BOOKID ");
            scanf("%hu", &bookid);
            printf("\n\t YOUR READING PROGRESS(min) ");
            scanf("%hu", &var);
            Listening_progress(id, bookid, var);
            break;
        case 7:
            id = LOG_USER(true);

            printf("\n\t BOOKID ");
            scanf("%hu", &bookid);
            printf("\n\t YOUR RATING(min) ");
            scanf("%hu", &var);
            Rate_Audiobook(id, bookid, var);
            break;
        case 8:
            id = LOG_USER(true);

            printf("\n\t BOOKID ");
            scanf("%hu", &bookid);
            Add_to_user_library(id, bookid);
            break;
        case 9:
            id = LOG_USER(true);

            Listening_history_report_user(id);
            break;
        case 10:
            Most_poplular_audiobook();
            break;
        case 11:
            Audiobook *after_edit = NULL;
            Audiobook* nptr;
            Audiobook *temp = Library;
            while (temp)
            {
                nptr = (Audiobook *)malloc(sizeof(Audiobook));
                nptr->id = temp->id;
                strcpy(nptr->title, temp->title);
                strcpy(nptr->author, temp->author);
                strcpy(nptr->narrator, temp->narrator);
                strcpy(nptr->genre, temp->genre);
                nptr->min = temp->min;
                nptr->total_listening_time = temp->total_listening_time;
                nptr->rating = temp->rating;
                nptr->num_ratings = temp->num_ratings;
                nptr->next = after_edit;
                after_edit = nptr;
                temp = temp->next;
            }

            after_edit = msort(after_edit);
            Display_audiobook_rating(after_edit);
            break;        case 12:
            printf("\n\t BOOKID ");
            scanf("%hu", &bookid);
            Listening_history_report_audiobook(bookid);
            break;
        case 13:
            id=LOG_USER(true);
            printf("\n\tENTER THE ID OF THE USER TO MERGE FROM: ");
            scanf("%hu",&var);
            
            Merge_user_libraries(id,var);
            break;
        case -1:
            LOG_USER(true);
            break;
        case -2:
            LOG_USER(false);
            break;
        case -3:
            DEBUG();
            break;
        case -4:
            FILEWRITER();
            break;
        default:
            printf("\n\tINVALID CHOICE");
            break;
        }
    }
}