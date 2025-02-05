#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/rl-util.h"


void rl_error(const char* msg) {
#ifdef RL_EXIT_ON_USER_ERROR
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
#endif
}


void* rl_malloc(size_t size) {
    void* ptr = malloc(size);

#ifdef RL_EXIT_ON_ALLOC_FAIL
    if (ptr == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
#endif

    return ptr;
}


void* rl_realloc(void* ptr, size_t size) {
    void* new_ptr = realloc(ptr, size);

#ifdef RL_EXIT_ON_ALLOC_FAIL
    if (new_ptr == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
#endif

    return new_ptr;
}


void* rl_calloc(size_t size) {
    void* ptr = rl_malloc(size);
    memset(ptr, 0, size);
    return ptr;
}
