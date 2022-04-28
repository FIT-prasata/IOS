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
#include <stdarg.h>

// TYPEDEFS, STRUCTS AND DEFINES
typedef struct args {
    long NO;
    long NH;
    long TI;
    long TB;
} args_t;

// EXIT STATES
#define ERROR 1
#define SUCCESS 0
#define FAILED -1

// TESTING
#define UNUSED(x) (void) (x) 

// FUNCTION PROTOTYPES
bool process_input(int argc, const char **argv, args_t *args);
bool sem_ctor();
bool shm_ctor();
void oxy_func(int p_num, args_t args);
void hydro_func(int p_num, args_t args);
bool sem_dtor();
bool shm_dtor();
void count_max_molecules(args_t args);

// FILE POINTER
FILE *file;

// SHARED MEMORY VARIABLES 
int shared_line_counter, \
    shared_idO, \
    shared_idH, \
    shared_noM, \
    shared_atom_counter, \
    shared_is_mergeable, \
    shared_oxy_left, \
    shared_hydro_left, \
    shared_max_molecules;

// VARIABLES
int *line_counter, \
    *idO, \
    *idH, \
    *noM, \
    *atom_counter, \
    *is_mergeable, \
    *oxy_left, \
    *hydro_left, \
    *max_molecules;

// SEMAPHORES
sem_t *print_mutex, \
    *mutex, \
    *hydro_queue, \
    *oxy_queue, \
    *barrier, \
    *barrier_mutex, \
    *second_barrier, \
    *queue_mutex;

#endif