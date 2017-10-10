//
//  main.c
//  CS544 Concurrency 1
//
//  Created by David Greiner on 10/9/17.
//  Copyright © 2017 David Greiner. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

// FIFO Implementation

typedef struct Item {
    int value;
    int waiting_period;
}Item;

typedef struct FIFO {
    int front, rear, size;
    unsigned capacity;
    Item* array;
}FIFO;

FIFO* create_FIFO(unsigned capacity) {
    FIFO* fifo = (FIFO*) malloc(sizeof(FIFO));
    fifo->capacity = capacity;
    fifo->front = fifo->size = 0;
    fifo->rear = capacity - 1;
    fifo->array = (Item*) malloc(fifo->capacity * sizeof(Item));
    return fifo;
}

int is_full(FIFO* fifo) {
    return (fifo->size == fifo->capacity);
}

int is_empty(FIFO* fifo) {
    return (fifo->size == 0);
}

void enqueue(FIFO* fifo, Item item)
{
    if(is_full(fifo))
        return;
    fifo->rear = (fifo->rear + 1) % fifo->capacity;
    fifo->array[fifo->rear] = item;
    fifo->size = fifo->size + 1;
}

Item dequeue(FIFO* fifo)
{
    Item item = fifo->array[fifo->front];
    fifo->front = (fifo->front + 1) % fifo->capacity;
    fifo->size = fifo->size - 1;
    return item;
}

// Random number generator

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

// Consumer-Producer Problem

const int MAX = 1000000000;
pthread_mutex_t the_mutex;
pthread_cond_t consumer_condition, producer_condition;
FIFO* buffer;

void* producer(void *ptr) {
    for (int i = 1; i <= MAX; i++) {
        pthread_mutex_lock(&the_mutex);
        while (is_full(buffer))
            pthread_cond_wait(&producer_condition, &the_mutex);
        int wait = get_random_int_in_range(3,7);
        sleep(wait);
        int value = get_random_int_in_range(0,10);
        int waiting_period = get_random_int_in_range(2,9);
        Item item = { value, waiting_period};
        enqueue(buffer, item);
        pthread_cond_signal(&consumer_condition);
        pthread_mutex_unlock(&the_mutex);
    }
    pthread_exit(0);
}

void* consumer(void *ptr) {
    for (int i = 1; i <= MAX; i++) {
        pthread_mutex_lock(&the_mutex);
        while (is_empty(buffer))
            pthread_cond_wait(&consumer_condition, &the_mutex);
        Item item = dequeue(buffer);
        sleep(item.waiting_period);
        printf("%i\n", item.value);
        pthread_cond_signal(&producer_condition);
        pthread_mutex_unlock(&the_mutex);
    }
    pthread_exit(0);
}

int main(int argc, const char * argv[]) {
    buffer = create_FIFO(32);
    
    pthread_t consumer_thread, producer_thread;

    pthread_mutex_init(&the_mutex, NULL);
    pthread_cond_init(&consumer_condition, NULL);
    pthread_cond_init(&producer_condition, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);
    pthread_create(&producer_thread, NULL, producer, NULL);
    
    pthread_join(consumer_thread, NULL);
    pthread_join(producer_thread, NULL);
    
    pthread_mutex_destroy(&the_mutex);
    pthread_cond_destroy(&consumer_condition);
    pthread_cond_destroy(&producer_condition);
}
