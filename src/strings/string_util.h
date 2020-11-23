//
// Created by Ieben Smessaert on 17/10/2020.
//

#ifndef AD3_PROJECT_STRING_UTIL_H
#define AD3_PROJECT_STRING_UTIL_H

#include "../query_handler.h"

uint32_t* normalize_string_utf8(char* string, int length);

char* normalize_string(char* string);

uint32_t* replace_punctuation_marks(uint32_t* string, int length);

void bitvector_to_string(int bitvector[], int size, char* dest);

void print_result(TotalMatch* total_match);

void print_usage();

#endif //AD3_PROJECT_STRING_UTIL_H
