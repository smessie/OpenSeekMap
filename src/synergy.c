//
// Created by Ieben Smessaert on 15/11/2020.
//

#include "synergy.h"

#include <stdlib.h>

#include "haversine_formula.h"

double calculate_synergy(Match* match_1, Match* match_2) {
    double extent = get_extent(match_1->value->rank);
    double extent_2 = get_extent(match_2->value->rank);
    if (extent_2 > extent) {
        extent = extent_2;
    }
    double synergy = extent / d_geo(match_1->value->latitude, match_1->value->longitude, match_2->value->latitude,
                                 match_2->value->longitude);
    synergy *= synergy;
    if (synergy > 2) {
        synergy = 2;
    }
    return synergy;
}

double calculate_total_match_synergy(TotalMatch* total_match) {
    double sum = 0;
    Match* outer_match = total_match->head;
    while (outer_match != NULL) {
        Match* inner_match = outer_match->next;
        while (inner_match != NULL) {
            sum += calculate_synergy(outer_match, inner_match) / (total_match->n * (total_match->n - 1));

            inner_match = inner_match->next;
        }

        outer_match = outer_match->next;
    }

    return sum;
}

void calculate_all_total_matches_synergy(TotalMatchCollection* collection) {
    TotalMatch* total_match = collection->head;
    while (total_match != NULL) {
        total_match->synergy = calculate_total_match_synergy(total_match);
        total_match = total_match->next;
    }
}

double get_extent(int rank) {
    if (rank < 1 || rank > 30) {
        return 0;
    }
    double extents[] = {100000, 50000, 10000, 1000, 750, 600, 500, 350, 200, 100, 75, 50, 10, 9, 7.5, 6, 4.5, 4, 2, 1,
                        0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2, 0.1, 0.05, 0.01};
    return extents[rank - 1];
}