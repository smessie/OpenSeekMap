//
// Created by Ieben Smessaert on 15/10/2020.
//

#include "database.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strings/string_util.h"

#define LINE_SIZE 200

Database* load_database(char* path) {
    // Open file
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        perror("Error while opening the database file.\n");
        exit(EXIT_FAILURE);
    }

    // Read contents of file
    Database* database = (Database*) malloc(sizeof(Database));
    database->head = NULL;

    char line[LINE_SIZE];
    while (fgets(line, sizeof(line), fp) != NULL) {
        Entry* entry = malloc(sizeof(Entry));
        if (entry == NULL) {
            perror("Failed to malloc entry.\n");
            continue;
        }
        entry->next = NULL;
        entry->id = strtoull(strtok(line, "\t"), NULL, 10);

        char* name = strtok(NULL, "\t");
        entry->name = (char*) malloc((strlen(name) + 1) * sizeof(char));
        if (entry->name == NULL) {
            perror("Failed to malloc entry name.\n");
            free(entry);
            continue;
        }
        strcpy(entry->name, name);

        char* normalized = normalize_string(name);
        entry->normalized = (char*) malloc((strlen(normalized) + 1) * sizeof(char));
        if (entry->normalized == NULL) {
            perror("Failed to malloc entry normalized name.\n");
            free(entry->name);
            free(entry);
            continue;
        }
        strcpy(entry->normalized, normalized);

        entry->rank = atoi(strtok(NULL, "\t"));
        entry->longitude = strtod(strtok(NULL, "\t"), NULL);
        entry->latitude = strtod(strtok(NULL, "\t"), NULL);

        add_entry(database, entry);
    }

    // Close file
    fclose(fp);

    return database;
}

void add_entry(Database* database, Entry* entry) {
    // TODO: check if having a pointer to the latest entry (tail) improves performance.
    if (database->head != NULL) {
        Entry* parent = database->head;
        while (parent->next != NULL) {
            parent = parent->next;
        }
        parent->next = entry;
    } else {
        database->head = entry;
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
        printf("%i. [%llu] %s (%s), (%f;%f)\n", i, entry->id, entry->name, entry->normalized, entry->longitude, entry->latitude);
        i++;
        entry = entry->next;
    }
}