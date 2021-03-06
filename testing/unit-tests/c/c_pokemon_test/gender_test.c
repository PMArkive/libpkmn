/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <unity.h>

#include <pkmn-c.h>

static const struct pkmn_pokemon empty_pokemon =
{
    .species = PKMN_SPECIES_NONE,
    .game = PKMN_GAME_NONE,
    .p_internal = NULL
};

static void gender_test(enum pkmn_game game)
{
    enum pkmn_error error = PKMN_ERROR_NONE;
    enum pkmn_gender gender = PKMN_GENDER_GENDERLESS;

    // Single-gender
    struct pkmn_pokemon nidorina = empty_pokemon;
    error = pkmn_pokemon_init(
                PKMN_SPECIES_NIDORINA,
                game,
                "",
                50,
                &nidorina
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(nidorina.p_internal);

    error = pkmn_pokemon_get_gender(
                &nidorina,
                &gender
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, gender);

    error = pkmn_pokemon_set_gender(
                &nidorina,
                PKMN_GENDER_FEMALE
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_set_gender(
                &nidorina,
                PKMN_GENDER_MALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_gender(
                &nidorina,
                PKMN_GENDER_GENDERLESS
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_pokemon_free(&nidorina);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(nidorina.p_internal);

    struct pkmn_pokemon nidorino = empty_pokemon;
    error = pkmn_pokemon_init(
                PKMN_SPECIES_NIDORINO,
                game,
                "",
                50,
                &nidorino
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(nidorino.p_internal);

    error = pkmn_pokemon_get_gender(
                &nidorino,
                &gender
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, gender);

    error = pkmn_pokemon_set_gender(
                &nidorino,
                PKMN_GENDER_MALE
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_set_gender(
                &nidorino,
                PKMN_GENDER_FEMALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_gender(
                &nidorino,
                PKMN_GENDER_GENDERLESS
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_pokemon_free(&nidorino);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(nidorino.p_internal);

    struct pkmn_pokemon magnemite = empty_pokemon;
    error = pkmn_pokemon_init(
                PKMN_SPECIES_MAGNEMITE,
                game,
                "",
                50,
                &magnemite
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(magnemite.p_internal);

    error = pkmn_pokemon_get_gender(
                &magnemite,
                &gender
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(PKMN_GENDER_GENDERLESS, gender);

    error = pkmn_pokemon_set_gender(
                &magnemite,
                PKMN_GENDER_GENDERLESS
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_pokemon_set_gender(
                &magnemite,
                PKMN_GENDER_MALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    error = pkmn_pokemon_set_gender(
                &magnemite,
                PKMN_GENDER_FEMALE
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

    error = pkmn_pokemon_free(&magnemite);
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NULL(magnemite.p_internal);

    static const enum pkmn_species mixed_pokemon[] =
    {
        PKMN_SPECIES_CHARMANDER, // 87.% male
        PKMN_SPECIES_GROWLITHE,  // 75% male
        PKMN_SPECIES_PIDGEY,     // 50% male
        PKMN_SPECIES_VULPIX,     // 25% male
        PKMN_SPECIES_NONE
    };

    for(size_t pokemon_index = 0;
        mixed_pokemon[pokemon_index] != PKMN_SPECIES_NONE;
        ++pokemon_index)
    {
        struct pkmn_pokemon pokemon = empty_pokemon;
        error = pkmn_pokemon_init(
                    mixed_pokemon[pokemon_index],
                    game,
                    "",
                    50,
                    &pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_get_gender(
                    &pokemon,
                    &gender
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_EQUAL(PKMN_GENDER_GENDERLESS, gender);

        error = pkmn_pokemon_set_gender(
                    &pokemon,
                    PKMN_GENDER_MALE
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_get_gender(
                    &pokemon,
                    &gender
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(PKMN_GENDER_MALE, gender);

        error = pkmn_pokemon_set_gender(
                    &pokemon,
                    PKMN_GENDER_FEMALE
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        error = pkmn_pokemon_get_gender(
                    &pokemon,
                    &gender
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(PKMN_GENDER_FEMALE, gender);

        error = pkmn_pokemon_set_gender(
                    &pokemon,
                    PKMN_GENDER_GENDERLESS
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(pokemon.p_internal);
    }
}

void gold_gender_test()
{
    gender_test(PKMN_GAME_GOLD);
}

void silver_gender_test()
{
    gender_test(PKMN_GAME_SILVER);
}

void crystal_gender_test()
{
    gender_test(PKMN_GAME_CRYSTAL);
}

void ruby_gender_test()
{
    gender_test(PKMN_GAME_RUBY);
}

void sapphire_gender_test()
{
    gender_test(PKMN_GAME_SAPPHIRE);
}

void emerald_gender_test()
{
    gender_test(PKMN_GAME_EMERALD);
}

void firered_gender_test()
{
    gender_test(PKMN_GAME_FIRERED);
}

void leafgreen_gender_test()
{
    gender_test(PKMN_GAME_LEAFGREEN);
}

void colosseum_gender_test()
{
    gender_test(PKMN_GAME_COLOSSEUM);
}

void xd_gender_test()
{
    gender_test(PKMN_GAME_XD);
}
