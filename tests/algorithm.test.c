//
// Created by Ieben Smessaert on 28/10/2020.
//

#include "algorithm.test.h"

#include <stdlib.h>
#include <stdio.h>

#include "../src/algorithm.h"
#include "../src/bitwise_operators.h"
#include "../src/strings/string_util.h"

void run_algorithm_tests() {
    int passed = 0;
    int failed = 0;

    test_characteristic_vectors(&passed, &failed);
    test_characteristic_vectors_2(&passed, &failed);

    test_shiftAND(&passed, &failed);

    test_M(&passed, &failed);

    printf("Completed algorithm tests with %d passes and %d failures.\n", passed, failed);
}

void test_characteristic_vectors(int* passed, int* failed) {
    int size = 3;
    characteristic_vectors* cvs = calculate_characteristic_vectors("ana", size);
    int expected_a[] = {1, 0, 1};
    if (equal(C(cvs, 'a'), expected_a, size) == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("characteristic_vectors char 'a' test failed!");
    }

    int expected_n[] = {0, 1, 0};
    if (equal(C(cvs, 'n'), expected_n, size) == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("characteristic_vectors char 'n' test failed!");
    }

    int expected_x[] = {0, 0, 0};
    if (equal(C(cvs, 'x'), expected_x, size) == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("characteristic_vectors char 'x' test failed!");
    }
    free_characteristic_vectors(cvs);
}

void test_characteristic_vectors_2(int* passed, int* failed) {
    int size = 8;
    characteristic_vectors* cvs = calculate_characteristic_vectors("barbados", size);
    int expected_a[] = {0, 1, 0, 0, 1, 0, 0, 0};
    if (equal(C(cvs, 'a'), expected_a, size) == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("characteristic_vectors 2 char 'a' test failed!");
    }

    int expected_b[] = {1, 0, 0, 1, 0, 0, 0, 0};
    if (equal(C(cvs, 'b'), expected_b, size) == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("characteristic_vectors 2 char 'b' test failed!");
    }

    int expected_x[] = {0, 0, 0, 0, 0, 0, 0, 0};
    if (equal(C(cvs, 'x'), expected_x, size) == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("characteristic_vectors 2 char 'x' test failed!");
    }
    free_characteristic_vectors(cvs);
}

void test_shiftAND(int* passed, int* failed) {
    if (shiftAND("zoek", "mijn zoek string") == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("shiftAND test 1 failed!");
    }

    if (shiftAND("zoeck", "mijn zoek string") == false) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("shiftAND test 2 failed!");
    }

    if (shiftAND("zoeken", "iets_zoets_zoeken") == true) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("shiftAND test 3 failed!");
    }

    if (shiftAND("ans", "ananas") == false) {
        (*passed)++;
    } else {
        (*failed)++;
        perror("shiftAND test 4 failed!");
    }
}

void test_M(int* passed, int* failed) {
    int size = 3;
    characteristic_vectors* cvs = calculate_characteristic_vectors("ana", size);
    M* matrix = calculate_M("ana", "ananas", cvs);
    int expected_ana[6][3] = {
            {1, 0, 0},
            {0, 1, 0},
            {1, 0, 1},
            {0, 1, 0},
            {1, 0, 1},
            {0, 0, 0}
    };
    bitvector* testing_column_ana = matrix->head;
    for (int i = 0; i < matrix->n; i++) {
        if (equal(testing_column_ana->value, expected_ana[i], size) == false) {
            (*failed)++;
            perror("M z=ana, t=ananas test failed!");
            break;
        }
        testing_column_ana = testing_column_ana->next;
    }
    if (testing_column_ana == NULL) {
        (*passed)++;
    }
    free_M(matrix);
    free_characteristic_vectors(cvs);

    size = 4;
    cvs = calculate_characteristic_vectors("bond", size);
    matrix = calculate_M("bond", "bonbond", cvs);
    int expected_bond[7][4] = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, 0},
            {0, 0, 0, 1}
    };
    bitvector* testing_column_bond = matrix->head;
    for (int i = 0; i < matrix->n; i++) {
        if (equal(testing_column_bond->value, expected_bond[i], size) == false) {
            (*failed)++;
            perror("M z=bond, t=bonbond test failed!");
            break;
        }
        testing_column_bond = testing_column_bond->next;
    }
    if (testing_column_bond == NULL) {
        (*passed)++;
    }
    free_M(matrix);
    free_characteristic_vectors(cvs);
}