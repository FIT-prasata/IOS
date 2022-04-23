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
#include <unistd.h>
#include <time.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/types.h>

// TYPEDEFS, STRUCTS AND DEFINES
typedef struct args {
    int NO;
    int NH;
    int TI;
    int TB;
} args_t;

#define ERROR 1
#define SUCCESS 0

// FUNCTION PROTOTYPES
bool process_input(int argc, const char **argv, args_t *args);
bool sem_ctor();
bool shm_ctor();
bool sem_dtor();
bool shm_dtor();

// SHARED MEMORY VARIABLES 
int shared_A, shared_idO, shared_idH;

// VARIABLES
int *A, *idO, *idH;

// SEMAPHORES
sem_t *hydro_sem, *oxy_sem, *mutex_sem, *barrier_sem;

#endif