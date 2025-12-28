#include "string_utils.h"
#include <ctype.h>
#include <string.h>

bool str_equal_ignore_case(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            return false;
        }
        s1++;
        s2++;
    }
    return *s1 == '\0' && *s2 == '\0';
}

void str_to_lower(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}