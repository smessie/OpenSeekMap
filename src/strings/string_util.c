//
// Created by Ieben Smessaert on 17/10/2020.
//

#include "string_util.h"

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <stddef.h>

#include "accents.h"
#include "utf8.h"

uint32_t* normalize_string_utf8(char* string, int length) {
    int i = 0;
    char* malloced_string = (char*) calloc(length + 1, sizeof(uint32_t));
    strcpy(malloced_string, string);

    uint32_t c;
    uint32_t* normalized = (uint32_t*) malloc((length + 1) * sizeof(uint32_t));
    for (int j = 0; j < length; j++) {
        c = u8_nextchar(malloced_string, &i);
        uint32_t normalized_char_int = remove_accent(c);
        normalized[j] = normalized_char_int;
    }
    normalized[length] = '\0';
    free(malloced_string);
    return normalized;
}

char* normalize_string(char* string) {
    char* normalized = string;
    for (int i = 0; i < strlen(string); i++) {
        char normalized_char = (char) remove_accent((wchar_t) string[i]);
        normalized[i] = tolower(normalized_char);
    }
    return normalized;
}

uint32_t* replace_punctuation_marks(uint32_t* string, int length) {
    uint32_t* normalized = (uint32_t*) malloc((length + 1) * sizeof(uint32_t));
    memcpy(normalized, string, (length + 1) * sizeof(uint32_t));
    for (int i = 0; i < length; i++) {
        char next = normalized[i];
        if (next == '?' || next == '!' || next == '.') {
            normalized[i] = ' ';
        }
    }
    return normalized;
}

void bitvector_to_string(int bitvector[], int size, char* dest) {
    for (int i = 0; i < size; i++) {
        dest[i] = bitvector[i] == 1 ? '1' : '0';
    }
    dest[size] = '\0';
}

void print_result(TotalMatch* total_match) {
    if (total_match == NULL) {
        return;
    }
    Match* match = total_match->representative;
    // Valgrind in docker requires (%lu) instead of (%llu).
    printf("%s (%lu)\ngeo:%f,%f\n~\n", match->value->name, match->value->id, match->value->latitude, match->value->longitude);
}