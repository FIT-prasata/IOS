// Name: proj2.h
// IOS Project 2 Solution
// Start date: 21.4.2022
// Author: Lukáš Zavadil
// Faculty: FIT
// Compiled: gcc (GCC) 9.2.0
// Git repository: https://github.com/lukaszavadil1/IOS

#ifndef HEADER_FILE
#define HEADER_FILE

// EXTERNAL INCLUDES
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>

// TYPEDEFS, STRUCTS AND DEFINES
typedef struct args {
    long NO;
    long NH;
    long TI;
    long TB;
} args_t;

// FUNCTION PROTOTYPES
bool process_input(int argc, const char **argv, args_t *args);

#endif