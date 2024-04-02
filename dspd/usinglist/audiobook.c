/*user list and library list needs to be a priosrity queue*/
/*CREATE node functions only malloc*/
/*ADD functions insert at apt positions sorted order*/

#include "audiobook.h"
extern Audiobook *Library;
extern user *uList;
bool isLogged = false;
HASHTYPE loggedID = 0;

HASHTYPE HASH(unsigned char *str)
{ /*
1.For generating unique userID
2. Also helps in checking if mail is unique
*/
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

void DEBUG()
{
    printf("\n================================");

    printf("\n\tUSERLIST");
    user *p = uList;
    while (p != NULL)
    {
        printf("\n\t\t%s %hu", p->name, p->uid);
        mybook *lib = p->mybooks;
        while (lib != NULL)
        {
            printf("\n\t\t\t%s %hu", lib->title, lib->id);
            lib = lib->next;
        }
        p = p->next;
    }

    printf("\n\tBOOKLIST");
    Audiobook *q = Library;
    while (q != NULL)
    {
        printf("\n\t\t%s %hu", q->title, q->id);
        q = q->next;
    }
    printf("\n================================");
}

void FILEWRITER()
{

    /*WRITING BOOKS*/
    FILE *file = fopen("../DATABASE/BOOKS.dat", "wb");
    Audiobook *p1 = Library;
    while (p1)
    {
        fwrite(p1, sizeof(Audiobook), 1, file);
        p1 = p1->next;
    }
    fclose(file);

    /*WRITING USERS*/
    file = fopen("../DATABASE/USERS.dat", "wb");
    user *p2 = uList;
    while (p2)
    {
        fwrite(p2, sizeof(user), 1, file);
        p2 = p2->next;
    }
    fclose(file);

    /*WRITING USER LIBRARIES*/
    user *p3 = uList;
    mybook *book = NULL;
    while (p3)
    {
        book = p3->mybooks;
        short id = p3->uid;
        char buf[SIZE];
        snprintf(buf, sizeof(buf), "../DATABASE/%hu.dat", id);
        FILE *f = fopen(buf, "wb+");

        /*start writing*/
        while (book)
        {
            fwrite(book, sizeof(mybook), 1, f);
            book = book->next;
        } /*all books of current written*/

        fclose(f);
        p3 = p3->next;
    }
}

void FILEREADER()
{
    /*READING BOOKS*/
    FILE *file = fopen("../DATABASE/BOOKS.dat", "rb");
    Audiobook *lib = NULL;
    Audiobook *p1 = NULL; // current pointer
    Audiobook temp;

    while (fread(&temp, sizeof(Audiobook), 1, file) == 1)
    {
        Audiobook *new = create_book_node(temp.id, temp.min, temp.title, temp.author, temp.narrator, temp.genre, temp.rating, temp.num_ratings, temp.total_listening_time);
        if (lib == NULL)
        {
            lib = new;
            p1 = lib;
        }
        else
        {
            p1->next = new;
            p1 = p1->next;
        }
    }
    fclose(file);
    Library = lib;

    /*READING USERS*/
    file = fopen("../DATABASE/USERS.dat", "rb");
    user *users = NULL;
    user *p2 = NULL; // current pointer
    user node;

    while (fread(&node, sizeof(user), 1, file) == 1)
    {
        user *new = create_user_node(node.uid, node.name, node.mail);
        if (users == NULL)
        {
            users = new;
            p2 = users;
        }
        else
        {
            p2->next = new;
            p2 = p2->next;
        }
    }
    fclose(file);
    uList = users;

    /*READING USER LIBRARIES*/
    user *p3 = uList;
    while (p3)
    {
        mybook *books = NULL;
        mybook *current = NULL;
        short id = p3->uid;
        char buf[SIZE];

        /*start reading*/
        snprintf(buf, sizeof(buf), "../DATABASE/%hu.dat", id);
        FILE *f = fopen(buf, "rb");
        mybook node;
        while (fread(&node, sizeof(mybook), 1, file) == 1)
        {
            mybook *new = create_mybook_node(node.id, node.min, node.title, node.author, node.narrator, node.genre, node.percent, node.uRating);
            if (books == NULL)
            {
                books = new;
                current = books;
            }
            else
            {
                current->next = new;
                current = current->next;
            }
        }
        fclose(f);
        /*all books of current read*/

        /*now save collected books to users mybooks collection*/
        p3->mybooks = books;
        p3 = p3->next;
    }
}

HASHTYPE LOG_USER(bool log)
{
    /*Logger function: If var 'log' is passed true:
        Checks if someone is logged or not. If logged, it returns their ID. If not logged it logs in and returns ID
    If var 'log' is passed false:
        It logs out and sets isLogged to false, sets loggedID to 0 (doesn't return anything)*/
    if (log == false)
    {
        printf("\n-------------------------------\n\tLOGGING OUT...\n-------------------------------");
        isLogged = false;
        loggedID = 0;
    }
    else
    {
        if (isLogged == true)
        {
            printf("\n-------------------------------\n\tYOU ARE LOGGED IN AS ID = %hu\n-------------------------------", loggedID);
            return loggedID;
        }
        else
        {
            printf("\n-------------------------------\n\tYOU ARE LOGGED OUT. ENTER ID TO LOGIN: ");
            HASHTYPE id;
            scanf("%hu", &id);

            /*NOW CHECK IF ID VALID OR NOT*/
            user *u = uList;
            while (u != NULL && id != u->uid)
                u = u->next;
            if (u == NULL)
            { /*USER NOT FOUND*/
                printf("\n-------------------------------\n\tNO SUCH USER EXISTS, SO NOT LOGGED IN\n-------------------------------");
            }

            else
            { /*USER  FOUND*/
                loggedID = id;
                isLogged = true;
                return loggedID;
            }
        }
    }
}

user *create_user_node(HASHTYPE id, char name[], char mail[])
{
    user *head = uList;
    head = (user *)malloc(sizeof(user));
    head->uid = id;
    head->mybooks = NULL;
    head->next = NULL;
    strcpy(head->name, name);
    strcpy(head->mail, mail);
    return head;
}

Audiobook *create_book_node(HASHTYPE id, short min, char title[], char author[], char narrator[], char genre[], short r, short n, short t)
{
    Audiobook *head = Library;
    head = (Audiobook *)malloc(sizeof(Audiobook));
    head->id = id;
    head->min = min;
    head->rating = r;
    head->num_ratings = n;
    head->total_listening_time = t;
    head->next = NULL;
    strcpy(head->author, author);
    strcpy(head->title, title);
    strcpy(head->narrator, narrator);
    strcpy(head->genre, genre);
    return head;
}

mybook *create_mybook_node(HASHTYPE id, short min, char tit[], char aut[], char nar[], char gen[], float p, short r)
{
    mybook *head = (mybook *)malloc(sizeof(mybook));
    head->id = id;
    head->min = min;
    head->percent = p;
    head->uRating = r;
    head->next = NULL;
    strcpy(head->author, aut);
    strcpy(head->title, tit);
    strcpy(head->narrator, nar);
    strcpy(head->genre, gen);
    return head;
}

user *Add_user(char name[], char mail[])
{
    user *head = uList;
    HASHTYPE id = HASH(mail); // we know that this is going to be unique

    if (head == NULL || id < head->uid)
    {
        user *new = create_user_node(id, name, mail);
        new->next = head;
        head = new;
    }

    else
    {
        user *p = head;
        while (p->next != NULL && p->next->uid < id)
        {
            p = p->next;
        }

        if (p->uid == id || (p->next != NULL && p->next->uid == id))
        {
            printf("\n----------------------------------------------------------------------------------------------\n\tDUPLICATE EMAIL %s, SO NOT ACCEPTED\n----------------------------------------------------------------------------------------------", mail);
            return head;
        }

        user *new = create_user_node(id, name, mail);
        new->next = p->next;
        p->next = new;
    }
    printf("\n----------------------------------------------------------------------------------------------\n\tUSER ADDED SUCCESFULLY (name=%s email=%s). NOTE ID = %hu\n----------------------------------------------------------------------------------------------", name, mail, id);
    return head;
}

Audiobook *Add_audiobook(short min, char tit[], char aut[], char nar[], char gen[])
{
    Audiobook *head = Library;
    char toHash[2 * SIZE + 1];
    strcat(toHash, tit);
    strcat(toHash, aut);
    HASHTYPE id = HASH(toHash); // we know that this is going to be unique

    if (head == NULL || id < head->id)
    {
        Audiobook *new = create_book_node(id, min, tit, aut, nar, gen, 0, 0, 0);
        new->next = head;
        head = new;
    }

    else
    {
        Audiobook *p = head;
        while (p->next != NULL && p->next->id < id)
            p = p->next;

        if (p->id == id || (p->next != NULL && p->next->id == id))
        {
            printf("\n----------------------------------------------------------------------------------------------\n\tDUPLICATE BOOK %s, SO NOT ACCEPTED\n----------------------------------------------------------------------------------------------", tit);
            return head;
        }
        Audiobook *new = create_book_node(id, min, tit, aut, nar, gen, 0, 0, 0);
        new->next = p->next;
        p->next = new;
    }
    printf("\n----------------------------------------------------------------------------------------------\n\tBOOK ADDED SUCCESFULLY (title=%s author=%s)\n----------------------------------------------------------------------------------------------", tit, aut);
    return head;
}

void Add_to_user_library(HASHTYPE id, HASHTYPE bookid)
{
    /*FIND THE BOOK*/

    Audiobook *lib = Library;
    while (lib != NULL && bookid != lib->id)
        lib = lib->next;
    if (lib == NULL)
    {
        printf("\n----------------------------------------------------------------------------------------------\n\tNO SUCH BOOK FOUND WITH ID=%hu\n----------------------------------------------------------------------------------------------", bookid);
        return;
    }
    char tit[SIZE], nar[SIZE], aut[SIZE], gen[SIZE];
    strcpy(tit, lib->title);
    strcpy(aut, lib->author);
    strcpy(nar, lib->narrator);
    strcpy(gen, lib->genre);
    short min = lib->min;
    HASHTYPE bkid = lib->id;
    /*FIND THE USER*/

    if (uList != NULL)
    {
        user *u = uList;
        while (u != NULL && u->uid != id)
        {
            u = u->next;
        }

        if (u != NULL && u->uid == id)
        { /*if user with id found*/
            printf("\n----------------------------------------------------------------------------------------------\n\tUSER FOUND (name=%s email=%s) ADDING BOOK=%s TO MYLIBRARY\n----------------------------------------------------------------------------------------------", u->name, u->mail, tit);
            /*user matched, here we will add the book to your library*/

            if (u->mybooks == NULL)
                u->mybooks = create_mybook_node(bkid, min, tit, aut, nar, gen, 0, 0);
            else
            {
                mybook *p = u->mybooks;
                while (p->next != NULL && p->next->id < bkid)
                    p = p->next;

                if (p->id == bkid || (p->next != NULL && p->next->id == bkid))
                {
                    printf("\n----------------------------------------------------------------------------------------------\n\tBOOK \"%s\" ALREADY PRESENT, SO NOT ACCEPTED\n----------------------------------------------------------------------------------------------", tit);
                    return;
                }
                mybook *new = create_mybook_node(bkid, min, tit, aut, nar, gen, 0, 0);
                new->next = p->next;
                p->next = new;
            }
            return;
        }
        else
            printf("\n----------------------------------------------------------------------------------------------\n\tNO USER FOUND WITH ID=%hu\n----------------------------------------------------------------------------------------------", id);
    }
    else
        printf("\n----------------------------------------------------------------------------------------------\n\tNO USER FOUND WITH ID=%hu\n----------------------------------------------------------------------------------------------", id);
}

void Search_audiobook()
{
    Audiobook *lib = Library;
    int choice;
    char s1[SIZE], s2[SIZE];
    char a[SIZE] = "TITLE";
    char b[SIZE] = "AUTHOR";
    char c[SIZE] = "GENRE";
    char d[SIZE] = "LENGTH (minutes)";
    char e[SIZE] = "TOTAL LISTENED (minutes)";
    char f[SIZE] = "RATING";
    printf("\n---------------------------------------------------------------------------------------------------------------------------------------------\n\tSEARCH OPTIONS:\n\t\t1. By Title OR Author\n\t\t2. By Title OR Genre\n\t\t3. By Genre OR Author\n\t\t4. All Books");
    scanf("%d", &choice);
    printf("\n---------------------------------------------------------------------------------------------------------------------------------------------\n%30s%30s%20s%20s%20s%20s\n", a, b, c, d, e, f);
    switch (choice)
    {
    case 1:
        printf("\n\t TITLE:");
        getchar();
        fgets(s1, SIZE, stdin);

        printf("\n\t AUTHOR:");
        fgets(s2, SIZE, stdin);

        s1[strcspn(s1, "\n")] = '\0';
        s2[strcspn(s2, "\n")] = '\0';
        /*FIND THE BOOK*/
        while (lib != NULL)
        {
            if (strcmp(s1, lib->title) == 0 || strcmp(s2, lib->author) == 0)
                printf("\n%30s%30s%20s%20hu%20hu%20hu", lib->title, lib->author, lib->genre, lib->min, lib->total_listening_time, lib->rating);
            lib = lib->next;
        }
        break;
    case 2:
        printf("\n\t TITLE:");
        getchar();
        fgets(s1, SIZE, stdin);

        printf("\n\t GENRE: ");
        fgets(s2, SIZE, stdin);

        s1[strcspn(s1, "\n")] = '\0';
        s2[strcspn(s2, "\n")] = '\0';
        /*FIND THE BOOK*/
        while (lib != NULL)
        {
            if (strcmp(s1, lib->title) == 0 || strcmp(s2, lib->genre) == 0)
                printf("\n%30s%30s%20s%20hu%20hu%20hu", lib->title, lib->author, lib->genre, lib->min, lib->total_listening_time, lib->rating);
            lib = lib->next;
        }
        break;
    case 3:
        printf("\n\t GENRE:");
        getchar();
        fgets(s1, SIZE, stdin);

        printf("\n\t AUTHOR:");
        fgets(s2, SIZE, stdin);

        s1[strcspn(s1, "\n")] = '\0';
        s2[strcspn(s2, "\n")] = '\0';
        /*FIND THE BOOK*/
        while (lib != NULL)
        {
            if (strcmp(s1, lib->genre) == 0 || strcmp(s2, lib->author) == 0)
                printf("\n%30s%30s%20s%20hu%20hu%20hu", lib->title, lib->author, lib->genre, lib->min, lib->total_listening_time, lib->rating);
            lib = lib->next;
        }
        break;
    case 4:
        Audiobook* lib = insertionSort(Library);
        while (lib != NULL)
        {
            printf("\n%30s%30s%20s%20hu%20hu%20hu", lib->title, lib->author, lib->genre, lib->min, lib->total_listening_time, lib->rating);
            lib = lib->next;
        }
        /*FREE THE UNNECESSARILY CREATED LIST*/
        while (lib != NULL) {
        Audiobook* temp = lib;
        lib = lib->next;
        free(temp);
    }
    }
    printf("\n---------------------------------------------------------------------------------------------------------------------------------------------");
}

void Delete_audiobook(char tit[], char aut[])
{
    /*FIND THE BOOK*/
    Audiobook *lib = Library;
    if (lib == NULL)
        return;
    else if (strcmp(tit, lib->title) == 0 && strcmp(aut, lib->author) == 0)
    {
        Library = Library->next;
        free(lib);
        return;
    }
    while (lib->next != NULL)
    {
        if (strcmp(tit, lib->next->title) == 0 && strcmp(aut, lib->next->author) == 0)
        {
            Audiobook *p = lib->next;
            lib->next = lib->next->next;
            free(p);
            printf("\n----------------------------------------------------------------------------------------------\n\tBOOK DELETED SUCCESSFULLY\n----------------------------------------------------------------------------------------------");
            return;
        }
        lib = lib->next;
    }
    printf("\n----------------------------------------------------------------------------------------------\n\tNO SUCH BOOK FOUND\n----------------------------------------------------------------------------------------------");
}

void update_Audiobook_totaltime(HASHTYPE bookid, short old, short new)
{
    /*FIND THE BOOK*/
    Audiobook *lib = Library;
    while (lib != NULL && lib->id != bookid)
        lib = lib->next;
    if (lib != NULL)
        lib->total_listening_time += (new - old);
}

void update_audiobook_rating(HASHTYPE bookid, short r)
{
    /*FIND THE BOOK*/
    Audiobook *lib = Library;
    while (lib != NULL && lib->id != bookid)
        lib = lib->next;
    if (lib != NULL)
    {
        lib->rating = ((lib->rating) * (lib->num_ratings) + r) / (lib->num_ratings + 1);
        lib->num_ratings++;
    }
}

void Listening_progress(HASHTYPE id, HASHTYPE bookid, short m)
{
    /*FIND THE USER*/
    user *u = uList;
    while (u != NULL && id != u->uid)
        u = u->next;
    if (u != NULL)
    {
        /*FIND THE BOOK*/
        mybook *lib = u->mybooks;
        while (lib != NULL && bookid != lib->id)
            lib = lib->next;
        if (lib != NULL)
        {
            /*SUBPROCESS: also if someone updates their listening progress*/
            update_Audiobook_totaltime(bookid, (short)(lib->percent / 100 * lib->min), m);
            /*NOW UPDATING USER TIME...*/
            lib->percent = 100 * ((float)m / lib->min);
            printf("\n----------------------------------------------------------------------------------------------\n\tYOUR LISTENING PROGRESS IS NOW %f percents\n----------------------------------------------------------------------------------------------", lib->percent);
            return;
        }
        printf("\n----------------------------------------------------------------------------------------------\n\tNO SUCH BOOK FOUND IN YOUR LIBRARY WITH ID=%hu\n----------------------------------------------------------------------------------------------", bookid);
        return;
    }
    printf("\n----------------------------------------------------------------------------------------------\n\tNO SUCH USER FOUND WITH ID=%hu\n----------------------------------------------------------------------------------------------", id);
}

void Rate_Audiobook(HASHTYPE id, HASHTYPE bookid, short r)
{
    /*FIND THE USER*/
    user *u = uList;
    while (u != NULL && id != u->uid)
        u = u->next;
    if (u != NULL)
    {
        /*FIND THE BOOK*/
        mybook *lib = u->mybooks;
        while (lib != NULL && bookid != lib->id)
            lib = lib->next;
        if (lib != NULL)
        {
            /*SUBPROCESS:update book rating if i am rating my book*/
            update_audiobook_rating(bookid, r);
            lib->uRating = r;
            printf("\n\tYOUR RATING OF BOOK %s IS NOW %hu*", lib->title, r);
            return;
        }
        printf("\n----------------------------------------------------------------------------------------------\n\tNO SUCH BOOK FOUND IN YOUR LIBRARY WITH ID=%hu\n----------------------------------------------------------------------------------------------", bookid);
        return;
    }
    printf("\n----------------------------------------------------------------------------------------------\n\tNO SUCH USER FOUND WITH ID=%hu\n----------------------------------------------------------------------------------------------", id);
}

void Listening_history_report_audiobook(HASHTYPE bookid)
{
    /*FIND THE BOOK*/
    Audiobook *lib = Library;
    while (lib != NULL && lib->id != bookid)
        lib = lib->next;
    if (lib != NULL)
    {
        printf("\n------------------------------------------------------------------------------------------------\n\tTITLE=%s AUTHOR=%s TOTAL LISTENING TIME=%hu\n----------------------------------------------------------------------------------------------", lib->title, lib->author, lib->total_listening_time);
    }
}

void Listening_history_report_user(HASHTYPE id)
{
    /*FIND THE USER*/
    user *u = uList;
    while (u != NULL && id != u->uid)
        u = u->next;
    if (u != NULL)
    {
        float total = 0;
        char a[SIZE] = "TITLE";
        char b[SIZE] = "AUTHOR";
        char c[SIZE] = "LISTENED (percent)";
        char d[SIZE] = "YOUR RATING";
        printf("\n--------------------------------------------------------------------------------------------------------------------\n%30s%30s%30s%20s\n", a, b, c, d);
        mybook *lib = u->mybooks;
        while (lib != NULL)
        {
            total += (lib->percent) * (lib->min) / 100;
            printf("\n%30s%30s%30f%20hu", lib->title, lib->author, lib->percent, lib->uRating);
            lib = lib->next;
        }
        printf("\n\tAND YOUR TOTAL LISTENING TIME IS %f minutes\n--------------------------------------------------------------------------------------------------------------------", total);
    }
}

void Most_poplular_audiobook()
{
    Audiobook *lib = Library;
    Audiobook *max = Library;
    if (lib == NULL)
        return;
    while (lib != NULL)
    {
        if (lib->rating > max->rating)
            max = lib;
        lib = lib->next;
    }
    if (max->rating != 0)
        printf("\n----------------------------------------------------------------------------------------------\n\tMOST POPULAR AUDIOBOOK IS title=%s author=%s rating=%hu\n----------------------------------------------------------------------------------------------", max->title, max->author, max->rating);
    else
        printf("\n----------------------------------------------------------------------------------------------\n\tNO BOOKS HAVE BEEN RATED YET.\n----------------------------------------------------------------------------------------------");
}

void Display_user_lib(HASHTYPE id)
{
    /*FIND THE USER*/
    user *u = uList;
    while (u != NULL && id != u->uid)
        u = u->next;
    if (u != NULL)
    {
        mybook *lib = u->mybooks;
        int choice;
        char s1[SIZE], s2[SIZE];
        char a[SIZE] = "TITLE";
        char b[SIZE] = "AUTHOR";
        char c[SIZE] = "GENRE";
        char d[SIZE] = "PERCENT LISTENED";

        printf("\n----------------------------------------------------------------------------------------------\n\t OPTIONS:\n\t\t1. By Title OR Author\n\t\t2. By Title OR Genre\n\t\t3. By Genre OR Author\n\t\t4. All Books");
        scanf("%d", &choice);
        printf("\n------------------------------------------------------------------------------------------------------\n%30s%30s%30s%30s\n", a, b, c, d);
        switch (choice)
        {

        case 1:
            printf("\n\t TITLE:");
            getchar();
            fgets(s1, SIZE, stdin);

            printf("\n\t AUTHOR:");
            fgets(s2, SIZE, stdin);

            s1[strcspn(s1, "\n")] = '\0';
            s2[strcspn(s2, "\n")] = '\0';
            /*FIND THE BOOK*/
            while (lib != NULL)
            {
                if (strcmp(s1, lib->title) == 0 || strcmp(s2, lib->author) == 0)
                    printf("\n%30s%30s%30s%30f", lib->title, lib->author, lib->genre, lib->percent);
                lib = lib->next;
            }
            break;
        case 2:
            printf("\n\t TITLE:");
            getchar();
            fgets(s1, SIZE, stdin);

            printf("\n\t GENRE: ");
            fgets(s2, SIZE, stdin);

            s1[strcspn(s1, "\n")] = '\0';
            s2[strcspn(s2, "\n")] = '\0';
            /*FIND THE BOOK*/
            while (lib != NULL)
            {
                if (strcmp(s1, lib->title) == 0 || strcmp(s2, lib->genre) == 0)
                    printf("\n%30s%30s%30s%30f", lib->title, lib->author, lib->genre, lib->percent);
                lib = lib->next;
            }
            break;
        case 3:
            printf("\n\t GENRE:");
            getchar();
            fgets(s1, SIZE, stdin);

            printf("\n\t AUTHOR:");
            fgets(s2, SIZE, stdin);

            s1[strcspn(s1, "\n")] = '\0';
            s2[strcspn(s2, "\n")] = '\0';
            /*FIND THE BOOK*/
            while (lib != NULL)
            {
                if (strcmp(s1, lib->genre) == 0 || strcmp(s2, lib->author) == 0)
                    printf("\n%30s%30s%30s%30f", lib->title, lib->author, lib->genre, lib->percent);
                lib = lib->next;
            }
            break;
        case 4:
            while (lib != NULL)
            {
                printf("\n%30s%30s%30s%30f", lib->title, lib->author, lib->genre, lib->percent);
                lib = lib->next;
            }
        }
        printf("\n----------------------------------------------------------------------------------------------");
    }
    else
        printf("\n-------------------------------------------------------\n\tUSER NOT FOUND\n-----------------------------------------------");
}

void Merge_user_libraries(HASHTYPE uid1, HASHTYPE uid2)
{
    user *ptr = uList;
    user *ptr1, *ptr2;
    while (ptr)
    {
        if (ptr->uid == uid1)
            ptr1 = ptr;
        if (ptr->uid == uid2)
            ptr2 = ptr;

        ptr = ptr->next;
    }

    mybook *temp1 = ptr1->mybooks;
    mybook *start = temp1;
    while (temp1->next)
    {
        temp1 = temp1->next;
    }

    temp1->next = ptr2->mybooks;
    temp1 = insertionSort_userbook(ptr1->mybooks);
    /*START PRINTING*/
    char a[SIZE] = "TITLE";
    char b[SIZE] = "AUTHOR";
    char c[SIZE] = "NARRATOR";
    printf("\n------------------------------------------------------------------------------------------------------\n\tMERGING...\n%30s%30s%30s\n", a, b, c);

    while (temp1)
    {
        printf("\n%30s%30s%30s", temp1->title, temp1->author, temp1->narrator);
        temp1 = temp1->next;
    }
    printf("\n----------------------------------------------------------------------------------------------");
    /*FREE THE UNNECESSARILY CREATED LIST*/
    while (temp1 != NULL) {
    mybook* temp = temp1;
    temp1 = temp1->next;
    free(temp);
    }
}

void Display_audiobook_rating(Audiobook *after_edit)
{
    char a[SIZE] = "TITLE";
    char b[SIZE] = "AUTHOR";
    char c[SIZE] = "GENRE";
    char d[SIZE] = "LENGTH (minutes)";
    char e[SIZE] = "TOTAL LISTENED (minutes)";
    char f[SIZE] = "RATING";
    printf("\n---------------------------------------------------------------------------------------------------------------------------------------------\n%30s%30s%20s%20s%20s%20s\n", a, b, c, d, e, f);

    Audiobook *lib = after_edit;
    while (lib)
    {
        printf("\n%30s%30s%20s%20hu%20hu%20hu", lib->title, lib->author, lib->genre, lib->min, lib->total_listening_time, lib->rating);
        lib = lib->next;
    }
    printf("\n---------------------------------------------------------------------------------------------------------------------------------------------");
}

void sortedInsert(Audiobook **sorted, Audiobook *p)
{
    Audiobook *current;

    // Special case for the head node or if the new node comes before the head
    if (*sorted == NULL || strcmp((*sorted)->title, p->title) > 0 ||
        (strcmp((*sorted)->title, p->title) == 0 &&
         strcmp((*sorted)->author, p->author) > 0))
    {
        Audiobook *temp = create_book_node(p->id, p->min, p->title, p->author, p->narrator, p->genre, p->rating, p->num_ratings, p->total_listening_time);
        temp->next=*sorted;
        *sorted = temp;
    }
    else
    {
        // Locate the node before the point of insertion
        current = *sorted;
        while (current->next != NULL &&
               (strcmp(current->next->title, p->title) < 0 ||
                (strcmp(current->next->title, p->title) == 0 &&
                 strcmp(current->next->author, p->author) < 0)))
        {
            current = current->next;
        }
        Audiobook *temp = create_book_node(p->id, p->min, p->title, p->author, p->narrator, p->genre, p->rating, p->num_ratings, p->total_listening_time);
        temp->next=current->next;
        current->next = temp;
    }
}

Audiobook *insertionSort(Audiobook *head)
{
    Audiobook *sorted = NULL;
    Audiobook *current = head;

    while (current != NULL)
    {
        Audiobook *next = current->next;
        sortedInsert(&sorted, current);
        current = next;
    }
    return sorted;
}

void sortedInsert_userbook(mybook **sorted, mybook *p)
{
    mybook *current;

    // Special case for the head node or if the new node comes before the head
    if (*sorted == NULL || strcmp((*sorted)->title, p->title) > 0 ||
        (strcmp((*sorted)->title, p->title) == 0 &&
         strcmp((*sorted)->author, p->author) > 0))
    {
        mybook *temp = create_mybook_node(p->id, p->min, p->title, p->author, p->narrator, p->genre,p->percent,p->uRating);
        temp->next=*sorted;
        *sorted = temp;
    }
    else
    {
        // Locate the node before the point of insertion
        current = *sorted;
        while (current->next != NULL &&
               (strcmp(current->next->title, p->title) < 0 ||
                (strcmp(current->next->title, p->title) == 0 &&
                 strcmp(current->next->author, p->author) < 0)))
        {
            current = current->next;
        }
        mybook *temp = create_mybook_node(p->id, p->min, p->title, p->author, p->narrator, p->genre,p->percent,p->uRating);
        temp->next=current->next;
        current->next = temp;
    }
}

mybook *insertionSort_userbook(mybook *head)
{
    mybook *sorted = NULL;
    mybook *current = head;

    while (current != NULL)
    {
        mybook *next = current->next;
        sortedInsert_userbook(&sorted, current);
        current = next;
    }
    return sorted;
}

Audiobook *mergesorted(Audiobook *lptr1,Audiobook *lptr2)
{
    Audiobook*head=NULL,*tail=NULL;
    while(lptr1&&lptr2)
    {
        if(lptr1->rating<=lptr2->rating)
        {
            if(head==NULL)
            {
                head=tail=lptr1;
                lptr1=lptr1->next;
            }
            else
            {
                tail->next=lptr1;
                lptr1=lptr1->next;
                tail=tail->next;
                tail->next=NULL;
            }
        }
        else
        {
            if(head==NULL)
            {
                head=tail=lptr2;
                lptr2=lptr2->next;
            }
            else
            {
                tail->next=lptr2;
                lptr2=lptr2->next;
                tail=tail->next;
                tail->next=NULL;
            }
        }
    }
    if(lptr1)
    {
        tail->next=lptr1;
    }
    if(lptr2)
    {
        tail->next=lptr2;
    }
    return head;
}
Audiobook *msort(Audiobook*lptr)
{
    Audiobook *newlptr=lptr;
    if(lptr&&lptr->next)
    {
        Audiobook *slow=lptr,*fast=lptr->next;
        while(fast&&fast->next)
        {
            slow=slow->next;
            fast=fast->next;
            if(fast)fast=fast->next;
        }
        Audiobook *lptr2=slow->next;;
        slow->next=NULL;
        lptr=msort(lptr);
        lptr2=msort(lptr2);
        newlptr=mergesorted(lptr,lptr2);
    }
    return newlptr;
}
