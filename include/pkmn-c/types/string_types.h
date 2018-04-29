/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_STRING_TYPES_H
#define PKMN_C_TYPES_STRING_TYPES_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

struct pkmn_string_list
{
    char** strings;
    size_t length;
};

struct pkmn_string_pair
{
    char* first;
    char* second;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_string_list_free(
    struct pkmn_string_list* string_list_ptr
);

PKMN_C_API enum pkmn_error pkmn_string_pair_free(
    struct pkmn_string_pair* string_pair_ptr
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_STRING_TYPES_H */
