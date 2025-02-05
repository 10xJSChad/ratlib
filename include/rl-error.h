#pragma once
#include "rl-global.h"


#define RESULT_ERR(err_code) ((Result) {err_code, -1})
#define RESULT_OK(value) ((Result) {0, value})


#define RL_ERR_VAR(name) RL_ERR_ ## name
#define RL_ERROR_ENUMS \
    RL_ERR_VAR(OK) = 0, \
    RL_ERR_VAR(GENERIC), \
    RL_ERR_VAR(ALLOC_FAIL), \
    RL_ERR_VAR(OUT_OF_BOUNDS)


enum {
    RL_ERROR_ENUMS
};


#ifdef RL_SHORT_ERROR_IDENTIFIERS
#undef RL_ERR_VAR
#define RL_ERR_VAR(name) ERR_ ## name
enum {
    RL_ERROR_ENUMS
};
#endif
