/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ENUMS_TYPE_H
#define PKMN_C_ENUMS_TYPE_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

enum pkmn_type
{
    PKMN_TYPE_NONE = 0,
    PKMN_TYPE_NORMAL = 1,
    PKMN_TYPE_FIGHTING = 2,
    PKMN_TYPE_FLYING = 3,
    PKMN_TYPE_POISON = 4,
    PKMN_TYPE_GROUND = 5,
    PKMN_TYPE_ROCK = 6,
    PKMN_TYPE_BUG = 7,
    PKMN_TYPE_GHOST = 8,
    PKMN_TYPE_STEEL = 9,
    PKMN_TYPE_FIRE = 10,
    PKMN_TYPE_WATER = 11,
    PKMN_TYPE_GRASS = 12,
    PKMN_TYPE_ELECTRIC = 13,
    PKMN_TYPE_PSYCHIC = 14,
    PKMN_TYPE_ICE = 15,
    PKMN_TYPE_DRAGON = 16,
    PKMN_TYPE_DARK = 17,
    PKMN_TYPE_FAIRY = 18,
    PKMN_TYPE_QUESTION_MARK = 10001,
    PKMN_TYPE_SHADOW = 10002,
    PKMN_TYPE_INVALID = 30000,
};

struct pkmn_type_enum_list
{
    enum pkmn_type* p_enums;
    size_t length;
};

struct pkmn_type_enum_pair
{
    enum pkmn_type first;
    enum pkmn_type second;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_type_enum_list_free(
    struct pkmn_type_enum_list* p_type_enum_list
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ENUMS_TYPE_H */
