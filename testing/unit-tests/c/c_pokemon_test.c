/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_test_common.h"

#include <pkmntest-c/gen1_pokemon_tests.h>
#include <pkmntest-c/gen2_pokemon_tests.h>
#include <pkmntest-c/gba_pokemon_tests.h>
#include <pkmntest-c/pokemon_io_tests.h>
#include <pkmntest-c/pokemon_tests_common.h>

#include <pkmn.h>

typedef void (*pokemon_test_fcn_t)(pkmn_pokemon_handle_t, const char*, const char*);

static void pkmntest_pokemon_test_common(
    const char* species,
    const char* game,
    pokemon_test_fcn_t test_fcn
) {
    pkmn_error_t error = PKMN_ERROR_NONE;
    pkmn_pokemon_handle_t pokemon = NULL;

    error = pkmn_pokemon_make(
                &pokemon,
                species,
                game,
                "",
                30
            );
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NOT_NULL(pokemon);

    test_fcn(
        pokemon,
        species,
        game
    );

    error = pkmn_pokemon_free(&pokemon);
    TEST_ASSERT_EQUAL(error, PKMN_ERROR_NONE);
    TEST_ASSERT_NULL(pokemon);
}

// Red

static void gen1_red_invalid_pokemon_test() {
    pkmntest_invalid_pokemon_test("Red");
}

static void gen1_red_friendship_test() {
    pkmntest_gen1_friendship_test("Red");
}

static void gen1_red_pokemon_test() {
    pkmntest_pokemon_test_common(
        "Charmander",
        "Red",
        pkmntest_gen1_pokemon_test
    );
}

// Blue

static void gen1_blue_invalid_pokemon_test() {
    pkmntest_invalid_pokemon_test("Blue");
}

static void gen1_blue_friendship_test() {
    pkmntest_gen1_friendship_test("Blue");
}

static void gen1_blue_pokemon_test() {
    pkmntest_pokemon_test_common(
        "Charmander",
        "Blue",
        pkmntest_gen1_pokemon_test
    );
}

// Yellow

static void gen1_yellow_invalid_pokemon_test() {
    pkmntest_invalid_pokemon_test("Yellow");
}

static void gen1_yellow_friendship_test() {
    pkmntest_gen1_friendship_test("Yellow");
}

static void gen1_yellow_pokemon_test() {
    pkmntest_pokemon_test_common(
        "Charmander",
        "Yellow",
        pkmntest_gen1_pokemon_test
    );
}

// Gold

static void gen2_gold_invalid_pokemon_test() {
    pkmntest_invalid_pokemon_test("Gold");
}

static void gen2_gold_gender_test() {
    pkmntest_gender_test("Gold");
}

static void gen2_gold_unown_test() {
    pkmntest_gen2_unown_test("Gold");
}

static void gen2_gold_pokemon_test() {
    pkmntest_pokemon_test_common(
        "Cyndaquil",
        "Gold",
        pkmntest_gen2_pokemon_test
    );
}

// Silver

static void gen2_silver_invalid_pokemon_test() {
    pkmntest_invalid_pokemon_test("Silver");
}

static void gen2_silver_gender_test() {
    pkmntest_gender_test("Silver");
}

static void gen2_silver_unown_test() {
    pkmntest_gen2_unown_test("Silver");
}

static void gen2_silver_pokemon_test() {
    pkmntest_pokemon_test_common(
        "Cyndaquil",
        "Silver",
        pkmntest_gen2_pokemon_test
    );
}

// Crystal

static void gen2_crystal_invalid_pokemon_test() {
    pkmntest_invalid_pokemon_test("Crystal");
}

static void gen2_crystal_gender_test() {
    pkmntest_gender_test("Crystal");
}

static void gen2_crystal_unown_test() {
    pkmntest_gen2_unown_test("Crystal");
}

static void gen2_crystal_pokemon_test() {
    pkmntest_pokemon_test_common(
        "Cyndaquil",
        "Crystal",
        pkmntest_gen2_pokemon_test
    );
}

// Ruby

static void gba_ruby_invalid_pokemon_test() {
    pkmntest_invalid_pokemon_test("Ruby");
}

static void gba_ruby_gender_test() {
    pkmntest_gender_test("Ruby");
}

static void gba_ruby_unown_test() {
    pkmntest_gba_unown_test("Ruby");
}

static void gba_ruby_pokemon_test() {
    pkmntest_pokemon_test_common(
        "Torchic",
        "Ruby",
        pkmntest_gba_pokemon_test
    );
}

// Sapphire

static void gba_sapphire_invalid_pokemon_test() {
    pkmntest_invalid_pokemon_test("Sapphire");
}

static void gba_sapphire_gender_test() {
    pkmntest_gender_test("Sapphire");
}

static void gba_sapphire_unown_test() {
    pkmntest_gba_unown_test("Sapphire");
}

