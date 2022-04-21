// Name: proj2.c
// IOS Project 2 Solution
// Start date: 21.4.2022
// Author: Lukáš Zavadil
// Faculty: FIT
// Compiled: gcc (GCC) 9.2.0
// Git repository: https://github.com/lukaszavadil1/IOS

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

int main(int argc, const char **argv) {
    args_t args;
    FILE *file;
    pid_t init, oxy, hydro;
    pid_t oxy_childs[args.NO], hydro_childs[args.NH];

    // Arguments validation
    if (process_input(argc, argv, &args) == false) {exit(1); }

    // File validation
    if ((file = fopen("proj2.exe", "w")) == NULL) {
        fprintf(stderr, "Chyba prace se souborem");
        exit(1);
    }

    // Magic
    init = fork();
    if (init == 0) {
        for (size_t i = 0; i < args.NO; i++) {
            // TODO
            oxy = fork();
        }
    }
    else if (init > 0) {
        for (size_t j = 0; j < args.NH; j++) {
            // TODO
            hydro = fork();
        }
    }
    else {
        fprintf(stderr, "Chyba init forku");
        fclose(file);
        exit(1);
    }
    fclose(file);
    return 0;
}