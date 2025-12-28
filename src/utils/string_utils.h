#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdbool.h>

// Compara duas strings ignorando maiúsculas/minúsculas
bool str_equal_ignore_case(const char *s1, const char *s2);

// Converte string para minúsculas in-place
void str_to_lower(char *str);

#endif