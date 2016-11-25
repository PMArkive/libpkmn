/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_ERROR_H
#define PKMN_C_ERROR_H

#include <pkmn-c/config.h>

typedef enum {
    PKMN_ERROR_NONE = 0,

    PKMN_ERROR_INVALID_ARGUMENT = 10,
    PKMN_ERROR_DOMAIN_ERROR = 11,
    PKMN_ERROR_LENGTH_ERROR = 12,
    PKMN_ERROR_OUT_OF_RANGE = 13,
    PKMN_ERROR_LOGIC_ERROR = 19,

    PKMN_ERROR_RANGE_ERROR = 20,
    PKMN_ERROR_OVERFLOW_ERROR = 21,
    PKMN_ERROR_UNDERFLOW_ERROR = 22,
    PKMN_ERROR_RUNTIME_ERROR = 29,

    PKMN_ERROR_STD_EXCEPTION = 50,

    PKMN_ERROR_UNKNOWN_ERROR = 60
} pkmn_error_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_API const char* pkmn_strerror();

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ERROR_H */