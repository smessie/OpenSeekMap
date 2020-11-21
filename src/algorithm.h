//
// Created by Ieben Smessaert on 27/10/2020.
//

#ifndef AD3_PROJECT_ALGORITHM_H
#define AD3_PROJECT_ALGORITHM_H

#include <stdbool.h>
#include <stdint.h>

typedef struct bitvector_t {
    uint32_t key;
    int* value;
    struct bitvector_t* next;
} bitvector;

typedef struct characteristic_vectors_t {
    bitvector* head;
    int size;
    int* zeros;
} characteristic_vectors;

typedef struct M_t {
    bitvector* head;
    int m;
    int n;
} M;

bool shiftAND(char* z, char* t);

/**
 * Checks if a search string is contained in a text with an edit distance of 3 or less.
 * Uses an algorithm based on shiftAND and edit distance.
 * @param z - The search string.
 * @param t - Text to be searched in.
 * @return Cost to find the search string in the text. -1 for infinity (=not found). -1 ≤ cost ≤ 3.
 */
int shiftAND_errors(uint32_t* z, uint32_t* t, int length_z, int length_t);

/**
 * Generate a linked list of all characteristic vectors for which the letter is in the search string.
 * @param z - The search string for which the CVs should be calculated.
 * @param size - Size of the search string.
 * @return a pointer to a struct containing a pointer to the linked list.
 */
characteristic_vectors* calculate_characteristic_vectors(uint32_t* z, int size);

/**
 * Find the characteristic vector in the struct holding it.
 * @param cvs - Struct which contains all characteristic vectors.
 * @param x - Character for which we want the characteristic vector.
 * @return the characteristic vector or all zeros if not contained.
 */
int* C(characteristic_vectors* cvs, uint32_t x);

/**
 * Free the struct and all its children.
 * @param cvs - Pointer to the characteristic vectors struct.
 */
void free_characteristic_vectors(characteristic_vectors* cvs);

/**
 * Generate a linked list of all bitvectors which indicates of a (prefix of a) search string matches a substring of a text.
 * Adjusted so that the whole text should match.
 * @param z - The search string for which the CVs should be calculated.
 * @param t - The text to search in.
 * @param cvs - The characteristic vectors for the search string.
 * @return a pointer to the linked list matrix M.
 */
M* calculate_M(uint32_t* z, uint32_t* t, characteristic_vectors* cvs, int length_z, int length_t);

/**
 * Free the struct and all its children.
 * @param matrix - Pointer to the matrix M struct.
 */
void free_M(M* matrix);

/**
 * Generate a linked list of all bitvectors which indicates of a (prefix of a) search string matches a substring of a text **with errors**.
 * Amount of allowed errors is amount of M_prev + 1.
 *
 * Implementation started from shiftAND with errors but modified so that it not only takes into account replacing a
 * character but also adding or deleting a character and switch two characters from place (TODO).
 * Adjusted so that the whole text should match.
 *
 * @param z - The search string for which the CVs should be calculated.
 * @param t - The text to search in.
 * @param cvs - The characteristic vectors for the search string.
 * @param M_prev - Matrix M which contains the values to start from (1 error less).
 * @return a pointer to the linked list matrix M.
 */
M* calculate_M_i(uint32_t* z, uint32_t* t, characteristic_vectors* cvs, M* M_prev, int length_z, int length_t);

bool check_for_match(M* m);

#endif //AD3_PROJECT_ALGORITHM_H
