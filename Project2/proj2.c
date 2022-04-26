// Name: proj2.c
// IOS Project 2 Solution
// Start date: 21.4.2022
// Author: Lukáš Zavadil
// Faculty: FIT
// Compiled: gcc (GCC) 9.2.0
// Git repository: https://github.com/lukaszavadil1/IOS

// TOTAL CAFFEINE CONSUMED - 0,64g
// HOPELESS CRYING IN THE SHOWER COUNTER - 4x
// PROCESSES THAT ESCAPED TO THE WILD AND WILL NEVER BE FOUND - way too many
// SEMAPHORES THAT WILL NEVER SIGNAL AGAIN - dozens
// SHARED MEMORY COMPLETELY LOST - a lot
// DEADLOCKS EXPLORED - 2

// LOCAL INCLUDES
#include "proj2.h"

bool process_input(int argc, const char **argv, args_t *args) {
    char *end_ptr_no = "", *end_ptr_nh = "", *end_ptr_ti = "", *end_ptr_tb = "";
    if (argc != 5) {
        fprintf(stderr, "Chybny pocet parametru");
        return false;
    }
    args->NO = strtol(argv[1], &end_ptr_no, 10);
    args->NH = strtol(argv[2], &end_ptr_nh, 10);
    args->TI = strtol(argv[3], &end_ptr_ti, 10);
    args->TB = strtol(argv[4], &end_ptr_tb, 10);
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

bool sem_ctor() {

    // Allocates memory blocks for semaphores with validation
    if (
        (print_mutex = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0)) == MAP_FAILED || \
        (queue_mutex = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0)) == MAP_FAILED || \
        (hydro_queue = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0)) == MAP_FAILED || \
        (oxy_queue = mmap(NULL, sizeof(sem_t), PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, 0, 0)) == MAP_FAILED

    ) {
        fprintf(stderr, "Chyba pri mapovani pameti pro semafor");
        return false;
    }

    // Initializes and validates semaphores
    if (
        sem_init(print_mutex, 1, 1) == -1 || \
        sem_init(queue_mutex, 1, 1) == -1 || \
        sem_init(hydro_queue, 1, 0) == -1 || \
        sem_init(oxy_queue, 1, 0) == -1
    ) {
        fprintf(stderr, "Chyba pri inicializaci semaforu");
        return false;
    }

    return true;
}

bool shm_ctor() {

    // Allocates memory blocks for shared memory
    if (
        (shared_counter = shmget(IPC_PRIVATE, sizeof(int), 0644 | IPC_CREAT | IPC_EXCL)) == -1 || \
        (shared_idO = shmget(IPC_PRIVATE, sizeof(int), 0644 | IPC_CREAT | IPC_EXCL)) == -1 || \
        (shared_idH = shmget(IPC_PRIVATE, sizeof(int), 0644 | IPC_CREAT | IPC_EXCL)) == -1 || \
        (shared_noM = shmget(IPC_PRIVATE, sizeof(int), 0644 | IPC_CREAT | IPC_EXCL)) == -1
    ) {
        fprintf(stderr, "Chyba alokace pameti pro sdilene promenne");
        return false;
    }

    // Maps allocated memory to address spaces of processes
    if (
        (counter = (int *) shmat(shared_counter, NULL, 0)) == NULL || \
        (idO = (int *) shmat(shared_idO, NULL, 0)) == NULL || \
        (idH = (int *) shmat(shared_idH, NULL, 0)) == NULL || \
        (noM = (int *) shmat(shared_noM, NULL, 0)) == NULL
    ) {
        fprintf(stderr, "Chyba mapovani sdilenych promennych");
        return false;
    }

    *counter = 0;
    *idO = 0;
    *idH = 0;
    *noM = 0;

    printf("Counter: %d, idO: %d, idH: %d, noM: %d\n", *counter, *idO, *idH, *noM);

    return true;
}

