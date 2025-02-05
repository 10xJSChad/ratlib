#pragma once
#include <stdlib.h>
#include "rl-global.h"


typedef struct RL_String {
    char* str;
    size_t len;
    size_t size; /* total allocated size */
} String;


String* String_Create(const char* str);
String* String_Duplicate(String* str);
void    String_Destroy(String* str);
Result  String_Append(String* str, const char* append);
Result  String_Insert(String* str, size_t idx, const char* insert);
void    String_Clear(String* str);
Result  String_Erase(String* str, size_t idx, size_t len);
Result  String_Replace(String* str, const char* find, const char* replace);
Result  String_ReplaceAll(String* str, const char* find, const char* replace);
long    String_Find(String* str, const char* find);
