//
// Created by Ieben Smessaert on 23/11/2020.
//

#include "algorithm2.test.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "../src/algorithm2.h"
#include "../src/strings/utf8.h"

void run_algorithm2_tests() {
    int passed = 0;
    int failed = 0;

    test_edit_distance(&passed, &failed);

    printf("\nCompleted algorithm2 tests with %d passes and %d failures.\n", passed, failed);
}

void test_edit_distance(int* passed, int* failed) {
    uint32_t* temp = (uint32_t*) malloc(5 * sizeof(uint32_t));
    u8_toucs(temp, 5, "zoek", -1);

    uint32_t* temp2 = (uint32_t*) malloc(6 * sizeof(uint32_t));
    u8_toucs(temp2, 6, "zoeck", -1);

    if (edit_distance(temp, temp, 4, 4) == 0) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("edit_distance test 1 failed!");
    }

    if (edit_distance(temp2, temp, 5, 4) == 1) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("edit_distance test 2 failed!");
    }

    free(temp);
    free(temp2);

    temp = (uint32_t*) malloc(7 * sizeof(uint32_t));
    u8_toucs(temp, 7, "werken", -1);

    temp2 = (uint32_t*) malloc(8 * sizeof(uint32_t));
    u8_toucs(temp2, 8, "gekreun", -1);

    if (edit_distance(temp, temp2, 6, 7) == 3) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("edit_distance test 3 failed!");
    }

    free(temp);

    temp = (uint32_t*) malloc(8 * sizeof(uint32_t));
    u8_toucs(temp, 8, "werkeun", -1);

    if (edit_distance(temp, temp2, 7, 7) == 2) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("edit_distance test 4 failed!");
    }

    free(temp);
    free(temp2);

    temp = (uint32_t*) malloc(4 * sizeof(uint32_t));
    u8_toucs(temp, 4, "ans", -1);

    temp2 = (uint32_t*) malloc(5 * sizeof(uint32_t));
    u8_toucs(temp2, 5, "anan", -1);

    if (edit_distance(temp, temp2, 3, 4) == 2) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("edit_distance test 5 failed!");
    }

    free(temp);
    free(temp2);

    temp = (uint32_t*) malloc(7 * sizeof(uint32_t));
    u8_toucs(temp, 7, "zoeven", -1);

    temp2 = (uint32_t*) malloc(8 * sizeof(uint32_t));
    u8_toucs(temp2, 8, "zoekken", -1);

    if (edit_distance(temp, temp2, 6, 7) == 2) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("edit_distance test 6 failed!");
    }

    free(temp);
    free(temp2);

    temp = (uint32_t*) malloc(10 * sizeof(uint32_t));
    u8_toucs(temp, 10, "huisbazin", -1);

    temp2 = (uint32_t*) malloc(10 * sizeof(uint32_t));
    u8_toucs(temp2, 10, "giusbaizn", -1);

    if (edit_distance(temp, temp2, 9, 9) == 3) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("edit_distance test 7 failed!");
    }

    free(temp);
    free(temp2);

    temp = (uint32_t*) malloc(6 * sizeof(uint32_t));
    u8_toucs(temp, 6, "bruhe", -1);

    temp2 = (uint32_t*) malloc(7 * sizeof(uint32_t));
    u8_toucs(temp2, 7, "brugge", -1);

    if (edit_distance(temp, temp2, 5, 6) == 2) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("edit_distance test 8 failed!");
    }

    free(temp);
    free(temp2);
}