/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_DATABASE_POKEMON_ENTRY_H
#define PKMN_C_DATABASE_POKEMON_ENTRY_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/enums/ability.h>
#include <pkmn-c/enums/egg_group.h>
#include <pkmn-c/enums/game.h>
#include <pkmn-c/enums/species.h>
#include <pkmn-c/enums/stat.h>
#include <pkmn-c/enums/type.h>

#include <pkmn-c/types/levelup_move.h>
#include <pkmn-c/types/string_types.h>

#include <stdbool.h>

struct pkmn_database_pokemon_entry
{
    enum pkmn_species species;
    char* p_species_name;
    enum pkmn_game game;
    char* p_category;
    char* p_form;
    char* p_pokedex_entry;
    float height;
    float weight;
    float chance_male;
    float chance_female;
    bool has_gender_differences;
    int base_friendship;
    struct pkmn_type_enum_pair types;
    struct pkmn_ability_enum_pair abilities;
    enum pkmn_ability hidden_ability;
    struct pkmn_egg_group_enum_pair egg_groups;
    struct pkmn_stat_enum_map base_stats;
    struct pkmn_stat_enum_map EV_yields;
    int experience_yield;
    struct pkmn_levelup_moves levelup_moves;
    struct pkmn_move_enum_list tm_hm_moves;
    struct pkmn_move_enum_list egg_moves;
    struct pkmn_move_enum_list tutor_moves;
    struct pkmn_string_list forms;
    struct pkmn_species_enum_list evolutions;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_database_get_pokemon_entry(
    enum pkmn_species species,
    enum pkmn_game game,
    const char* p_form,
    struct pkmn_database_pokemon_entry* p_pokemon_entry_out
);

PKMN_C_API enum pkmn_error pkmn_database_pokemon_entry_set_form(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    const char* p_form
);

PKMN_C_API enum pkmn_error pkmn_database_pokemon_entry_experience_at_level(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    int level,
    int* p_experience_out
);

PKMN_C_API enum pkmn_error pkmn_database_pokemon_entry_level_at_experience(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    int experience,
    int* p_level_out
);

PKMN_C_API enum pkmn_error pkmn_database_pokemon_entry_icon_filepath(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    bool shiny,
    char* p_icon_filepath_out,
    size_t buffer_len,
    size_t* p_icon_filepath_length_out
);

PKMN_C_API enum pkmn_error pkmn_database_pokemon_entry_sprite_filepath(
    struct pkmn_database_pokemon_entry* p_pokemon_entry,
    bool female,
    bool shiny,
    char* p_sprite_filepath_out,
    size_t buffer_len,
    size_t* p_sprite_filepath_length_out
);

PKMN_C_API enum pkmn_error pkmn_database_pokemon_entry_free(
    struct pkmn_database_pokemon_entry* p_pokemon_entry
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_DATABASE_POKEMON_ENTRY_H */
