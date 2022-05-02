// Name: proj2.c
// IOS Project 2 Solution
// Start date: 21.4.2022
// Author: Lukáš Zavadil
// Faculty: FIT
// Compiled: gcc (GCC) 9.2.0
// Git repository: https://github.com/lukaszavadil1/IOS

// Little book of semaphores recommended :)

// TOTAL CAFFEINE CONSUMED - 1,08g
// HOPELESS CRYING IN THE SHOWER COUNTER - 5x
// PROCESSES THAT ESCAPED TO THE WILD AND WILL NEVER BE FOUND - way too many
// SEMAPHORES THAT WILL NEVER SIGNAL AGAIN - dozens
// SHARED MEMORY COMPLETELY LOST - a lot
// DEADLOCKS EXPLORED - 8

// LOCAL INCLUDES
#include "proj2.h"

// Handles user input
bool process_input(int argc, const char **argv, args_t *args) {
    char *end_ptr_no = "", \
        *end_ptr_nh = "", \
        *end_ptr_ti = "", \
        *end_ptr_tb = "";

    // Check number of arguments
    if (argc != 5) {
        fprintf(stderr, "Chybny pocet parametru");
        return false;
    }

    // Convert arguments from string to long
    args->NO = strtol(argv[1], &end_ptr_no, 10);
    args->NH = strtol(argv[2], &end_ptr_nh, 10);
    args->TI = strtol(argv[3], &end_ptr_ti, 10);
    args->TB = strtol(argv[4], &end_ptr_tb, 10);

    // Check for assigned ranges
    if ((args->NO <= 0) || \
        (args->NH <= 0) || \
        (args->TI < 0) || \
        (args->TI > 1000) || \
        (args->TB < 0) || \
        (args->TB > 1000) || \
        (strcmp(end_ptr_no, "") != 0) || \
        (strcmp(end_ptr_nh, "") != 0) || \
        (strcmp(end_ptr_ti, "") != 0) || \
        (strcmp(end_ptr_tb, "") != 0)
    ) {
        fprintf(stderr, "Chyba v jednom z parametru");
        return false;
    }

    return true;
}

// Calculates maximum number of possible molecules and remaining atoms 
void count_max_molecules(args_t args) {

    if (args.NO >= 1 && args.NH >= 2) {
        // How many molecules can be created
        if (args.NO > (args.NH / 2)) {
            *max_molecules = args.NH / 2;
        }
        else {
            *max_molecules = args.NO;
        }
    }
    else {
        *max_molecules = 0;
        *is_mergeable = 0;
    }
    // Number of atoms that will not be merged
    *hydro_left = args.NH - (*max_molecules * 2);
    *oxy_left = args.NO - *max_molecules;
}

// Semaphores init
bool sem_ctor() {

    // Allocates memory blocks for semaphores with validation
    if (
        (print_mutex = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0)) == MAP_FAILED || \
        (mutex = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0)) == MAP_FAILED || \
        (hydro_queue = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0)) == MAP_FAILED || \
        (oxy_queue = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0)) == MAP_FAILED || \
        (barrier = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0)) == MAP_FAILED || \
        (barrier_mutex = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0)) == MAP_FAILED || \
        (second_barrier = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0)) == MAP_FAILED || \
        (queue_mutex = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0)) == MAP_FAILED
    ) {
        fprintf(stderr, "Chyba pri mapovani pameti pro semafor");
        return false;
    }

    // Initializes and validates semaphores
    if (
        sem_init(print_mutex, 1, 1) == FAILED || \
        sem_init(mutex, 1, 1) == FAILED || \
        sem_init(hydro_queue, 1, 0) == FAILED || \
        sem_init(oxy_queue, 1, 0) == FAILED || \
        sem_init(barrier, 1, 0) == FAILED || \
        sem_init(barrier_mutex, 1, 1) == FAILED || \
        sem_init(second_barrier, 1, 1) == FAILED || \
        sem_init(queue_mutex, 1, 1) == FAILED
    ) {
        fprintf(stderr, "Chyba pri inicializaci semaforu");
        return false;
    }

    return true;
}

