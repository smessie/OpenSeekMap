//
// Created by Ieben Smessaert on 27/10/2020.
//

#include "algorithm.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bitwise_operators.h"
#include "strings/utf8.h"
#include "strings/string_util.h"

bool shiftAND(char* z, char* t) {
    // Initialize variables
    int length_z = u8_strlen(z);
    uint32_t* z_i = normalize_string_utf8(z, length_z);
    int length_t = u8_strlen(t);
    uint32_t* t_i = normalize_string_utf8(t, length_t);

    int* column = calloc(length_z, sizeof(int));
    characteristic_vectors* cvs = calculate_characteristic_vectors(z_i, length_z);

    if (z[0] == t[0]) {
        if (length_z == 1) {
            free(z_i);
            free(t_i);
            free(column);
            free_characteristic_vectors(cvs);
            return true;
        }
        bit(column, length_z - 1, length_z, false);
    }
    // Else: column is already full of zeros

    for (int j = 1; j < length_t; j++) {
        shift(column, length_z, 1);
        AND(column, C(cvs, t_i[j]), column, length_z);

        if (column[length_z - 1] == 1) {
            free(z_i);
            free(t_i);
            free(column);
            free_characteristic_vectors(cvs);
            return true;
        }
    }

    free(z_i);
    free(t_i);
    free(column);
    free_characteristic_vectors(cvs);
    return false;
}

int shiftAND_errors(uint32_t* z, uint32_t* t, int length_z, int length_t) {
    // Initialize variables
    int max_errors = 3;
    characteristic_vectors* cvs = calculate_characteristic_vectors(z, length_z);

    // First calculate M_0 matrix, so without errors.
    M* m = calculate_M(z, t, cvs, length_z, length_t);
    for (int i = 0; i < max_errors; i++) {
        if (check_for_match(m) == true) {
            // Search string found with cost i.
            free_characteristic_vectors(cvs);
            free_M(m);
            return i;
        }
        // Now calculate matrix with one more error = cost +1
        M* m_next = calculate_M_i(z, t, cvs, m, length_z, length_t);
        free_M(m);
        m = m_next;
    }

    if (check_for_match(m) == true) {
        // Search string found with the maximal allowed cost.
        free_characteristic_vectors(cvs);
        free_M(m);
        return max_errors;
    }

    // Cost higher than 3 not allowed so cost is infinity aka -1. Search string not found.
    free_characteristic_vectors(cvs);
    free_M(m);
    return -1;
}

bool check_for_match(M* m) {
    if (m->tail->value[m->m - 1] == 1) {
        // There was a 1 on the last row and column so we found a match.
        return true;
    }
    return false;
}

characteristic_vectors* calculate_characteristic_vectors(uint32_t* z, int size) {
    characteristic_vectors* cvs = (characteristic_vectors*) malloc(sizeof(characteristic_vectors));
    cvs->zeros = (int*) calloc(size, sizeof(int));
    cvs->head = NULL;

    for (int i = 0; i < size; i++) {
        // Character in the search string for which we will make sure its bit in his characteristic vector is set to 1.
        uint32_t x = z[i];

        // Find the characteristic vector which corresponds to the character.
        if (cvs->head == NULL) {
            cvs->head = (bitvector*) malloc(sizeof(bitvector));
            cvs->head->key = x;
            cvs->head->next = NULL;

            // Create new characteristic vector (all zeros) for this character and set 1-bit on this position i.
            cvs->head->value = (int*) calloc(size, sizeof(int));
            cvs->head->value[i] = 1;
        } else {
            bitvector* cv = cvs->head;

            // If key = x we want to stop because we need to adjust this cv,
            // if next is NULL we want to stop because we need to initialise the next-entry.
            while (cv->key != x && cv->next != NULL) {
                cv = cv->next;
            }
            if (cv->key == x) {
                // Set 1-bit on this position i in the characteristic vector.
                cv->value[i] = 1;
            } else {
                cv->next = (bitvector*) malloc(sizeof(bitvector));
                cv->next->key = x;
                cv->next->next = NULL;

                // Create new characteristic vector (all zeros) for this character and set 1-bit on this position i.
                cv->next->value = (int*) calloc(size, sizeof(int));
                cv->next->value[i] = 1;
            }
        }
    }
    return cvs;
}

int* C(characteristic_vectors* cvs, uint32_t x) {
    bitvector* cv = cvs->head;
    while (cv != NULL && cv->key != x) {
        cv = cv->next;
    }
    if (cv == NULL) {
        return cvs->zeros;
    }
    return cv->value;
}

void free_characteristic_vectors(characteristic_vectors* cvs) {
    if (cvs == NULL) {
        return;
    }
    while (cvs->head != NULL) {
        bitvector* toBeRemoved = cvs->head;
        cvs->head = toBeRemoved->next;
        free(toBeRemoved->value);
        free(toBeRemoved);
    }
    free(cvs->zeros);
    free(cvs);
}

