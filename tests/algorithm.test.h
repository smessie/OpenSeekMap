//
// Created by Ieben Smessaert on 31/10/2020.
//

#ifndef AD3_PROJECT_ALGORITHM_TEST_H
#define AD3_PROJECT_ALGORITHM_TEST_H

void run_algorithm_tests();

void test_characteristic_vectors(int* passed, int* failed);

void test_characteristic_vectors_2(int* passed, int* failed);

void test_shiftAND(int* passed, int* failed);

void test_shiftAND_errors(int* passed, int* failed);

void test_M(int* passed, int* failed);

void test_M_i(int* passed, int* failed);

void test_shiftAND_errors_legacy(int* passed, int* failed);

#endif //AD3_PROJECT_ALGORITHM_TEST_H