// Shared memory init
bool shm_ctor() {

    // Allocates memory blocks for shared memory
    if (
        (shared_line_counter = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) == FAILED || \
        (shared_idO = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) == FAILED || \
        (shared_idH = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) == FAILED || \
        (shared_noM = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) == FAILED || \
        (shared_atom_counter = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) == FAILED || \
        (shared_is_mergeable = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) == FAILED || \
        (shared_oxy_left = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) == FAILED || \
        (shared_hydro_left = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) == FAILED || \
        (shared_max_molecules = shmget(IPC_PRIVATE, sizeof(int), 0666 | IPC_CREAT | IPC_EXCL)) == FAILED
    ) {
        fprintf(stderr, "Chyba alokace pameti pro sdilene promenne");
        return false;
    }

    // Maps allocated memory to address spaces of processes
    if (
        (line_counter = (int *) shmat(shared_line_counter, NULL, 0)) == NULL || \
        (idO = (int *) shmat(shared_idO, NULL, 0)) == NULL || \
        (idH = (int *) shmat(shared_idH, NULL, 0)) == NULL || \
        (noM = (int *) shmat(shared_noM, NULL, 0)) == NULL || \
        (atom_counter = (int *) shmat(shared_atom_counter, NULL, 0)) == NULL || \
        (is_mergeable = (int *) shmat(shared_is_mergeable, NULL, 0)) == NULL || \
        (oxy_left = (int *) shmat(shared_oxy_left, NULL, 0)) == NULL || \
        (hydro_left = (int *) shmat(shared_hydro_left, NULL, 0)) == NULL || \
        (max_molecules = (int *) shmat(shared_max_molecules, NULL, 0)) == NULL
    ) {
        fprintf(stderr, "Chyba mapovani sdilenych promennych");
        return false;
    }

    // Variables init
    *line_counter = 0;
    *idO = 0;
    *idH = 0;
    *noM = 0;
    *atom_counter = 0;
    *is_mergeable = 1;
    *oxy_left = 0;
    *hydro_left = 0;
    *max_molecules = 0;

    return true;
}

// Oxygen process function
void oxy_func(int p_num, args_t args) {

    // Generate random seed
    srand(time(NULL) *getpid());

    // Started state print
    sem_wait(print_mutex);
        fprintf(file, "%d: O %d: started\n", ++(*line_counter), p_num + 1);
        fflush(file);
    sem_post(print_mutex);

    // Sleep before joining queue
    usleep((rand() % (args.TI + 1 - 0) + 0) * 1000);

    // Going to queue state print
    sem_wait(queue_mutex);
        fprintf(file, "%d: O %d: going to queue\n", ++(*line_counter), p_num + 1);
        fflush(file);
    sem_post(queue_mutex);

    // Main shared mutex
    sem_wait(mutex);

    (*idO)++;

    // Check the queue for two H and one O atoms
    if (*idH >= 2 && *idO >= 1) {

        // Release two H atoms from the front of a queue
        sem_post(hydro_queue);
        sem_post(hydro_queue);
        (*idH)--;
        (*idH)--;

        // Release one O atom from the front of a queue
        sem_post(oxy_queue);
        (*idO)--;
    }

    else {

        // Release another atom
        sem_post(mutex);
    }

    // Queue of oxygens
    sem_wait(oxy_queue);

    // Check if new molecule can be created
    if (*is_mergeable == 0) {
        fprintf(file, "%d: O %d: not enough H\n", ++(*line_counter) , p_num + 1);
        fflush(file);
        exit(SUCCESS);
    }

    // Creating molecule state print
    sem_wait(print_mutex);
        fprintf(file, "%d: O %d: creating molecule %d\n", ++(*line_counter), p_num + 1, (*noM)+1);
        fflush(file);
    sem_post(print_mutex);

    // Sleep after creating molecule
    usleep((rand() % (args.TB + 1 - 0) + 0) * 1000);

    // Lock the second and unlock the first barrier
    sem_wait(barrier_mutex);
        (*atom_counter)++;

        if (*atom_counter == 3){
            sem_wait(second_barrier);
            sem_post(barrier);
        }
    sem_post(barrier_mutex);

    // First turnstile
    sem_wait(barrier);
    sem_post(barrier);

    (*noM)++;
    
    //  Lock the first and unlock the second barrier
    sem_wait(barrier_mutex);
        (*atom_counter)--;

        if (*atom_counter == 0){
            sem_wait(barrier);
            sem_post(second_barrier);
        }
    sem_post(barrier_mutex);

    // Second turnstile
    sem_wait(second_barrier);
    sem_post(second_barrier);

    // Print molecule created state
    sem_wait(print_mutex);
        fprintf(file, "%d: O %d: molecule %d created\n", ++(*line_counter), p_num + 1, *noM);
        fflush(file);
    sem_post(print_mutex);

    // Release another atom to the hydro or oxy queue
    sem_post(mutex);

    // Check if all the molecules have been created
    if (*max_molecules == *noM) {

        // Disables further merging of the atoms
        sem_wait(print_mutex);
            *is_mergeable = 0;
        sem_post(print_mutex);
        
        // Releases all remaining oxygens from the queue
        for (int i = 0; i < *(oxy_left); i++) {
            sem_post(oxy_queue);
        }

        // Releases all remaining hydrogens from the queue
        for (int i = 0; i < *(hydro_left); i++) {
            sem_post(hydro_queue);
        }
    }

    exit(SUCCESS);
}

