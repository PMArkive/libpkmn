/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_TYPES_MOVE_SLOT_H
#define PKMN_C_TYPES_MOVE_SLOT_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/enums/move.h>

#include <stdlib.h>

/*!
 * @brief A struct representing a slot in an move list.
 */
struct pkmn_move_slot
{
    enum pkmn_move move;
    int pp;
};

/*!
 * @brief A list of move slots.
 *
 * This struct's memory is allocated by pkmn_pokemon_get_moves and must
 * be freed with ::pkmn_move_slots_free.
 */
struct pkmn_move_slots
{
    //! The list of move slots, dynamically allocated.
    struct pkmn_move_slot* p_move_slots;
    //! The number of moves in the list.
    size_t length;
};

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @brief Frees the dynamically allocated memory in a pkmn_move_slots_t.
 *
 * \param move_slots_ptr The move slots to free
 * \returns ::PKMN_ERROR_NONE upon success
 * \returns ::PKMN_ERROR_NULL_POINTER if move_slots_ptr is NULL
 */
PKMN_C_API enum pkmn_error pkmn_move_slots_free(
    struct pkmn_move_slots* p_move_slots
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_TYPES_MOVE_SLOT_H */
