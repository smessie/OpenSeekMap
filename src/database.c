//
// Created by Ieben Smessaert on 15/10/2020.
//

#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strings/string_util.h"

#define LINE_SIZE 200

Database* load_database(char* path, int max) {
    // Open file
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Error while opening the database file.\n");
        exit(EXIT_FAILURE);
    }

    // Read contents of file
    Database* database = (Database*) malloc(sizeof(Database));
    database->head = NULL;
    database->tail = NULL;

    regex_t regex;
    int reti = regcomp(&regex, "^[0-9]+\t[^\t]+\t[1-3]?[0-9]\t[0-9]+(.[0-9]+)?\t[0-9]+(.[0-9]+)?(\n)?$", REG_EXTENDED);
    if (reti) {
        perror("Could not compile regex.");
        fclose(fp);
        regfree(&regex);
        return database;
    }

    char line[LINE_SIZE];
    while (fgets(line, sizeof(line), fp) != NULL) {
        if (validate_input_entry(line, regex) == false) {
            printf("Following entry in the database was invalid and thereby skipped:\n%s\n", line);
            continue;
        }
        uint64_t id = strtoull(strtok(line, "\t"), NULL, 10);
        char* name = strtok(NULL, "\t");
        int length = strlen(name);

        // If length is longer than longest query length + 3 add errors; entry will never be matched.
        if (length > max + 3) {
            continue;
        }

        Entry* entry = malloc(sizeof(Entry));
        entry->next = NULL;
        entry->id = id;

        entry->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
        strcpy(entry->name, name);

        char* normalized = normalize_string(name);
        entry->normalized = (char*) malloc((strlen(normalized) + 1) * sizeof(char));
        strcpy(entry->normalized, normalized);

        entry->rank = atoi(strtok(NULL, "\t"));
        entry->longitude = strtod(strtok(NULL, "\t"), NULL);
        entry->latitude = strtod(strtok(NULL, "\t"), NULL);

        add_entry(database, entry);
    }

    // Close file and regex buffer
    fclose(fp);
    regfree(&regex);

    return database;
}

void add_entry(Database* database, Entry* entry) {
    if (database->tail != NULL) {
        database->tail->next = entry;
        database->tail = entry;
    } else {
        database->head = entry;
        database->tail = entry;
    }
}

void free_database(Database* database) {
    if (database == NULL) {
        return;
    }
    while (database->head != NULL) {
        Entry* toBeRemoved = database->head;
        database->head = toBeRemoved->next;
        free(toBeRemoved->name);
        free(toBeRemoved->normalized);
        free(toBeRemoved);
    }
    free(database);
}

void print_database(Database* database) {
    if (database == NULL) {
        return;
    }
    int i = 0;
    Entry* entry = database->head;
    while (entry != NULL) {
        // Valgrind in docker requires [%lu] instead of [%llu].
        printf("%i. [%lu] %s (%s), (%f;%f)\n", i, entry->id, entry->name, entry->normalized, entry->longitude, entry->latitude);
        i++;
        entry = entry->next;
    }
}

bool validate_input_entry(char line[], regex_t regex) {
    int reti = regexec(&regex, line, 0, NULL, 0);
    return reti == 0;
}