// Hydrogen process function
void hydro_func(int p_num, args_t args) {

    // Generate random seed
    srand(time(NULL) *getpid());

    // Started state print
    sem_wait(print_mutex);
        fprintf(file, "%d: H %d: started\n", ++(*line_counter), p_num + 1);
        fflush(file);
    sem_post(print_mutex);

    // Sleep before joining queue
    usleep((rand() % (args.TI + 1 - 0) + 0) * 1000);

    // Going to queue state print
    sem_wait(queue_mutex);
        fprintf(file, "%d: H %d: going to queue\n", ++(*line_counter), p_num + 1);
        fflush(file);
    sem_post(queue_mutex);

    // Main shared mutex
    sem_wait(mutex);
    
    (*idH)++;

    // Check the queue for two H and one O atoms
    if (*idH >= 2 && *idO >= 1) {

        // Release two H atoms from the front of a queue
        sem_post(hydro_queue);
        sem_post(hydro_queue);
        (*idH)--;
        (*idH)--;

        // Release one O atom from the front of a queue
        sem_post(oxy_queue);
        (*idO)--;
    }

    else {

        // Release another atom
        sem_post(mutex);
    }

    // Queue of hydrogens
    sem_wait(hydro_queue);

    // Check if new molecule can be created
    if (*is_mergeable == 0) {
        fprintf(file, "%d: H %d: not enough O or H\n", ++(*line_counter), p_num + 1);
        fflush(file);
        exit(SUCCESS);
    }

    // Creating molecule state print
        sem_wait(print_mutex);
            fprintf(file, "%d: H %d: creating molecule %d\n", ++(*line_counter), p_num + 1, (*noM)+1);
            fflush(file);
        sem_post(print_mutex);

    // Lock the second and unlock the first barrier
    sem_wait(barrier_mutex);
        (*atom_counter)++;
        
        if (*atom_counter == 3){
            sem_wait(second_barrier);
            sem_post(barrier);
        }
    sem_post(barrier_mutex);

    // First turnstile
    sem_wait(barrier);
    sem_post(barrier);
    
    // Lock the first and unlock the second barrier
    sem_wait(barrier_mutex);
        (*atom_counter)--;

        if (*atom_counter == 0){
            sem_wait(barrier);
            sem_post(second_barrier);
        }
    sem_post(barrier_mutex);

    // Second turnstile
    sem_wait(second_barrier);
    sem_post(second_barrier);

    // Molecule created state print
    sem_wait(print_mutex);
        fprintf(file, "%d: H %d: molecule %d created\n", ++(*line_counter), p_num + 1, *noM);
        fflush(file);
    sem_post(print_mutex);

    exit(SUCCESS);
}

