//
// Created by Ieben Smessaert on 23/11/2020.
//

#include "algorithm2.h"

int edit_distance(uint32_t* z, uint32_t* t, int length_z, int length_t) {
    int d[length_z + 1][length_t + 1];
    for (int i = 0; i <= length_z; i++) {
        d[i][0] = i;
    }
    for (int j = 0; j <= length_t; j++) {
        d[0][j] = j;
    }
    for (int i = 1; i <= length_z; i++) {
        for (int j = 1; j <= length_t; j++) {
            int exchange_cost = 1;
            if (z[i - 1] == t[j - 1]) {
                exchange_cost = 0;
            }
            if (z[i - 1] == t[j] && z[i] == t[j - 1]) {
                exchange_cost = 0;
            }
            d[i][j] = min(d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + exchange_cost);
        }
    }
    return d[length_z][length_t];
}

int min(int i1, int i2, int i3) {
    int min = i1;
    if (i2 < min) {
        min = i2;
    }
    if (i3 < min) {
        min = i3;
    }
    return min;
}
