/*
 * p_Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * p_Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * p_or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_GAME_SAVE_H
#define PKMN_C_GAME_SAVE_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/item_bag.h>
#include <pkmn-c/item_list.h>
#include <pkmn-c/pokedex.h>
#include <pkmn-c/pokemon_party.h>
#include <pkmn-c/pokemon_pc.h>

#include <pkmn-c/enums/game.h>
#include <pkmn-c/enums/game_save_type.h>
#include <pkmn-c/enums/gender.h>

#include <pkmn-c/types/attribute_names.h>
#include <pkmn-c/types/time_duration.h>
#include <pkmn-c/types/trainer_info.h>

#include <stdint.h>

struct pkmn_game_save
{
    enum pkmn_game game;

    void* p_internal;
};

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API enum pkmn_error pkmn_game_save_detect_type(
    const char* p_filepath,
    enum pkmn_game_save_type* p_game_save_type_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_init_from_file(
    const char* p_filepath,
    struct pkmn_game_save* p_game_save_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_free(
    struct pkmn_game_save* p_game_save
);

PKMN_C_API const char* pkmn_game_save_strerror(
    const struct pkmn_game_save* p_game_save
);

// Save file actions

PKMN_C_API enum pkmn_error pkmn_game_save_get_filepath(
    const struct pkmn_game_save* p_game_save,
    char* p_filepath_buffer,
    size_t filepath_buffer_length,
    size_t* p_actual_filepath_length_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_save(
    const struct pkmn_game_save* p_game_save
);

PKMN_C_API enum pkmn_error pkmn_game_save_save_as(
    const struct pkmn_game_save* p_game_save,
    const char* p_filepath
);

// Trainer info

PKMN_C_API enum pkmn_error pkmn_game_save_get_trainer_info(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_trainer_info* p_trainer_info_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_trainer_name(
    const struct pkmn_game_save* p_game_save,
    const char* p_trainer_name
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_trainer_id(
    const struct pkmn_game_save* p_game_save,
    uint32_t trainer_id
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_trainer_public_id(
    const struct pkmn_game_save* p_game_save,
    uint16_t trainer_public_id
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_trainer_secret_id(
    const struct pkmn_game_save* p_game_save,
    uint16_t trainer_secret_id
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_trainer_gender(
    const struct pkmn_game_save* p_game_save,
    enum pkmn_gender gender
);

// Other fields

PKMN_C_API enum pkmn_error pkmn_game_save_get_time_played(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_time_duration* p_time_played_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_time_played(
    const struct pkmn_game_save* p_game_save,
    const struct pkmn_time_duration* p_time_played
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_rival_name(
    const struct pkmn_game_save* p_game_save,
    char* p_rival_name_buffer,
    size_t rival_name_buffer_length,
    size_t* p_actual_rival_name_length_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_rival_name(
    const struct pkmn_game_save* p_game_save,
    const char* p_rival_name
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_money(
    const struct pkmn_game_save* p_game_save,
    int* p_money_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_money(
    const struct pkmn_game_save* p_game_save,
    int money
);

// Pokémon and items

PKMN_C_API enum pkmn_error pkmn_game_save_get_pokedex(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_pokedex* p_pokedex_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_pokemon_party(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_pokemon_party* pokemon_party_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_pokemon_pc(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_pokemon_pc* pokemon_pc_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_item_bag(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_item_bag* p_item_bag_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_item_pc(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_item_list* p_item_pc_out
);

// Attributes

PKMN_C_API enum pkmn_error pkmn_game_save_get_numeric_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    int* p_value_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_numeric_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    int value
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_string_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    char* p_value_out,
    size_t value_buffer_len,
    size_t* p_actual_value_len_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_string_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    const char* p_value
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_boolean_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    bool* p_value_out
);

PKMN_C_API enum pkmn_error pkmn_game_save_set_boolean_attribute(
    const struct pkmn_game_save* p_game_save,
    const char* p_attribute_name,
    bool value
);

PKMN_C_API enum pkmn_error pkmn_game_save_get_attribute_names(
    const struct pkmn_game_save* p_game_save,
    struct pkmn_attribute_names* p_attribute_names_out
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_GAME_SAVE_H */
