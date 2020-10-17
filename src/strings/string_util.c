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