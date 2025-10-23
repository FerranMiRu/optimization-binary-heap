#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

#define SAFE_MALLOC(ptr, count)                      \
    do {                                             \
        (ptr) = malloc(sizeof(*(ptr)) * (count));    \
        if ((ptr) == NULL) {                         \
            perror("Error: cannot allocate memory"); \
            exit(EXIT_FAILURE);                      \
        }                                            \
    } while (0)

#endif
