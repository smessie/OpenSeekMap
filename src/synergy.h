//
// Created by Ieben Smessaert on 15/11/2020.
//

#ifndef AD3_PROJECT_SYNERGY_H
#define AD3_PROJECT_SYNERGY_H

#include "query_handler.h"

double calculate_synergy(Match* match_1, Match* match_2);

double calculate_total_match_synergy(TotalMatch* total_match);

void calculate_all_total_matches_synergy(TotalMatchCollection* collection);

double get_extent(int rank);

#endif //AD3_PROJECT_SYNERGY_H
