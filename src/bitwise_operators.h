//
// Created by Ieben Smessaert on 29/10/2020.
//

#ifndef AD3_PROJECT_BITWISE_OPERATORS_H
#define AD3_PROJECT_BITWISE_OPERATORS_H

#include <stdbool.h>

void AND(int value1[], int value2[], int dest[], int size);

bool equal(int bv1[], int bv2[], int size);

/**
 * bit(i) = 1 << i
 * Makes sure there is a 1 at position i.
 * @param arr - The array that should be written to.
 * @param i - Position of the one-bit.
 * @param size - Size of the array that was passed.
 * @param reset - Whether or not all other bits has to be reset to zero. Can be false if you pass a new array.
 */
void bit(int arr[], int i, int size, bool reset);

/**
 * Shift the bitvector array and set the new bit equal to inserted_value.
 * @param bitvector - The array to be shifted.
 * @param size - The size of the bitvector array.
 * @param inserted_value - Value to be inserted at the new position (size-1). 0 or 1.
 */
void shift(int bitvector[], int size, int inserted_value);

#endif //AD3_PROJECT_BITWISE_OPERATORS_H
