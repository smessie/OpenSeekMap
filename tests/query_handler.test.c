//
// Created by Ieben Smessaert on 14/11/2020.
//

#include "query_handler.test.h"

#include <stdio.h>
#include <string.h>
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
    u8_toucs(temp_de, 3, "de", -1);
    uint32_t* temp_de_sterre = (uint32_t*) malloc(10 * sizeof(uint32_t));
    u8_toucs(temp_de_sterre, 10, "de sterre", -1);
    uint32_t* temp_sterre = (uint32_t*) malloc(7 * sizeof(uint32_t));
    u8_toucs(temp_sterre, 7, "sterre", -1);
    uint32_t* temp_gent = (uint32_t*) malloc(5 * sizeof(uint32_t));
    u8_toucs(temp_gent, 5, "gent", -1);
    uint32_t* temp_sterre_gent = (uint32_t*) malloc(12 * sizeof(uint32_t));
    u8_toucs(temp_sterre_gent, 12, "sterre gent", -1);
    uint32_t* temp_de_sterre_gent = (uint32_t*) malloc(15 * sizeof(uint32_t));
    u8_toucs(temp_de_sterre_gent, 15, "de sterre gent", -1);


    QueryBreakdownCollection* collection = create_query_breakdown_collection("de sterre gent");
    QueryBreakdown* breakdown = collection->head;
    while (breakdown != NULL) {
        if (breakdown->head == NULL) {
            continue;
        }
        count++;
        if (breakdown->head->length == 2 && equal_value(breakdown->head->value, temp_de, 2) == true) {
            if (breakdown->head->next != NULL) {
                if (breakdown->head->next->length == 6 && equal_value(breakdown->head->next->value, temp_sterre, 6) == true) {
                    if (breakdown->head->next->next != NULL && breakdown->head->next->next->length == 4 && equal_value(breakdown->head->next->next->value, temp_gent, 4) == true) {
                        if (breakdown->head->next->next->next == NULL) {
                            de__sterre__gent++;
                        }
                    }
                } else if (breakdown->head->next->length == 11 && equal_value(breakdown->head->next->value, temp_sterre_gent, 11) == true) {
                    if (breakdown->head->next->next == NULL) {
                        de__sterre_gent++;
                    }
                }
            }
        } else if (breakdown->head->length == 9 && equal_value(breakdown->head->value, temp_de_sterre, 9) == true) {
            if (breakdown->head->next != NULL && breakdown->head->next->length == 4 && equal_value(breakdown->head->next->value, temp_gent, 4) == true) {
                if (breakdown->head->next->next == NULL) {
                    de_sterre__gent++;
                }
            }
        } else if (breakdown->head->length == 14 && equal_value(breakdown->head->value, temp_de_sterre_gent, 14) == true) {
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

bool equal_value(uint32_t* got, uint32_t* expected, int length) {
    for (int i = 0; i < length; i++) {
        if (got[i] != expected[i]) {
            return false;
        }
    }
    return true;
}