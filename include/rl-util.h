#pragma once
#include <stdlib.h>
#include <stdio.h>


void* rl_malloc(size_t size);
void* rl_realloc(void* ptr, size_t size);
void* rl_calloc(size_t size);
void  rl_error(const char* msg);