static void gba_sapphire_pokemon_test() {
    pkmntest_pokemon_test_common(
        "Torchic",
        "Sapphire",
        pkmntest_gba_pokemon_test
    );
}

// Emerald

static void gba_emerald_invalid_pokemon_test() {
    pkmntest_invalid_pokemon_test("Emerald");
}

static void gba_emerald_gender_test() {
    pkmntest_gender_test("Emerald");
}

static void gba_emerald_unown_test() {
    pkmntest_gba_unown_test("Emerald");
}

static void gba_emerald_pokemon_test() {
    pkmntest_pokemon_test_common(
        "Torchic",
        "Emerald",
        pkmntest_gba_pokemon_test
    );
}

// FireRed

static void gba_firered_invalid_pokemon_test() {
    pkmntest_invalid_pokemon_test("FireRed");
}

static void gba_firered_gender_test() {
    pkmntest_gender_test("FireRed");
}

static void gba_firered_unown_test() {
    pkmntest_gba_unown_test("FireRed");
}

static void gba_firered_pokemon_test() {
    pkmntest_pokemon_test_common(
        "Torchic",
        "FireRed",
        pkmntest_gba_pokemon_test
    );
}

// LeafGreen

static void gba_leafgreen_invalid_pokemon_test() {
    pkmntest_invalid_pokemon_test("LeafGreen");
}

static void gba_leafgreen_gender_test() {
    pkmntest_gender_test("LeafGreen");
}

static void gba_leafgreen_unown_test() {
    pkmntest_gba_unown_test("LeafGreen");
}

static void gba_leafgreen_pokemon_test() {
    pkmntest_pokemon_test_common(
        "Torchic",
        "LeafGreen",
        pkmntest_gba_pokemon_test
    );
}

// Other
static void _3gpkm_test() {
    pkmntest_test_3gpkm();
}

PKMN_C_TEST_MAIN(
    PKMN_C_TEST(gen1_red_invalid_pokemon_test)
    PKMN_C_TEST(gen1_red_friendship_test)
    PKMN_C_TEST(gen1_red_pokemon_test)

    PKMN_C_TEST(gen1_blue_invalid_pokemon_test)
    PKMN_C_TEST(gen1_blue_friendship_test)
    PKMN_C_TEST(gen1_blue_pokemon_test)

    PKMN_C_TEST(gen1_yellow_invalid_pokemon_test)
    PKMN_C_TEST(gen1_yellow_friendship_test)
    PKMN_C_TEST(gen1_yellow_pokemon_test)

    PKMN_C_TEST(gen2_gold_invalid_pokemon_test)
    PKMN_C_TEST(gen2_gold_gender_test)
    PKMN_C_TEST(gen2_gold_unown_test)
    PKMN_C_TEST(gen2_gold_pokemon_test)

    PKMN_C_TEST(gen2_silver_invalid_pokemon_test)
    PKMN_C_TEST(gen2_silver_gender_test)
    PKMN_C_TEST(gen2_silver_unown_test)
    PKMN_C_TEST(gen2_silver_pokemon_test)

    PKMN_C_TEST(gen2_crystal_invalid_pokemon_test)
    PKMN_C_TEST(gen2_crystal_gender_test)
    PKMN_C_TEST(gen2_crystal_unown_test)
    PKMN_C_TEST(gen2_crystal_pokemon_test)

    PKMN_C_TEST(gba_ruby_invalid_pokemon_test)
    PKMN_C_TEST(gba_ruby_gender_test)
    PKMN_C_TEST(gba_ruby_unown_test)
    PKMN_C_TEST(gba_ruby_pokemon_test)

    PKMN_C_TEST(gba_sapphire_invalid_pokemon_test)
    PKMN_C_TEST(gba_sapphire_gender_test)
    PKMN_C_TEST(gba_sapphire_unown_test)
    PKMN_C_TEST(gba_sapphire_pokemon_test)

    PKMN_C_TEST(gba_emerald_invalid_pokemon_test)
    PKMN_C_TEST(gba_emerald_gender_test)
    PKMN_C_TEST(gba_emerald_unown_test)
    PKMN_C_TEST(gba_emerald_pokemon_test)

    PKMN_C_TEST(gba_firered_invalid_pokemon_test)
    PKMN_C_TEST(gba_firered_gender_test)
    PKMN_C_TEST(gba_firered_unown_test)
    PKMN_C_TEST(gba_firered_pokemon_test)

    PKMN_C_TEST(gba_leafgreen_invalid_pokemon_test)
    PKMN_C_TEST(gba_leafgreen_gender_test)
    PKMN_C_TEST(gba_leafgreen_unown_test)
    PKMN_C_TEST(gba_leafgreen_pokemon_test)

    PKMN_C_TEST(_3gpkm_test)
)
