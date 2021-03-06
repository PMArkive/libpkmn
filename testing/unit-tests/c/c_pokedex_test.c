/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"
#include "pkmntest-c/util.h"

#include <pkmn-c.h>

#include <stdlib.h>
#include <time.h>

static void pokedex_test(enum pkmn_game game)
{
    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_pokedex pokedex =
    {
        .game = PKMN_GAME_NONE,
        .p_internal = NULL
    };

    int generation = game_to_generation(game);

    error = pkmn_pokedex_init(
                game,
                &pokedex
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(game, pokedex.game);
    TEST_ASSERT_NOT_NULL(pokedex.p_internal);

    // Check initial values.

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_pokedex_strerror(&pokedex)
    );

    size_t num_seen = 0;
    error = pkmn_pokedex_get_num_seen(
                &pokedex,
                &num_seen
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(0, num_seen);

    size_t num_caught = 0;
    error = pkmn_pokedex_get_num_caught(
                &pokedex,
                &num_caught
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(0, num_caught);

    // Set random Pokémon to be seen and caught.
    struct pkmn_species_enum_list all_pokemon =
    {
        .p_enums = NULL,
        .length = 0
    };
    error = pkmn_database_pokemon_list(generation, true, &all_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(all_pokemon.p_enums);

    num_seen = (size_t)((rand() % (int)all_pokemon.length/2 + 1));
    num_caught = (size_t)((rand() % (int)all_pokemon.length/2 + 1));

    for(size_t pokemon_index = 0; pokemon_index < num_seen; ++pokemon_index)
    {
        error = pkmn_pokedex_set_has_seen_species(
                    &pokedex,
                    all_pokemon.p_enums[pokemon_index],
                    true
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        bool has_seen = false;
        error = pkmn_pokedex_has_seen(
                    &pokedex,
                    all_pokemon.p_enums[pokemon_index],
                    &has_seen
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_TRUE(has_seen);
    }

    size_t num_seen_after = 0;
    error = pkmn_pokedex_get_num_seen(
                &pokedex,
                &num_seen_after
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(num_seen, num_seen_after);

    for(size_t pokemon_index = (all_pokemon.length-num_caught); pokemon_index < all_pokemon.length; ++pokemon_index)
    {
        error = pkmn_pokedex_set_has_caught_species(
                    &pokedex,
                    all_pokemon.p_enums[pokemon_index],
                    true
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        bool has_caught = false;
        error = pkmn_pokedex_has_caught(
                    &pokedex,
                    all_pokemon.p_enums[pokemon_index],
                    &has_caught
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_TRUE(has_caught);
    }

    size_t num_caught_after = 0;
    error = pkmn_pokedex_get_num_caught(
                &pokedex,
                &num_caught_after
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(num_caught, num_caught_after);

    // Remove all caught.

    struct pkmn_species_enum_list all_caught =
    {
        .p_enums = NULL,
        .length = 0
    };
    error = pkmn_pokedex_get_all_caught(
                &pokedex,
                &all_caught
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(all_caught.p_enums);

    for(size_t pokemon_index = 0; pokemon_index < all_caught.length; ++pokemon_index)
    {
        error = pkmn_pokedex_set_has_caught_species(
                    &pokedex,
                    all_caught.p_enums[pokemon_index],
                    false
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        bool has_caught = true;
        error = pkmn_pokedex_has_caught(
                    &pokedex,
                    all_caught.p_enums[pokemon_index],
                    &has_caught
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_FALSE(has_caught);
    }

    num_caught_after = 0;
    error = pkmn_pokedex_get_num_caught(
                &pokedex,
                &num_caught_after
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(0, num_caught_after);

    error = pkmn_species_enum_list_free(&all_caught);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(all_caught.p_enums);

    // Remove all seen.

    struct pkmn_species_enum_list all_seen =
    {
        .p_enums = NULL,
        .length = 0
    };
    error = pkmn_pokedex_get_all_seen(
                &pokedex,
                &all_seen
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(all_seen.p_enums);

    for(size_t pokemon_index = 0; pokemon_index < all_seen.length; ++pokemon_index)
    {
        error = pkmn_pokedex_set_has_seen_species(
                    &pokedex,
                    all_seen.p_enums[pokemon_index],
                    false
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        bool has_seen = true;
        error = pkmn_pokedex_has_seen(
                    &pokedex,
                    all_seen.p_enums[pokemon_index],
                    &has_seen
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_FALSE(has_seen);
    }

    num_seen_after = 0;
    error = pkmn_pokedex_get_num_seen(
                &pokedex,
                &num_seen_after
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(0, num_seen_after);

    error = pkmn_species_enum_list_free(&all_seen);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(all_seen.p_enums);

    error = pkmn_species_enum_list_free(&all_pokemon);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(all_pokemon.p_enums);

    error = pkmn_pokedex_free(&pokedex);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(pokedex.p_internal);
}

#define POKEDEX_TEST_FCN(game_enum, game) \
static void game ## _test_pokedex() \
{ \
    pokedex_test(game_enum); \
}

POKEDEX_TEST_FCN(PKMN_GAME_RED, Red)
POKEDEX_TEST_FCN(PKMN_GAME_BLUE, Blue)
POKEDEX_TEST_FCN(PKMN_GAME_YELLOW, Yellow)

POKEDEX_TEST_FCN(PKMN_GAME_GOLD, Gold)
POKEDEX_TEST_FCN(PKMN_GAME_SILVER, Silver)
POKEDEX_TEST_FCN(PKMN_GAME_CRYSTAL, Crystal)

POKEDEX_TEST_FCN(PKMN_GAME_RUBY, Ruby)
POKEDEX_TEST_FCN(PKMN_GAME_SAPPHIRE, Sapphire)
POKEDEX_TEST_FCN(PKMN_GAME_EMERALD, Emerald)
POKEDEX_TEST_FCN(PKMN_GAME_FIRERED, FireRed)
POKEDEX_TEST_FCN(PKMN_GAME_LEAFGREEN, LeafGreen)

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(Red_test_pokedex)
    PKMN_C_TEST(Blue_test_pokedex)
    PKMN_C_TEST(Yellow_test_pokedex)

    PKMN_C_TEST(Gold_test_pokedex)
    PKMN_C_TEST(Silver_test_pokedex)
    PKMN_C_TEST(Crystal_test_pokedex)

    PKMN_C_TEST(Ruby_test_pokedex)
    PKMN_C_TEST(Sapphire_test_pokedex)
    PKMN_C_TEST(Emerald_test_pokedex)
    PKMN_C_TEST(FireRed_test_pokedex)
    PKMN_C_TEST(LeafGreen_test_pokedex)
)
