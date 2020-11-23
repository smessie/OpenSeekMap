//
// Created by Ieben Smessaert on 14/11/2020.
//

#ifndef AD3_PROJECT_QUERY_HANDLER_TEST_H
#define AD3_PROJECT_QUERY_HANDLER_TEST_H

#include <stdbool.h>
#include <stdint.h>

void run_query_handler_tests();

void test_create_query_breakdown_collection(int* passed, int* failed);

bool equal_value(uint32_t* got, uint32_t* expected, int length);

#endif //AD3_PROJECT_QUERY_HANDLER_TEST_H
