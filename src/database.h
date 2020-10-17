//
// Created by Ieben Smessaert on 15/10/2020.
//

#ifndef AD3_PROJECT_DATABASE_H
#define AD3_PROJECT_DATABASE_H

#include <stdint.h>

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

Database* load_database(char* path);

void add_entry(Database* database, Entry* entry);

void free_database(Database* database);

void print_database(Database* database);

#endif //AD3_PROJECT_DATABASE_H
