//
// Created by Ieben Smessaert on 16/11/2020.
//

#ifndef AD3_PROJECT_SORT_UTIL_H
#define AD3_PROJECT_SORT_UTIL_H

#include "query_handler.h"

void calculate_best_matches(BestMatches* best_matches, TotalMatchCollection* collection);

void calculate_best_matches_geo(BestMatches* best_matches, TotalMatchCollection* collection, double latitude,
                                double longitude);

void replace_with_if_better(BestMatches* best_matches, TotalMatch* match, double value);

void replace_with_if_better_helper(TotalMatch** to_insert_or_drop_match, double* to_insert_or_drop_value,
                                   TotalMatch** best_match, double* best_value);

Match* get_max_rank(TotalMatch* total_match);

#endif //AD3_PROJECT_SORT_UTIL_H
