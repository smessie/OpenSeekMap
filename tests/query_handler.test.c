//
// Created by Ieben Smessaert on 14/11/2020.
//

#include "query_handler.test.h"

#include "../src/query_handler.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void run_query_handler_tests() {
    int passed = 0;
    int failed = 0;

    test_create_query_breakdown_collection(&passed, &failed);

    printf("Completed query_handler tests with %d passes and %d failures.\n", passed, failed);
}

void test_create_query_breakdown_collection(int* passed, int* failed) {
    int de_sterre_gent = 0;
    int de_sterre__gent = 0;
    int de__sterre_gent = 0;
    int de__sterre__gent = 0;

    QueryBreakdownCollection* collection = create_query_breakdown_collection("De Sterre Gent");
    QueryBreakdown* breakdown = collection->head;
    while (breakdown != NULL) {
        if (breakdown->head == NULL) {
            continue;
        }
        if (strcmp(breakdown->head->value, "De") == 0) {
            if (breakdown->head->next != NULL) {
                if (strcmp(breakdown->head->next->value, "Sterre Gent") == 0) {
                    if (breakdown->head->next->next == NULL) {
                        de__sterre_gent++;
                    }
                } else if (strcmp(breakdown->head->next->value, "Sterre") == 0) {
                    if (breakdown->head->next->next != NULL && strcmp(breakdown->head->next->next->value, "Gent") == 0) {
                        if (breakdown->head->next->next->next == NULL) {
                            de__sterre__gent++;
                        }
                    }
                }
            }
        } else if (strcmp(breakdown->head->value, "De Sterre") == 0) {
            if (breakdown->head->next != NULL && strcmp(breakdown->head->next->value, "Gent") == 0) {
                if (breakdown->head->next->next == NULL) {
                    de_sterre__gent++;
                }
            }
        } else if (strcmp(breakdown->head->value, "De Sterre Gent") == 0) {
            if (breakdown->head->next == NULL) {
                de_sterre_gent++;
            }
        }
        breakdown = breakdown->next;
    }

    free_query_breakdown_collection(collection);

    if (de_sterre_gent != 1) {
        (*failed)++;
        perror("QBC de_sterre_gent test failed!");
        return;
    }
    if (de_sterre__gent != 1) {
        (*failed)++;
        perror("QBC de_sterre__gent test failed!");
        return;
    }
    if (de__sterre_gent != 1) {
        (*failed)++;
        perror("QBC de__sterre_gent test failed!");
        return;
    }
    if (de__sterre__gent != 1) {
        (*failed)++;
        perror("QBC de__sterre__gent test failed!");
        return;
    }
    (*passed)++;
}