M* calculate_M(uint32_t* z, uint32_t* t, characteristic_vectors* cvs, int length_z, int length_t) {
    // Initialize variables and struct
    M* matrix = (M*) malloc(sizeof(M));
    matrix->head = NULL;
    matrix->tail = NULL;
    matrix->m = length_z;
    matrix->n = length_t;
    int* column = calloc(length_z, sizeof(int));

    if (z[0] == t[0]) {
        bit(column, length_z - 1, length_z, false);
    }
    // Else: column is already full of zeros

    // Write back the resulting column to new alloc.
    int* write_back_column = calloc(length_z, sizeof(int));
    for (int i = 0; i < length_z; i++) {
        write_back_column[i] = column[i];
    }
    // Save column to entry in matrix head.
    bitvector* entry = malloc(sizeof(bitvector));
    entry->key = t[0];
    entry->value = write_back_column;
    entry->next = NULL;
    matrix->head = entry;

    for (int j = 1; j < length_t; j++) {
        // This is where the magic happens :tada:
        shift(column, length_z, 0);
        AND(column, C(cvs, t[j]), column, length_z);

        // Write back the resulting column to new alloc.
        int* write_back_column = calloc(length_z, sizeof(int));
        for (int i = 0; i < length_z; i++) {
            write_back_column[i] = column[i];
        }
        // Save column to new entry in previous entry's next.
        bitvector* new_entry = malloc(sizeof(bitvector));
        new_entry->key = t[0];
        new_entry->value = write_back_column;
        new_entry->next = NULL;
        entry->next = new_entry;
        entry = new_entry;
    }
    matrix->tail = entry;
    free(column);

    return matrix;
}

void free_M(M* matrix) {
    if (matrix == NULL) {
        return;
    }
    while (matrix->head != NULL) {
        bitvector* toBeRemoved = matrix->head;
        matrix->head = toBeRemoved->next;
        free(toBeRemoved->value);
        free(toBeRemoved);
    }
    free(matrix);
}

M* calculate_M_i(uint32_t* z, uint32_t* t, characteristic_vectors* cvs, M* M_prev, int length_z, int length_t) {
    // Initialize variables and struct
    M* matrix = (M*) malloc(sizeof(M));
    matrix->head = NULL;
    matrix->m = length_z;
    matrix->n = length_t;

    int* temp_shifting_M_minus_1_column = calloc(length_z, sizeof(int));
    int* temp_M_minus_1_insert_char_column = calloc(length_z, sizeof(int));
    int* temp_M_minus_1_delete_char_column = calloc(length_z, sizeof(int));
    int* temp_M_switch_char_column_minus_1 = calloc(length_z, sizeof(int));
    int* temp_shifting_M_switch_char_column_minus_1 = calloc(length_z, sizeof(int));

    int* column = calloc(length_z, sizeof(int));
    for (int i = 0; i < length_z; i++) {
        column[i] = M_prev->head->value[i];
    }
    shift(column, length_z, 1);

    // Write back the resulting column to new alloc.
    int* write_back_column = calloc(length_z, sizeof(int));
    for (int i = 0; i < length_z; i++) {
        write_back_column[i] = column[i];
    }
    // Save column to entry in matrix head.
    bitvector* entry = malloc(sizeof(bitvector));
    entry->key = t[0];
    entry->value = write_back_column;
    entry->next = NULL;
    matrix->head = entry;

    bitvector* M_minus_1_column = M_prev->head;
    for (int j = 1; j < length_t; j++) {
        for (int i = 0; i < length_z; i++) {
            temp_shifting_M_minus_1_column[i] = M_minus_1_column->value[i];
            temp_M_minus_1_insert_char_column[i] = M_minus_1_column->next->value[i];
            temp_M_minus_1_delete_char_column[i] = M_minus_1_column->value[i];
            temp_M_switch_char_column_minus_1[i] = column[i];
            temp_shifting_M_switch_char_column_minus_1[i] = column[i];
        }
        // This is where the magic happens :tada: *take 2*
        // We are not going to OR with (1,0,...,0) as we already use a SHIFT that inserts a 1.
        shift(column, length_z, 0);
        AND(column, C(cvs, t[j]), column, length_z);
        shift(temp_shifting_M_minus_1_column, length_z, 0);
        shift(temp_M_minus_1_insert_char_column, length_z, 0);
        OR(temp_shifting_M_minus_1_column, column, column, length_z);
        OR(temp_M_minus_1_insert_char_column, column, column, length_z);
        OR(temp_M_minus_1_delete_char_column, column, column, length_z);

        // Switch 2 chars from position: ... OR ( (C[t[j-1]] & shift(M[][j-1])) & shift(C[t[j]] & M[][j-1]))
        shift(temp_shifting_M_switch_char_column_minus_1, length_z, 0);
        AND(C(cvs, t[j-1]), temp_shifting_M_switch_char_column_minus_1, temp_shifting_M_switch_char_column_minus_1, length_z);
        AND(C(cvs, t[j]), temp_M_switch_char_column_minus_1, temp_M_switch_char_column_minus_1, length_z);
        shift(temp_M_switch_char_column_minus_1, length_z, 0);
        AND(temp_shifting_M_switch_char_column_minus_1, temp_M_switch_char_column_minus_1, temp_M_switch_char_column_minus_1, length_z);
        OR(temp_M_switch_char_column_minus_1, column, column, length_z);

        // Write back the resulting column to new alloc.
        int* write_back_column = calloc(length_z, sizeof(int));
        for (int i = 0; i < length_z; i++) {
            write_back_column[i] = column[i];
        }
        // Save column to new entry in previous entry's next.
        bitvector* new_entry = malloc(sizeof(bitvector));
        new_entry->key = t[0];
        new_entry->value = write_back_column;
        new_entry->next = NULL;
        entry->next = new_entry;
        entry = new_entry;

        // Update dependency variables
        M_minus_1_column = M_minus_1_column->next;
    }
    matrix->tail = entry;

    // Clean up our local variables
    free(temp_shifting_M_minus_1_column);
    free(temp_M_minus_1_insert_char_column);
    free(temp_M_minus_1_delete_char_column);
    free(temp_M_switch_char_column_minus_1);
    free(temp_shifting_M_switch_char_column_minus_1);
    free(column);

    return matrix;
}