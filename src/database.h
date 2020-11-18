//
// Created by Ieben Smessaert on 15/10/2020.
//

#ifndef AD3_PROJECT_DATABASE_H
#define AD3_PROJECT_DATABASE_H

#include <stdint.h>
#include <stdbool.h>
#include <regex.h>

typedef struct Entry_t {
    uint64_t id;
    char* name;
    char* normalized;
    int rank;
    double longitude;
    double latitude;

    struct Entry_t* next;
} Entry;

typedef struct Database_t {
    Entry* head;
    Entry* tail;
} Database;

Database* load_database(char* path, int max);

void add_entry(Database* database, Entry* entry);

void free_database(Database* database);

void print_database(Database* database);

bool validate_input_entry(char line[], regex_t regex);

#endif //AD3_PROJECT_DATABASE_H
