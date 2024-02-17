#include <stdio.h>
#include <stdlib.h>

// Structure representing a memory block, sizeof(struct Block) = 24
typedef struct Block {
    int size;
    struct Block *next;
    int free;
} Block;

// Global pointer to the start of the heap
Block *heap = NULL;

// Function to initialize the heap
void initialize_heap() {
    heap = (Block *)malloc(sizeof(Block));
    heap->size = 0;
    heap->next = NULL;
    heap->free = 1;
}

// Function to allocate memory
void *allocate(int size) {
    Block *current = heap;
    Block *prev = NULL;

    while (current != NULL) {
        if (current->free && current->size >= size) {
            // If block is larger (than 24 + size), split it
            if (current->size > size + sizeof(Block)) {
                Block *newBlock = (Block *)((char *)current + sizeof(Block) + size);
                newBlock->size = current->size - size - sizeof(Block);         
                newBlock->next = current->next;
                newBlock->free = 1;

                current->size = size;
                current->next = newBlock;
            }

            current->free = 0;
            return (void *)(current + 1); // Return pointer to the data
        }
        prev = current;
        current = current->next;
    }

    // If no suitable block found, allocate new memory
    Block *newBlock = (Block *)malloc(sizeof(Block) + size);
    newBlock->size = size;
    newBlock->next = NULL;
    newBlock->free = 0;

    if (prev != NULL)
        prev->next = newBlock;
    else
        heap = newBlock;

    return (void *)(newBlock); // Return pointer to the data
}

// Function to free memory
void free_memory(void *ptr) {
    if (ptr == NULL)
        return;

    ((Block*)ptr)->free=1;

    // Merge adjacent free blocks
    Block *current = heap;
    while (current != NULL && current->next != NULL) {
        if (current->free && current->next->free) {
            current->size += sizeof(Block) + current->next->size;
            current->next = current->next->next;
        }
        current = current->next;
    }
}

// Function to print heap status
void print_heap() {
    Block *current = heap;
    printf("\n\t\t\tHEAP FRAME");
    printf("\n+-------------------------------------------------------+\n|");
    while (current != NULL) {
        printf("\n|\tBlock %p - Size: %d, Free: %d\t\t|\n|", (void *)current, current->size, current->free);
        printf("\n|\t\t\t|\t\t\t\t|\n|\t\t\tV\t\t\t\t|\n|");
        current = current->next;
    }
    printf("\n+-------------------------------------------------------+\n");

}

int main() {
    int choice, size;
    void *ptr;

    initialize_heap();

    do {
        printf("\n\t1. Allocate\n\t2. Free\n\t3. Print Heap\n\t0. Exit\n\tEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\tEnter size to allocate: ");
                scanf("%d", &size);
                ptr = allocate(size);
                if (ptr == NULL)
                    printf("\tFailed to allocate memory!\n");
                else
                    printf("\tMemory allocated at: %p\n", ptr);
                break;
            case 2:
                printf("\tEnter pointer to free: ");
                scanf("%p", &ptr);
                free_memory(ptr);
                printf("\t----------------------\n\tMemory freed = sizeof(Block) + size of data = 24 + %d\n\t----------------------",((Block*)ptr)->size);
                break;
            case 3:
                print_heap();
                break;
            case 0:
                printf("\tExiting...\n");
                break;
            default:
                printf("\tInvalid choice!\n");
        }
    } while (choice != 0);

    return 0;
}
