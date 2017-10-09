//
//  main.c
//  CS544 Concurrency 1
//
//  Created by David Greiner on 10/9/17.
//  Copyright © 2017 David Greiner. All rights reserved.
//

#include <stdio.h>

struct item {
    int value;
    int waiting_period;
};

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
