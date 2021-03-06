/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_LEVELUP_MOVE_H
#define PKMN_C_TYPES_LEVELUP_MOVE_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/enums/move.h>

#include <stdlib.h>

//! A struct with information on when a Pokémon learns a given move leveling up.
struct pkmn_levelup_move
{
    //! The move.
    enum pkmn_move move;
    //! The level at which the Pokémon learns the move.
    int level;
};

//! A list of levelup moves.
struct pkmn_levelup_moves
{
    //! The level-up moves, dynamically allocated.
    struct pkmn_levelup_move* p_levelup_moves;
    //! The number of level-up moves in the list.
    size_t length;
};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Frees the dynamically-allocated memory in the given level-up move list.
 *
 * \param levelup_moves_ptr A pointer to the level-up move list to free
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if levelup_moves_ptr is NULL
 */
PKMN_C_API enum pkmn_error pkmn_levelup_moves_free(
    struct pkmn_levelup_moves* p_levelup_moves
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_LEVELUP_MOVE_H */
