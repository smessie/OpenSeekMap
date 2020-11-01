//
// Created by Ieben Smessaert on 29/10/2020.
//

#include "bitwise_operators.h"

#include <stdio.h>

void AND(int value1[], int value2[], int dest[], int size) {
    for (int i = 0; i < size; i++) {
        dest[i] = value1[i] & value2[i];
    }
}

bool equal(int bv1[], int bv2[], int size) {
    for (int i = 0; i < size; i++) {
        if (bv1[i] != bv2[i]) {
            return false;
        }
    }
    return true;
}

void bit(int arr[], int i, int size, bool reset) {
    if (reset) {
        for (int i = 0; i < size; i++) {
            arr[i] = 0;
        }
    }
    arr[size - i - 1] = 1;
}

void shift(int bitvector[], int size, int inserted_value) {
    if (inserted_value != 0 && inserted_value != 1) {
        perror("The inserted_value for the new bit shifted in can only be O or 1!");
        return;
    }
    for (int i = size - 1; i > 0; i--) {
        bitvector[i] = bitvector[i - 1];
    }
    bitvector[0] = inserted_value;
}