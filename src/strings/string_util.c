//
// Created by Ieben Smessaert on 17/10/2020.
//

#include "string_util.h"
#include <string.h>
#include "accents.h"
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <stddef.h>

char* normalize_string(char* string) {
    char* normalized = string;
    for (int i = 0; i < strlen(string); i++) {
        char normalized_char = (char) remove_accent((wchar_t) string[i]);
        normalized[i] = tolower(normalized_char);
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
    printf("\n");
    Match* part = total_match->head;
    while (part != NULL) {
        printf("%s ", part->value->name);
        part = part->next;
    }
    // Valgrind in docker requires (%lu) instead of (%llu).
    printf("(%lu)\ngeo:%f,%f\n~\n", match->value->id, match->value->latitude, match->value->longitude);
}