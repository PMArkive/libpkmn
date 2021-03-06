/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef PKMN_C_ENUMS_BALL_H
#define PKMN_C_ENUMS_BALL_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <stdlib.h>

enum pkmn_ball
{
    PKMN_BALL_NONE = 0,
    PKMN_BALL_MASTER_BALL = 1,
    PKMN_BALL_ULTRA_BALL = 2,
    PKMN_BALL_GREAT_BALL = 3,
    PKMN_BALL_POKE_BALL = 4,
    PKMN_BALL_SAFARI_BALL = 5,
    PKMN_BALL_NET_BALL = 6,
    PKMN_BALL_DIVE_BALL = 7,
    PKMN_BALL_NEST_BALL = 8,
    PKMN_BALL_REPEAT_BALL = 9,
    PKMN_BALL_TIMER_BALL = 10,
    PKMN_BALL_LUXURY_BALL = 11,
    PKMN_BALL_PREMIER_BALL = 12,
    PKMN_BALL_DUSK_BALL = 13,
    PKMN_BALL_HEAL_BALL = 14,
    PKMN_BALL_QUICK_BALL = 15,
    PKMN_BALL_CHERISH_BALL = 16,
    PKMN_BALL_FAST_BALL = 17,
    PKMN_BALL_LEVEL_BALL = 18,
    PKMN_BALL_LURE_BALL = 19,
    PKMN_BALL_HEAVY_BALL = 20,
    PKMN_BALL_LOVE_BALL = 21,
    PKMN_BALL_FRIEND_BALL = 22,
    PKMN_BALL_MOON_BALL = 23,
    PKMN_BALL_SPORT_BALL = 24,
    PKMN_BALL_PARK_BALL = 25,
    PKMN_BALL_DREAM_BALL = 26,
    PKMN_BALL_INVALID = 30000,
};

struct pkmn_ball_enum_list
{
    enum pkmn_ball* p_enums;
    size_t length;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_ball_enum_list_free(
    struct pkmn_ball_enum_list* p_ball_enum_list
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ENUMS_BALL_H */