// Semaphores destructor
bool sem_dtor() {

    // Destroys semaphores
    if (
        sem_destroy(print_mutex) == FAILED || \
        sem_destroy(mutex) == FAILED || \
        sem_destroy(hydro_queue) == FAILED || \
        sem_destroy(oxy_queue) == FAILED || \
        sem_destroy(barrier) == FAILED || \
        sem_destroy(barrier_mutex) == FAILED || \
        sem_destroy(second_barrier) == FAILED || \
        sem_destroy(queue_mutex) == FAILED
    ) {
        fprintf(stderr, "Chyba pri uvolnovani semaforu");
        return false;
    }

    return true;
}

// Shared memory destructor
bool shm_dtor() {

    // Destroys and detaches allocated memory segments
    if (
        shmctl(shared_line_counter, IPC_RMID, NULL) == FAILED || \
        shmctl(shared_idO, IPC_RMID, NULL) == FAILED || \
        shmctl(shared_idH, IPC_RMID, NULL) == FAILED || \
        shmctl(shared_noM, IPC_RMID, NULL) == FAILED || \
        shmctl(shared_atom_counter, IPC_RMID, NULL) == FAILED || \
        shmctl(shared_is_mergeable, IPC_RMID, NULL) == FAILED || \
        shmctl(shared_oxy_left, IPC_RMID, NULL) == FAILED || \
        shmctl(shared_hydro_left, IPC_RMID, NULL) == FAILED || \
        shmctl(shared_max_molecules, IPC_RMID, NULL) == FAILED || \
        shmdt(line_counter) == FAILED || \
        shmdt(idO) == FAILED|| \
        shmdt(idH) == FAILED || \
        shmdt(noM) == FAILED || \
        shmdt(atom_counter) == FAILED || \
        shmdt(is_mergeable) == FAILED || \
        shmdt(oxy_left) == FAILED || \
        shmdt(hydro_left) == FAILED || \
        shmdt(max_molecules) == FAILED
    ) {
        fprintf(stderr, "Chyba pri uvolnovani sdilene pameti");
    }

    return true;
}

int main(int argc, const char **argv) {
    args_t args;

    // Arguments validation
    if (process_input(argc, argv, &args) == false) {exit(ERROR); }

    pid_t oxy, hydro;
    pid_t oxy_childs[args.NO], hydro_childs[args.NH];

    // File validation
    if ((file = fopen("proj2.out", "w")) == NULL) {
        fprintf(stderr, "Chyba prace se souborem");
        exit(ERROR);
    }
    setbuf(file, NULL);

    // Semaphores and shared memory initialization
    if ((sem_ctor() == false) || (shm_ctor() == false)) {
        fclose(file);
        exit(ERROR);
    }

    count_max_molecules(args);

    // Magic
    for (int i = 0; i < args.NO; i++) {
        oxy = fork();
        if (oxy == 0) {
            oxy_func(i, args);
        }
        oxy_childs[i] = oxy;
    }

    for (int i = 0; i < args.NH; i++) {
        hydro = fork();
        if (hydro == 0) {
            hydro_func(i, args);
        }
        hydro_childs[i] = hydro;
    }

    // Wait for oxy childs
    for (int i = 0; i < args.NO; i++) {
        waitpid(oxy_childs[i], NULL, 0);
    }

    // Wait for hydro childs
    for (int i = 0; i < args.NH; i++) {
        waitpid(hydro_childs[i], NULL, 0);
    }

    // Cleaning up semaphores and shared memory
    if ((sem_dtor() == false) || (shm_dtor() == false)) { 
        fclose(file);
        exit(ERROR);
    }
    fclose(file);

    return 0;
}