void oxy_func(int p_num, args_t args) {

    //Process started print
    srand(getpid());
    sem_wait(print_mutex);
    printf("%d: O %d: started\n", ++(*counter), p_num + 1);
    sem_post(print_mutex);

    sem_wait(queue_mutex);
    usleep((rand() % (args.TI + 1 - 0) + 0) * 1000);
    printf("%d: O %d: going to queue\n", ++(*counter), p_num + 1);
    (*idO)++;

    if (*idH >= 2 && *idO >= 1) {

        sem_post(hydro_queue);
        sem_post(hydro_queue);
        (*idH)--;
        (*idH)--;
        sem_post(oxy_queue);
        (*idO)--;

        sem_wait(print_mutex);
        usleep((rand() % (args.TB + 1 - 0) + 0) * 1000);
        printf("%d: O %d: creating molecule %d\n", ++(*counter), p_num + 1, ++(*noM));
        sem_post(print_mutex);
    }

    else {
        sem_post(queue_mutex);
    }

    sem_wait(oxy_queue);
    
    // TODO bond()
}

void hydro_func(int p_num, args_t args) {

    //Process started print
    srand(getpid());
    sem_wait(print_mutex);
    printf("%d: H %d: started\n", ++(*counter), p_num + 1);
    sem_post(print_mutex);

    sem_wait(queue_mutex);
    usleep((rand() % (args.TI + 1 - 0) + 0) * 1000);
    printf("%d: H %d: going to queue\n", ++(*counter), p_num + 1);
    (*idH)++;

    if (*idH >= 2 && *idO >= 1) {

        sem_post(hydro_queue);
        sem_post(hydro_queue);
        (*idH)--;
        (*idH)--;
        sem_post(oxy_queue);
        (*idO)--;

        sem_wait(print_mutex);
        usleep((rand() % (args.TB + 1 - 0) + 0) * 1000);
        printf("%d: H %d: creating molecule %d\n", ++(*counter), p_num + 1, ++(*noM));
        sem_post(print_mutex);
    }

    else {
        sem_post(queue_mutex);
    }

    sem_wait(hydro_queue);
    
    // TODO bond()
}

bool sem_dtor() {

    // Destroys semaphores
    if (
        sem_destroy(print_mutex) == -1 || \
        sem_destroy(queue_mutex) == -1 || \
        sem_destroy(hydro_queue) == -1 || \
        sem_destroy(oxy_queue) == -1
    ) {
        fprintf(stderr, "Chyba pri uvolnovani semaforu");
        return false;
    }

    return true;
}

bool shm_dtor() {

    // Destroys and detaches allocated memory segments
    if (
        shmctl(shared_counter, IPC_RMID, NULL) == -1 || \
        shmctl(shared_idO, IPC_RMID, NULL) == -1 || \
        shmctl(shared_idH, IPC_RMID, NULL) == -1 || \
        shmctl(shared_noM, IPC_RMID, NULL) == -1 || \
        shmdt(counter) == -1 || \
        shmdt(idO) == -1|| \
        shmdt(idH) == -1 || \
        shmdt(noM) == -1
    ) {
        fprintf(stderr, "Chyba pri uvolnovani sdilene pameti");
    }

    return true;
}

int main(int argc, const char **argv) {
    args_t args;

    // Arguments validation
    if (process_input(argc, argv, &args) == false) {exit(ERROR); }

    pid_t init, oxy, hydro;
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

    // Magic
    init = fork();
    if (init == 0) {
        for (int i = 0; i < args.NO; i++) {
            usleep(rand() % (args.TI + 1 - 0) + 0);
            oxy = fork();
            if (oxy == 0) {
                oxy_func(i, args);
                exit(SUCCESS);
            }
            else if (oxy == -1) {
                fprintf(stderr, "Chyba oxy forku");
                fclose(file);
                exit(ERROR);
            }
            else {
                oxy_childs[i] = oxy;
            }
        }
        
        // Wait for oxy childs
        for (int i = 0; i < args.NO; i++) {
            waitpid(oxy_childs[i], NULL, 0);
        }
    }
    else if (init == -1) {
        fprintf(stderr, "Chyba init forku");
        fclose(file);
        exit(ERROR);
    }
    else {
        for (int i = 0; i < args.NH; i++) {
            hydro = fork();
            if (hydro == 0) {
                hydro_func(i, args);
                exit(SUCCESS);
            }
            else if (hydro == -1) {
                fprintf(stderr, "Chyba hydro forku");
                fclose(file);
                exit(ERROR);
            }
            else {
                hydro_childs[i] = hydro;
            }
        }

        // Wait for hydro childs
        for (int i = 0; i < args.NH; i++) {
            waitpid(hydro_childs[i], NULL, 0);
        }
    }

    if ((sem_dtor() == false) || (shm_dtor() == false)) { exit(ERROR); }
    fclose(file);

    return 0;
}