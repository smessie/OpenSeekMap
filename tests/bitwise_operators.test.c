//
// Created by Ieben Smessaert on 30/10/2020.
//

#include "bitwise_operators.test.h"

#include "../src/bitwise_operators.h"
#include "../src/strings/string_util.h"

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void run_bitwise_operators_tests() {
    int passed = 0;
    int failed = 0;

    test_AND(&passed, &failed);
    test_bit(&passed, &failed);
    test_shift(&passed, &failed);

    printf("Completed bitwise_operators tests with %d passes and %d failures.\n", passed, failed);
}

void test_AND(int* passed, int* failed) {
    int size = 8;
    int value1[] = {1, 0, 0, 1, 0, 1, 1, 1};
    int value2[] = {1, 0, 1, 0, 0, 1, 0, 1};
    int dest[size];
    AND(value1, value2, dest, size);
    int expected[] = {1, 0, 0, 0, 0, 1, 0, 1};
    if (equal(dest, expected, size) == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("AND test failed!");
    }
}

void test_bit(int* passed, int* failed) {
    int size = 8;
    int bitvector[size];
    char bitvector_string[size];
    bit(bitvector, 5, size, false);
    bitvector_to_string(bitvector, size, bitvector_string);
    if (strcmp(bitvector_string, "00010000")) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("bit(5) test failed!");
    }

    bit(bitvector, 2, size, true);
    bitvector_to_string(bitvector, size, bitvector_string);
    if (strcmp(bitvector_string, "00000100")) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("bit(2) test failed!");
    }
}

void test_shift(int* passed, int* failed) {
    int bitvector[] = {1, 0, 0, 1, 0};
    shift(bitvector, 5, 1);
    char bitvector_string[5];
    bitvector_to_string(bitvector, 5, bitvector_string);
    if (strcmp(bitvector_string, "00101")) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("shift test failed!");
    }
}