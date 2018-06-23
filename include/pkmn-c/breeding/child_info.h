/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_BREEDING_CHILD_INFO_H
#define PKMN_C_BREEDING_CHILD_INFO_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>
#include <pkmn-c/pokemon.h>

#include <pkmn-c/types/game.h>
#include <pkmn-c/types/gender.h>
#include <pkmn-c/types/string_types.h>

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_breeding_get_possible_child_species(
    const char* p_mother_species,
    const char* p_father_species,
    enum pkmn_game game,
    struct pkmn_string_list* p_possible_child_species_out
);

PKMN_C_API enum pkmn_error pkmn_breeding_get_child_moves(
    const struct pkmn_pokemon* p_mother,
    const struct pkmn_pokemon* p_father,
    const char* p_child_species,
    struct pkmn_string_list* p_child_moves_out
);

PKMN_C_API enum pkmn_error pkmn_breeding_get_ideal_child_IVs(
    const struct pkmn_pokemon* p_mother,
    const struct pkmn_pokemon* p_father,
    enum pkmn_gender gender,
    int* p_IVs_buffer_out,
    size_t IV_buffer_size,
    size_t* p_actual_num_IVs_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_BREEDING_CHILD_INFO_H */
