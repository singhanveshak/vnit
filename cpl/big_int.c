#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define INT_SIZE 32
#define NUM_WORDS 32

typedef struct {
    uint32_t data[NUM_WORDS];
} huge;

/* numbers are stored in little endian format meaning that 0xabcdef is stored as

____    ____    ____    .....
 ef      cd      ab   
____    ____    ____    .....

---->increasing mem address        

*/

void show(const huge *num);
void add(const huge *a, const huge *b, huge *result) ;
void sub(const huge *a, const huge *b, huge *result);

int main() {
    huge a, b, sum, difference, product;

    // INITIALIZING OUR ARRAYS SETTING EACH DIGIT AS 0
    memset(&a, 0, sizeof(huge));
    memset(&b, 0, sizeof(huge));
    memset(&product, 0, sizeof(huge));
    a.data[0] = 0x92fabb7e;
    b.data[0] = 0x2eebd119;

    printf("\nnum1: ");
    show(&a);

    printf("\nnum2: ");
    show(&b);    

    // Addition
    add(&a, &b, &sum);
    printf("\nSum: ");
    show(&sum);

    // Subtraction
    sub(&a, &b, &difference);
    printf("\nDifference: ");
    show(&difference);

    return 0;
}

void show(const huge *num) {
    for (int i = NUM_WORDS - 1; i >= 0; i--) {
        if(!(num->data[i]==0))
            printf("%x", num->data[i]);
    }
    printf("\n");
}

void add(const huge *a, const huge *b, huge *result) {
    uint32_t carry = 0;
    for (int i = 0; i < NUM_WORDS; i++) {
        uint64_t sum = (uint64_t)a->data[i] + b->data[i] + carry;
        result->data[i] = (uint32_t)sum;
        carry = (uint32_t)(sum >> INT_SIZE);
    }
}

void sub(const huge *a, const huge *b, huge *result) {
    uint32_t borrow = 0;
    for (int i = 0; i < NUM_WORDS; i++) {
        // printf("\ni=%d, a-b-borrow=%x-%x-%x",i,a->data[i],b->data[i],borrow);
        uint64_t diff = (uint64_t)a->data[i] - b->data[i] - borrow;
        result->data[i] = (uint32_t)diff;
        borrow = (diff >> (sizeof(uint32_t) * 8)) & 1;
    }
}
