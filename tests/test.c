//
// Created by Ieben Smessaert on 03/11/2020.
//
#include <stdlib.h>
#include <stdio.h>
#include "algorithm.test.h"
#include "bitwise_operators.test.h"

int main() {
    run_algorithm_tests();
    run_bitwise_operators_tests();

    // End program
    fclose(stdin);
    fclose(stdout);
    fclose(stderr);
    return EXIT_SUCCESS;
}