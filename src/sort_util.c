//
// Created by Ieben Smessaert on 16/11/2020.
//

#include "sort_util.h"

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "haversine_formula.h"
#include "synergy.h"

void calculate_best_matches(BestMatches* best_matches, TotalMatchCollection* collection) {
    TotalMatch* total_match = collection->head;
    while (total_match != NULL) {
        Match* max = get_max_rank(total_match);
        total_match->representative = max;

        double max_rank = max->value->rank / 30.0;

        double value = 0.2 * total_match->correctness + 0.7 * total_match->synergy + 0.1 * max_rank;
        replace_with_if_better(best_matches, total_match, value);

        total_match = total_match->next;
    }
}

void calculate_best_matches_geo(BestMatches* best_matches, TotalMatchCollection* collection, double latitude,
                                double longitude) {
    TotalMatch* total_match = collection->head;
    while (total_match != NULL) {
        Match* max = get_max_rank(total_match);
        total_match->representative = max;

        double distance = d_geo(max->value->latitude, max->value->longitude, latitude, longitude);
        double extent = get_extent(max->value->rank);
        double dist_value = 1 - (log(distance / extent) / log(1000));
        if (dist_value < 0) {
            dist_value = 0;
        } else if (dist_value > 1) {
            dist_value = 1;
        }
        double max_rank = max->value->rank / 30.0;

        double value = 0.15 * total_match->correctness + 0.55 * total_match->synergy + 0.15 * max_rank +
                       0.15 * dist_value;
        replace_with_if_better(best_matches, total_match, value);

        total_match = total_match->next;
    }
}

void replace_with_if_better(BestMatches* best_matches, TotalMatch* match, double value) {
    TotalMatch* to_insert_or_drop_match = match;
    match->selected_as_best = true;
    double to_insert_or_drop_value = value;
    replace_with_if_better_helper(&to_insert_or_drop_match, &to_insert_or_drop_value, &(best_matches->match_1),
                                  &(best_matches->value_1));
    replace_with_if_better_helper(&to_insert_or_drop_match, &to_insert_or_drop_value, &(best_matches->match_2),
                                  &(best_matches->value_2));
    replace_with_if_better_helper(&to_insert_or_drop_match, &to_insert_or_drop_value, &(best_matches->match_3),
                                  &(best_matches->value_3));
    replace_with_if_better_helper(&to_insert_or_drop_match, &to_insert_or_drop_value, &(best_matches->match_4),
                                  &(best_matches->value_4));
    replace_with_if_better_helper(&to_insert_or_drop_match, &to_insert_or_drop_value, &(best_matches->match_5),
                                  &(best_matches->value_5));
    if (to_insert_or_drop_match != NULL) {
        to_insert_or_drop_match->selected_as_best = false;
        if (to_insert_or_drop_match->parent_collection_freed == true) {
            free_total_match(to_insert_or_drop_match);
        }
    }
}

void replace_with_if_better_helper(TotalMatch** to_insert_or_drop_match, double* to_insert_or_drop_value,
                                   TotalMatch** best_match, double* best_value) {
    if (*to_insert_or_drop_value > *best_value) {
        TotalMatch* temp_match = *best_match;
        double temp_value = *best_value;
        *best_match = *to_insert_or_drop_match;
        *best_value = *to_insert_or_drop_value;
        *to_insert_or_drop_match = temp_match;
        *to_insert_or_drop_value = temp_value;
    }
}

Match* get_max_rank(TotalMatch* total_match) {
    Match* max = NULL;
    Match* match = total_match->head;
    while (match != NULL) {
        if (max == NULL || match->value->rank > max->value->rank) {
            max = match;
        }

        match = match->next;
    }
    return max;
}