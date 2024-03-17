/*ANVESHAK SINGH 17/3/2024*/
/*Program to implement symbol table and scoping and lookup*/
/*
It has 2 modes of running: 
1. Manual and 
2. File mode: Takes in a file 'script.txt' written in a made up language in which each
  line is either an assignment statement like a=1 or a lookup statement like ?a
  Scopes can be changed by tab indentation as in python
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLESIZE 100
#define SYMBOLSIZE 10

typedef enum
{
    false,
    true
} bool;

typedef struct SymbolEntry
{
    char name;
    int value;
    struct SymbolEntry *next;
} SymbolEntry;

typedef struct SymbolTable
{
    SymbolEntry *table[TABLESIZE];
    struct SymbolTable *parent;
} SymbolTable;

int hash(char name, int size)
{
    return (int)name % size;
}

SymbolTable *createSymbolTable(SymbolTable *parent)
{
    SymbolTable *table = (SymbolTable *)malloc(sizeof(SymbolTable));
    table->parent = parent;
    return table;
}

void insert(SymbolTable *table, char name, int value)
{
    if (table == NULL)
    {
        // printf("\n-------------------------------\n\tNULL TABLE SO NOT INSERTED\n-------------------------------");
        return;
    }
    int index = hash(name, TABLESIZE);
    SymbolEntry *entry = (SymbolEntry *)malloc(sizeof(SymbolEntry));
    entry->name = name;
    entry->value = value;
    entry->next = table->table[index];
    table->table[index] = entry;
}

bool lookup(SymbolTable *table, char name, int *retval)
{
    int index = hash(name, TABLESIZE);
    SymbolEntry *entry = table->table[index];
    while (entry)
    {
        if (entry->name == name)
        {
            *retval = entry->value;
            return true;
        }
        entry = entry->next;
    }
    if (table->parent != NULL)
    {
        return lookup(table->parent, name, retval);
    }
    return false; // Symbol not found
}

void printSymbolTable(SymbolTable *table)
{
    if (table == NULL)
        printf("\n\tNULL TABLE\n");
    else
    {
        printf("\n\tSYMBOL TABLE: (Higher Scope downwards)\n");

        short num_tabs = 0;
        while (table)
        {
            // first print current table
            printf("--------------------------------------------------\n");
            for (int i = 0; i < TABLESIZE; i++)
            {
                SymbolEntry *entry = table->table[i];
                while (entry)
                {
                    for (int k = 0; k < num_tabs; k++)
                    { // print some pretty tabs
                        printf("\t");
                    }
                    printf("%c:%d\n", entry->name, entry->value);
                    entry = entry->next;
                }
            }
            // then print table's parent
            table = table->parent;
            num_tabs++;
        }
        printf("\n--------------------------------------------------");
    }
}

void freeSymbolTable(SymbolTable *table)
{
    for (int i = 0; i < TABLESIZE; i++)
    {
        SymbolEntry *entry = table->table[i];
        while (entry)
        {
            SymbolEntry *temp = entry;
            entry = entry->next;
            free(temp);
        }
    }
    free(table);
}

SymbolTable *processLine(SymbolTable *table, const char *line, int *indent)
{
    SymbolTable *temp = NULL;
    // Count leading tabs to determine the current scope level
    int numTabs = 0;
    int prevIndent = *indent;
    while (line[numTabs] == ' ')
    {
        numTabs++;
    }

    *indent = numTabs / 4; // update indent
    line += numTabs;       // skip tabs

    if (prevIndent < *indent)
    {
        // add child scope
        printf("\n\tCREATING CHILD SCOPE...");
        temp = createSymbolTable(table);
        table = temp;
    }
    if (prevIndent > *indent)
    {
        if (table)
        {
            printf("\n\tFREEING CURRENT SCOPE...");
            temp = table->parent;
            freeSymbolTable(table);
            table = temp;
        }
        else
            printf("\n\tNULL TABLE SO CANNOT FREE");
    }

    // Check if it's an assignment or lookup statement
    if (line[0] == '?')
    {
        char name = line[1];
        int val;
        printf("\n\tLOOKING UP SYMBOL '%c'", name);
        if (lookup(table, name, &val) != false)
        {
            // printSymbolTable(table);
            printf("\n\t\tSYMBOL '%c' FOUND: %c = %d", name, name, val);
        }
        else
        {
            printf("\n\t\tSYMBOL '%c' NOT FOUND", name);
        }
    }
    else
    {
        char name = line[0];
        int value = atoi(line + 2); // Skip the '=' character
        insert(table, name, value);
        printf("\n\tINSERTED %c=%d", name, value);
    }
    return table;
}

SymbolTable *readFile(SymbolTable *table)
{
    FILE *file = fopen("script.txt", "r");
    char line[100];
    int currentIndent = 0;
    while (fgets(line, sizeof(line), file))
    {
        line[strcspn(line, "")] = 0;
        table = processLine(table, line, &currentIndent);
    }
    fclose(file);
    printf("\n");
    return table;
}

int main()
{
    SymbolTable *current = createSymbolTable(NULL);
    SymbolTable *parent = NULL;
    int choice = 0, val;
    char c;

    printf("\n1. Enter file mode\n2. Enter manual mode ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        current = readFile(current);
    }
    else
    {
        while (true)
        {
            printf("\n-1. Print symbol table\n0. Exit\n1. New child scope\n2. Insert Symbol into scope\n3. Lookup Symbol\n4. Free current scope");
            scanf("%d", &choice);
            switch (choice)
            {
            case -1:
                printSymbolTable(current);
                break;
            case 0:
                printf("\n-------------------------------\n\tEXITING...\n-------------------------------\n");
                return 0;
            case 1:
                printf("\n-------------------------------\n\tCREATING CHILD SCOPE...\n-------------------------------");
                current = createSymbolTable(parent);
                parent = current;
                break;
            case 2:
                printf("\n-------------------------------\n\tENTER symbol name: ");
                getchar();
                scanf("%c", &c);
                printf("\n\tENTER symbol value: ");
                scanf("%d", &val);
                insert(current, c, val);
                printf("\n\tINSERTED\n-------------------------------");
                break;
            case 3:
                printf("\n-------------------------------\n\tENTER Symbol name: ");
                getchar();
                scanf("%c", &c);
                if (lookup(current, c, &val) != false)
                {
                    printf("\n\tSYMBOL '%c' FOUND: %c = %d\n-------------------------------", c, c, val);
                }
                else
                {
                    printf("\n\tSYMBOL '%c' NOT FOUND\n-------------------------------", c);
                }
                break;
            case 4:
                if (current)
                {
                    printf("\n-------------------------------\n\tFREEING CURRENT SCOPE...\n-------------------------------");
                    SymbolTable *ptr_parent = current->parent;
                    freeSymbolTable(current);
                    current = ptr_parent;
                    if (current)
                    {
                        parent = current->parent;
                    }
                    else
                    {
                        parent = NULL;
                    }
                }
                else
                {
                    printf("\n-------------------------------\n\tNULL TABLE SO CANNOT FREE\n-------------------------------");
                }
                break;
            default:
                printf("\n-------------------------------\n\tWRONG CHOICE !\n-------------------------------");
                break;
            }
        }
    }

    return 0;
}
