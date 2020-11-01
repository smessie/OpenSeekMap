//
// Created by Ieben Smessaert on 27/10/2020.
//

#include "algorithm.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bitwise_operators.h"
#include "../src/strings/string_util.h"

bool shiftAND(char* z, char* t) {
    // Initialize variables
    int length_z = strlen(z);
    int* column = calloc(length_z, sizeof(int));
    characteristic_vectors* cvs = calculate_characteristic_vectors(z, length_z);

    if (z[0] == t[0]) {
        if (length_z == 1) {
            return true;
        }
        bit(column, length_z - 1, length_z, false);
    }
    // Else: column is already full of zeros

    for (int j = 1; j < strlen(t); j++) {
        shift(column, length_z, 1);
        AND(column, C(cvs, t[j]), column, length_z);

        if (column[length_z - 1] == 1) {
            return true;
        }
    }

    free(column);
    free_characteristic_vectors(cvs);
    return false;
}

characteristic_vectors* calculate_characteristic_vectors(char* z, int size) {
    characteristic_vectors* cvs = (characteristic_vectors*) malloc(sizeof(characteristic_vectors));
    cvs->zeros = (int*) calloc(size, sizeof(int));
    cvs->head = NULL;

    for (int i = 0; i < size; i++) {
        // Character in the search string for which we will make sure its bit in his characteristic vector is set to 1.
        char x = z[i];

        // Find the characteristic vector which corresponds to the character.
        if (cvs->head == NULL) {
            cvs->head = (characteristic_vector*) malloc(sizeof(characteristic_vector));
            cvs->head->key = x;
            cvs->head->next = NULL;

            // Create new characteristic vector (all zeros) for this character and set 1-bit on this position i.
            cvs->head->value = (int*) calloc(size, sizeof(int));
            cvs->head->value[i] = 1;
        } else {
            characteristic_vector* cv = cvs->head;

            // If key = x we want to stop because we need to adjust this cv,
            // if next is NULL we want to stop because we need to initialise the next-entry.
            while (cv->key != x && cv->next != NULL) {
                cv = cv->next;
            }
            if (cv->key == x) {
                // Set 1-bit on this position i in the characteristic vector.
                cv->value[i] = 1;
            } else {
                cv->next = (characteristic_vector*) malloc(sizeof(characteristic_vector));
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

int* C(characteristic_vectors* cvs, char x) {
    characteristic_vector* cv = cvs->head;
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
        characteristic_vector* toBeRemoved = cvs->head;
        cvs->head = toBeRemoved->next;
        free(toBeRemoved->value);
        free(toBeRemoved);
    }
    free(cvs->zeros);
    free(cvs);
}