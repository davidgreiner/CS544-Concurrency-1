//
//  main.c
//  CS544 Concurrency 1
//
//  Created by David Greiner on 10/9/17.
//  Copyright © 2017 David Greiner. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>

typedef struct Item {
    int value;
    int waiting_period;
}Item;

typedef struct FIFO {
    int front, rear, size;
    unsigned capacity;
    int* array;
}FIFO;

FIFO* create_FIFO(unsigned capacity) {
    FIFO* fifo = (FIFO*) malloc(sizeof(FIFO));
    fifo->capacity = capacity;
    fifo->front = fifo->size = 0;
    fifo->rear = capacity - 1;
    fifo->array = (int*) malloc(fifo->capacity * sizeof(int));
    return fifo;
}

int is_full(FIFO* fifo) {
    return (fifo->size == fifo->capacity);
}

int is_empty(FIFO* fifo) {
    return (fifo->size == 0);
}

void enqueue(FIFO* fifo, int item)
{
    if(is_full(fifo))
        return;
    fifo->rear = (fifo->rear + 1) % fifo->capacity;
    fifo->array[fifo->rear] = item;
    fifo->size = fifo->size + 1;
}

int dequeue(FIFO* fifo)
{
    if(is_empty(fifo))
        return -1;
    int item = fifo->array[fifo->front];
    fifo->front = (fifo->front + 1) % fifo->capacity;
    fifo->size = fifo->size - 1;
    return item;
}

int rdrand16_step (uint32_t *rand)
{
    unsigned char ok;

    asm volatile ("rdrand %0; setc %1"
                  : "=r" (*rand), "=qm" (ok));

    return (int) ok;
}

int get_random_int_in_range(int start, int end) {
    uint32_t number;
    rdrand16_step(&number);
    return number % (end - start + 1) + start;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    printf("Hello, World!\n");
    return 0;
}
