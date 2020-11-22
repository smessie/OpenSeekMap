//
// Created by Ieben Smessaert on 14/11/2020.
//

#include "query_handler.test.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "../src/query_handler.h"
#include "../src/strings/utf8.h"

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
    int count = 0;

    uint32_t* temp_de = (uint32_t*) malloc(3 * sizeof(uint32_t));
    u8_toucs(temp_de, 3, "De", -1);
    uint32_t* temp_de_sterre = (uint32_t*) malloc(10 * sizeof(uint32_t));
    u8_toucs(temp_de_sterre, 10, "De Sterre", -1);
    uint32_t* temp_sterre = (uint32_t*) malloc(7 * sizeof(uint32_t));
    u8_toucs(temp_sterre, 7, "Sterre", -1);
    uint32_t* temp_gent = (uint32_t*) malloc(5 * sizeof(uint32_t));
    u8_toucs(temp_gent, 5, "Gent", -1);
    uint32_t* temp_sterre_gent = (uint32_t*) malloc(12 * sizeof(uint32_t));
    u8_toucs(temp_sterre_gent, 12, "Sterre Gent", -1);
    uint32_t* temp_de_sterre_gent = (uint32_t*) malloc(15 * sizeof(uint32_t));
    u8_toucs(temp_de_sterre_gent, 15, "De Sterre Gent", -1);


    QueryBreakdownCollection* collection = create_query_breakdown_collection("De Sterre Gent");
    QueryBreakdown* breakdown = collection->head;
    while (breakdown != NULL) {
        if (breakdown->head == NULL) {
            continue;
        }
        count++;
        if (memcmp(breakdown->head->value, temp_de, 3 * sizeof(uint32_t)) == 0) {
            if (breakdown->head->next != NULL) {
                if (memcmp(breakdown->head->next->value, temp_sterre, 7 * sizeof(uint32_t)) == 0) {
                    if (breakdown->head->next->next != NULL && memcmp(breakdown->head->next->next->value, temp_gent, 5 * sizeof(uint32_t)) == 0) {
                        if (breakdown->head->next->next->next == NULL) {
                            de__sterre__gent++;
                        }
                    }
                } else if (memcmp(breakdown->head->next->value, temp_sterre_gent, 12 * sizeof(uint32_t)) == 0) {
                    if (breakdown->head->next->next == NULL) {
                        de__sterre_gent++;
                    }
                }
            }
        } else if (memcmp(breakdown->head->value, temp_de_sterre, 10 * sizeof(uint32_t)) == 0) {
            if (breakdown->head->next != NULL && memcmp(breakdown->head->next->value, temp_gent, 5 * sizeof(uint32_t)) == 0) {
                if (breakdown->head->next->next == NULL) {
                    de_sterre__gent++;
                }
            }
        } else if (memcmp(breakdown->head->value, temp_de_sterre_gent, 15 * sizeof(uint32_t)) == 0) {
            if (breakdown->head->next == NULL) {
                de_sterre_gent++;
            }
        }
        breakdown = breakdown->next;
    }

    free_query_breakdown_collection(collection);
    free(temp_de);
    free(temp_sterre);
    free(temp_gent);
    free(temp_de_sterre);
    free(temp_sterre_gent);
    free(temp_de_sterre_gent);

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
    if (count != 4) {
        (*failed)++;
        perror("QBC count test failed!");
        return;
    }
    (*passed)++;
}