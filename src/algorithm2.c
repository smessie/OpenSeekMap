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
        int min_value = 4; // At least one higher than max cost (3).
        for (int j = 1; j <= length_t; j++) {
            int exchange_cost = 1;
            if (z[i - 1] == t[j - 1]) {
                exchange_cost = 0;
            }
            if (z[i - 1] == t[j] && z[i] == t[j - 1]) {
                exchange_cost = 0;
            }
            int cost = min(d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + exchange_cost);
            d[i][j] = cost;
            if (cost < min_value) {
                min_value = cost;
            }
        }
        if (min_value > 3) {
            // If our smallest value is already bigger than our max cost, we'll never find an edit distance smaller than or equal to max cost.
            return -1;
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
