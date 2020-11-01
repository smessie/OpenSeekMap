//
// Created by Ieben Smessaert on 27/10/2020.
//

#ifndef AD3_PROJECT_ALGORITHM_H
#define AD3_PROJECT_ALGORITHM_H

#include <stdbool.h>

typedef struct characteristic_vector_t {
    char key;
    int* value;
    struct characteristic_vector_t* next;
} characteristic_vector;

typedef struct characteristic_vectors_t {
    characteristic_vector* head;
    int size;
    int* zeros;
} characteristic_vectors;

bool shiftAND(char* z, char* t);

/**
 * Generate a linked list of all characteristic vectors for which the letter is in the search string.
 * @param z - The search string for which the CVs should be calculated.
 * @param size - Size of the search string.
 * @return a pointer to a struct containing a pointer to the linked list.
 */
characteristic_vectors* calculate_characteristic_vectors(char* z, int size);

/**
 * Find the characteristic vector in the struct holding it.
 * @param cvs - Struct which contains all characteristic vectors.
 * @param x - Character for which we want the characteristic vector.
 * @return the characteristic vector or all zeros if not contained.
 */
int* C(characteristic_vectors* cvs, char x);

void free_characteristic_vectors(characteristic_vectors* cvs);

#endif //AD3_PROJECT_ALGORITHM_H
