//
// Created by Ieben Smessaert on 17/10/2020.
//

#ifndef AD3_PROJECT_STRING_UTIL_H
#define AD3_PROJECT_STRING_UTIL_H

#include "../query_handler.h"

char* normalize_string(char* string);

void bitvector_to_string(int bitvector[], int size, char* dest);

void print_result(TotalMatch* total_match);

#endif //AD3_PROJECT_STRING_UTIL_H
