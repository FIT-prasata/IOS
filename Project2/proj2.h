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
#include <sys/wait.h>

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
void oxy_func(int p_num, args_t args);
void hydro_func(int p_num, args_t args);
bool sem_dtor();
bool shm_dtor();

// SHARED MEMORY VARIABLES 
int shared_counter, shared_idO, shared_idH, shared_noM;

// VARIABLES
int *counter, *idO, *idH, *noM;

// SEMAPHORES
sem_t *print_mutex, *queue_mutex, *hydro_queue, *oxy_queue;

FILE *file;

#define UNUSED(x) (void) (x) 

#endif