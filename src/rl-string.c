#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../include/rl-error.h"
#include "../include/rl-util.h"
#include "../include/rl-string.h"


#ifndef RL_OVERALLOCATE
#define RL_OVERALLOCATE 0
#endif


String* String_Create(const char* str) {
    String* new_str = rl_malloc(sizeof *new_str);
    if (new_str == NULL)
        return NULL;

    size_t len = strlen(str);
    size_t size = len + 1;

    new_str->str = rl_malloc(size);
    if (new_str->str == NULL) {
        free(new_str); /* avoid leakage */
        return NULL;
    }

    new_str->len = len;
    new_str->size = size;
    memcpy(new_str->str, str, len + 1);

    return new_str;
}


String* String_Duplicate(String* str) {
    String* new_str = rl_malloc(sizeof *new_str);
    if (new_str == NULL)
        return NULL;

    new_str->len = str->len;
    new_str->size = str->size;
    new_str->str = rl_malloc(str->size);
    if (new_str->str == NULL) {
        free(new_str); /* avoid leakage */
        return NULL;
    }


    memcpy(new_str->str, str->str, str->len + 1);
    return new_str;
}


void String_Destroy(String* str) {
    free(str->str);
    free(str);
}


Result String_Append(String* str, const char* append) {
    size_t append_len = strlen(append);

    if (str->len + append_len + 1 > str->size) {
        size_t new_size = str->len + append_len + 1;

        /* use more memory but avoid frequent reallocations,
           ideal if performance is a priority */
        if (RL_OVERALLOCATE)
            new_size *= 2;

        char* new_str = rl_realloc(str->str, new_size);
        if (new_str == NULL)
            return RESULT_ERR(RL_ERR_ALLOC_FAIL);

        str->str = new_str;
        str->size = new_size;
    }

    memcpy(str->str + str->len, append, append_len + 1);
    str->len += append_len;
    return RESULT_OK(0);
}


Result String_Insert(String* str, size_t idx, const char* insert) {
    size_t insert_len = strlen(insert);
    size_t new_len = str->len + insert_len;
    size_t new_size = new_len + 1;

    if (RL_OVERALLOCATE)
        new_size *= 2;

    char* new_str = rl_malloc(new_size);
    if (new_str == NULL)
        return RESULT_ERR(RL_ERR_ALLOC_FAIL);

    memcpy(new_str, str->str, idx);
    memcpy(new_str + idx, insert, insert_len);
    memcpy(new_str + idx + insert_len, str->str + idx, str->len - idx + 1);

    free(str->str);
    str->str = new_str;
    str->len = new_len;
    str->size = new_size;

    return RESULT_OK(0);
}


void String_Clear(String* str) {
    /* setting str[0] to '\0' is probably fine but the function says
       clear, so we're going to clear. */
    memset(str->str, 0, str->size);
    str->len = 0;
}


Result String_Erase(String* str, size_t idx, size_t len) {
    if (idx >= str->len) {
        rl_error("String_Erase: index out of bounds");
        return RESULT_ERR(RL_ERR_OUT_OF_BOUNDS);
    }

    if (len == 0 || (idx + len > str->len))
        len = str->len - idx;

    memmove(str->str + idx, str->str + idx + len, str->len - idx - len + 1);
    str->len -= len;
    return RESULT_OK(0);
}


long String_Find(String* str, const char* find) {
    char* ptr = strstr(str->str, find);

    if (ptr == NULL)
        return -1;

    return ptr - str->str;
}


Result String_Replace(String* str, const char* find, const char* replace) {
    long idx = String_Find(str, find);
    if (idx == -1)
        return RESULT_OK(-1);

    size_t find_len = strlen(find);
    String_Erase(str, idx, find_len);

    Result result = String_Insert(str, idx, replace);
    if (result.err_code != RL_ERR_OK)
        return result;

    return RESULT_OK(idx);
}


Result String_ReplaceAll(String* str, const char* find, const char* replace) {
    int replaced = 0;

    while (1) {
        Result result = String_Replace(str, find, replace);

        if (result.err_code != RL_ERR_OK)
            return RESULT_ERR(result.err_code);

        if (result.value == -1)
            break;

        ++replaced;
    }

    return RESULT_OK(replaced);
}


