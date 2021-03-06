/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <unity.h>

#include <pkmntest-c/util.h>

#include <pkmn-c.h>

#include <string.h>

static inline bool is_game_rs(enum pkmn_game game)
{
    return (game == PKMN_GAME_RUBY) || (game == PKMN_GAME_SAPPHIRE);
}

static inline bool is_game_gamecube(enum pkmn_game game)
{
    return (game == PKMN_GAME_COLOSSEUM) || (game == PKMN_GAME_XD);
}

static inline bool is_game_hgss(enum pkmn_game game)
{
    return (game == PKMN_GAME_HEARTGOLD) || (game == PKMN_GAME_SOULSILVER);
}

static inline bool is_game_oras(enum pkmn_game game)
{
    return (game == PKMN_GAME_OMEGA_RUBY) || (game == PKMN_GAME_ALPHA_SAPPHIRE);
}

static void test_gen1_pokemon_forms(enum pkmn_game game)
{
    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_pokemon pokemon =
    {
        .species = PKMN_SPECIES_NONE,
        .game = PKMN_GAME_NONE,
        .p_internal = NULL
    };

    int generation = game_to_generation(game);

    // Check that Mega forms only work in their given games.
    static const enum pkmn_species gen1_pokemon_with_xy_mega_forms[] =
    {
        PKMN_SPECIES_VENUSAUR,
        PKMN_SPECIES_BLASTOISE,
        PKMN_SPECIES_ALAKAZAM,
        PKMN_SPECIES_GENGAR,
        PKMN_SPECIES_KANGASKHAN,
        PKMN_SPECIES_PINSIR,
        PKMN_SPECIES_GYARADOS,
        PKMN_SPECIES_AERODACTYL,
        PKMN_SPECIES_NONE
    };
    for(size_t pokemon_index = 0;
        gen1_pokemon_with_xy_mega_forms[pokemon_index] != PKMN_SPECIES_NONE;
        ++pokemon_index)
    {
        error = pkmn_pokemon_init(
                    gen1_pokemon_with_xy_mega_forms[pokemon_index],
                    game,
                    "Mega",
                    100,
                    &pokemon
                );
        if(generation >= 6)
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon.p_internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NULL(pokemon.p_internal);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon.p_internal);
        }
    }
    static const enum pkmn_species gen1_pokemon_with_oras_mega_forms[] =
    {
        PKMN_SPECIES_BEEDRILL,
        PKMN_SPECIES_PIDGEOT,
        PKMN_SPECIES_SLOWBRO,
        PKMN_SPECIES_NONE
    };
    for(size_t pokemon_index = 0;
        gen1_pokemon_with_oras_mega_forms[pokemon_index] != PKMN_SPECIES_NONE;
        ++pokemon_index)
    {
        error = pkmn_pokemon_init(
                    gen1_pokemon_with_oras_mega_forms[pokemon_index],
                    game,
                    "Mega",
                    100,
                    &pokemon
                );
        if(is_game_oras(game))
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon.p_internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NULL(pokemon.p_internal);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon.p_internal);
        }
    }

    // Cosplay Pikachu should only work for OR/AS.
    struct pkmn_database_pokemon_entry oras_pikachu_entry;
    error = pkmn_database_get_pokemon_entry(
                PKMN_SPECIES_PIKACHU,
                PKMN_GAME_OMEGA_RUBY,
                "",
                &oras_pikachu_entry
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT(oras_pikachu_entry.forms.length > 0);

    for(size_t form_index = 1; form_index < oras_pikachu_entry.forms.length; ++form_index)
    {
        TEST_ASSERT_NOT_NULL(oras_pikachu_entry.forms.pp_strings[form_index]);

        error = pkmn_pokemon_init(
                    PKMN_SPECIES_PIKACHU,
                    game,
                    oras_pikachu_entry.forms.pp_strings[form_index],
                    5,
                    &pokemon
                );
        if(is_game_oras(game))
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon.p_internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NULL(pokemon.p_internal);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon.p_internal);
        }
    }

    error = pkmn_database_pokemon_entry_free(&oras_pikachu_entry);
    PKMN_TEST_ASSERT_SUCCESS(error);

    static const enum pkmn_species gen1_pokemon_with_two_mega_forms[] =
    {
        PKMN_SPECIES_CHARIZARD,
        PKMN_SPECIES_MEWTWO,
        PKMN_SPECIES_NONE
    };
    for(size_t pokemon_index = 0;
        gen1_pokemon_with_two_mega_forms[pokemon_index] != PKMN_SPECIES_NONE;
        ++pokemon_index)
    {
        error = pkmn_pokemon_init(
                    gen1_pokemon_with_two_mega_forms[pokemon_index],
                    game,
                    "Mega X",
                    50,
                    &pokemon
                );
        if(generation >= 6)
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon.p_internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NULL(pokemon.p_internal);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon.p_internal);
        }

        error = pkmn_pokemon_init(
                    gen1_pokemon_with_two_mega_forms[pokemon_index],
                    game,
                    "Mega Y",
                    50,
                    &pokemon
                );
        if(generation >= 6)
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon.p_internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NULL(pokemon.p_internal);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon.p_internal);
        }
    }
}

static void test_gen2_pokemon_forms(enum pkmn_game game)
{
    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_pokemon pokemon =
    {
        .species = PKMN_SPECIES_NONE,
        .game = PKMN_GAME_NONE,
        .p_internal = NULL
    };

    int generation = game_to_generation(game);

    // Check that Mega forms only work in their given games.
    static const enum pkmn_species gen2_pokemon_with_xy_mega_forms[] =
    {
        PKMN_SPECIES_AMPHAROS,
        PKMN_SPECIES_SCIZOR,
        PKMN_SPECIES_HERACROSS,
        PKMN_SPECIES_HOUNDOOM,
        PKMN_SPECIES_TYRANITAR,
        PKMN_SPECIES_NONE
    };
    for(size_t pokemon_index = 0;
        gen2_pokemon_with_xy_mega_forms[pokemon_index] != PKMN_SPECIES_NONE;
        ++pokemon_index)
    {
        error = pkmn_pokemon_init(
                    gen2_pokemon_with_xy_mega_forms[pokemon_index],
                    game,
                    "Mega",
                    100,
                    &pokemon
                );
        if(generation >= 6)
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon.p_internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NULL(pokemon.p_internal);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
            TEST_ASSERT_NULL(pokemon.p_internal);
        }
    }

    error = pkmn_pokemon_init(
                PKMN_SPECIES_STEELIX,
                game,
                "Mega",
                100,
                &pokemon
            );
    if(is_game_oras(game))
    {
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon.p_internal);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    else
    {
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
    TEST_ASSERT_NULL(pokemon.p_internal);

    // Spiky-eared Pichu should only work in HG/SS.
    error = pkmn_pokemon_init(
                PKMN_SPECIES_PICHU,
                game,
                "Spiky-eared",
                100,
                &pokemon
            );
    if(is_game_hgss(game))
    {
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon.p_internal);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    else
    {
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
    TEST_ASSERT_NULL(pokemon.p_internal);

    // Unown's "!" and "?" forms aren't in Generation II.
    struct pkmn_database_pokemon_entry unown_entry;
    error = pkmn_database_get_pokemon_entry(
                PKMN_SPECIES_UNOWN,
                PKMN_GAME_OMEGA_RUBY,
                "",
                &unown_entry
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(28, unown_entry.forms.length);
    for(size_t i = 0; i < 26; ++i)
    {
        TEST_ASSERT_NOT_NULL(unown_entry.forms.pp_strings[i]);
        error = pkmn_pokemon_init(
                    PKMN_SPECIES_UNOWN,
                    game,
                    unown_entry.forms.pp_strings[i],
                    10,
                    &pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon.p_internal);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(pokemon.p_internal);
    }

    error = pkmn_database_pokemon_entry_free(&unown_entry);
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_pokemon_init(
                PKMN_SPECIES_UNOWN,
                game,
                "!",
                10,
                &pokemon
            );
    if(generation >= 3)
    {
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon.p_internal);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    else
    {
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
    TEST_ASSERT_NULL(pokemon.p_internal);

    error = pkmn_pokemon_init(
                PKMN_SPECIES_UNOWN,
                game,
                "?",
                10,
                &pokemon
            );
    if(generation >= 3)
    {
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon.p_internal);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    else
    {
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
    TEST_ASSERT_NULL(pokemon.p_internal);
}

static void test_gen3_pokemon_forms(enum pkmn_game game)
{
    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_pokemon pokemon =
    {
        .species = PKMN_SPECIES_NONE,
        .game = PKMN_GAME_NONE,
        .p_internal = NULL
    };

    int generation = game_to_generation(game);

    // Check that Mega forms only work in their given games.
    static const enum pkmn_species gen3_pokemon_with_xy_mega_forms[] =
    {
        PKMN_SPECIES_BLAZIKEN,
        PKMN_SPECIES_GARDEVOIR,
        PKMN_SPECIES_MAWILE,
        PKMN_SPECIES_AGGRON,
        PKMN_SPECIES_MEDICHAM,
        PKMN_SPECIES_MANECTRIC,
        PKMN_SPECIES_BANETTE,
        PKMN_SPECIES_ABSOL,
        PKMN_SPECIES_LATIAS,
        PKMN_SPECIES_LATIOS,
        PKMN_SPECIES_GARCHOMP,
        PKMN_SPECIES_NONE
    };
    for(size_t pokemon_index = 0;
        gen3_pokemon_with_xy_mega_forms[pokemon_index] != PKMN_SPECIES_NONE;
        ++pokemon_index)
    {
        error = pkmn_pokemon_init(
                    gen3_pokemon_with_xy_mega_forms[pokemon_index],
                    game,
                    "Mega",
                    100,
                    &pokemon
                );
        if(generation >= 6)
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon.p_internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon.p_internal);
    }
    static const enum pkmn_species gen3_pokemon_with_oras_mega_forms[] =
    {
        PKMN_SPECIES_SCEPTILE,
        PKMN_SPECIES_SWAMPERT,
        PKMN_SPECIES_SABLEYE,
        PKMN_SPECIES_SHARPEDO,
        PKMN_SPECIES_CAMERUPT,
        PKMN_SPECIES_ALTARIA,
        PKMN_SPECIES_GLALIE,
        PKMN_SPECIES_SALAMENCE,
        PKMN_SPECIES_METAGROSS,
        PKMN_SPECIES_RAYQUAZA,
        PKMN_SPECIES_NONE
    };
    for(size_t pokemon_index = 0;
        gen3_pokemon_with_oras_mega_forms[pokemon_index] != PKMN_SPECIES_NONE;
        ++pokemon_index)
    {
        error = pkmn_pokemon_init(
                    gen3_pokemon_with_oras_mega_forms[pokemon_index],
                    game,
                    "Mega",
                    100,
                    &pokemon
                );
        if(is_game_oras(game))
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon.p_internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon.p_internal);
    }

    // Castform should always work.
    struct pkmn_database_pokemon_entry castform_entry;
    error = pkmn_database_get_pokemon_entry(
                PKMN_SPECIES_CASTFORM,
                PKMN_GAME_OMEGA_RUBY,
                "",
                &castform_entry
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(4, castform_entry.forms.length);
    for(size_t form_index = 0; form_index < 4; ++form_index)
    {
        TEST_ASSERT_NOT_NULL(castform_entry.forms.pp_strings[form_index]);
        error = pkmn_pokemon_init(
                    PKMN_SPECIES_CASTFORM,
                    game,
                    castform_entry.forms.pp_strings[form_index],
                    30,
                    &pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon.p_internal);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NULL(pokemon.p_internal);
    }

    error = pkmn_database_pokemon_entry_free(&castform_entry);
    PKMN_TEST_ASSERT_SUCCESS(error);

    // Primal Reversion forms should only work in OR/AS.
    static const enum pkmn_species primal_version_pokemon[] =
    {
        PKMN_SPECIES_GROUDON,
        PKMN_SPECIES_KYOGRE
    };
    for(size_t pokemon_index = 0; pokemon_index < 2; ++pokemon_index)
    {
        error = pkmn_pokemon_init(
                    primal_version_pokemon[pokemon_index],
                    game,
                    "Standard",
                    70,
                    &pokemon
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pokemon.p_internal);

        error = pkmn_pokemon_free(&pokemon);
        PKMN_TEST_ASSERT_SUCCESS(error);

        error = pkmn_pokemon_init(
                    primal_version_pokemon[pokemon_index],
                    game,
                    "Primal Reversion",
                    70,
                    &pokemon
                );
        if(is_game_oras(game))
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon.p_internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon.p_internal);
    }

    // In Generation III, Deoxys's form is game-specific.
    if(generation == 3)
    {
        error = pkmn_pokemon_init(
                    PKMN_SPECIES_DEOXYS,
                    game,
                    "Normal",
                    70,
                    &pokemon
                );
        if(is_game_rs(game) || is_game_gamecube(game))
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon.p_internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon.p_internal);

        error = pkmn_pokemon_init(
                    PKMN_SPECIES_DEOXYS,
                    game,
                    "Attack",
                    70,
                    &pokemon
                );
        if(game == PKMN_GAME_FIRERED)
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon.p_internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon.p_internal);

        error = pkmn_pokemon_init(
                    PKMN_SPECIES_DEOXYS,
                    game,
                    "Defense",
                    70,
                    &pokemon
                );
        if(game == PKMN_GAME_LEAFGREEN)
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon.p_internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon.p_internal);

        error = pkmn_pokemon_init(
                    PKMN_SPECIES_DEOXYS,
                    game,
                    "Speed",
                    70,
                    &pokemon
                );
        if(game == PKMN_GAME_EMERALD)
        {
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon.p_internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
        }
        else
        {
            TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
        }
        TEST_ASSERT_NULL(pokemon.p_internal);
    }
    else
    {
        struct pkmn_database_pokemon_entry deoxys_entry;
        error = pkmn_database_get_pokemon_entry(
                    PKMN_SPECIES_DEOXYS,
                    game,
                    "",
                    &deoxys_entry
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_EQUAL(4, deoxys_entry.forms.length);

        for(size_t form_index = 0; form_index < 4; ++form_index)
        {
            TEST_ASSERT_NOT_NULL(deoxys_entry.forms.pp_strings[form_index]);

            error = pkmn_pokemon_init(
                        PKMN_SPECIES_DEOXYS,
                        game,
                        deoxys_entry.forms.pp_strings[form_index],
                        70,
                        &pokemon
                    );
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NOT_NULL(pokemon.p_internal);

            error = pkmn_pokemon_free(&pokemon);
            PKMN_TEST_ASSERT_SUCCESS(error);
            TEST_ASSERT_NULL(pokemon.p_internal);
        }

        error = pkmn_database_pokemon_entry_free(&deoxys_entry);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
}

void red_form_test()
{
    test_gen1_pokemon_forms(PKMN_GAME_RED);
}

void blue_form_test()
{
    test_gen1_pokemon_forms(PKMN_GAME_BLUE);
}

void yellow_form_test()
{
    test_gen1_pokemon_forms(PKMN_GAME_YELLOW);
}

void gold_form_test()
{
    test_gen1_pokemon_forms(PKMN_GAME_GOLD);
    test_gen2_pokemon_forms(PKMN_GAME_GOLD);
}

void silver_form_test()
{
    test_gen1_pokemon_forms(PKMN_GAME_SILVER);
    test_gen2_pokemon_forms(PKMN_GAME_SILVER);
}

void crystal_form_test()
{
    test_gen1_pokemon_forms(PKMN_GAME_CRYSTAL);
    test_gen2_pokemon_forms(PKMN_GAME_CRYSTAL);
}

void ruby_form_test()
{
    test_gen1_pokemon_forms(PKMN_GAME_RUBY);
    test_gen2_pokemon_forms(PKMN_GAME_RUBY);
    test_gen3_pokemon_forms(PKMN_GAME_RUBY);
}

void sapphire_form_test()
{
    test_gen1_pokemon_forms(PKMN_GAME_SAPPHIRE);
    test_gen2_pokemon_forms(PKMN_GAME_SAPPHIRE);
    test_gen3_pokemon_forms(PKMN_GAME_SAPPHIRE);
}

void emerald_form_test()
{
    test_gen1_pokemon_forms(PKMN_GAME_EMERALD);
    test_gen2_pokemon_forms(PKMN_GAME_EMERALD);
    test_gen3_pokemon_forms(PKMN_GAME_EMERALD);
}

void firered_form_test()
{
    test_gen1_pokemon_forms(PKMN_GAME_FIRERED);
    test_gen2_pokemon_forms(PKMN_GAME_FIRERED);
    test_gen3_pokemon_forms(PKMN_GAME_FIRERED);
}

void leafgreen_form_test()
{
    test_gen1_pokemon_forms(PKMN_GAME_LEAFGREEN);
    test_gen2_pokemon_forms(PKMN_GAME_LEAFGREEN);
    test_gen3_pokemon_forms(PKMN_GAME_LEAFGREEN);
}

void colosseum_form_test()
{
    test_gen1_pokemon_forms(PKMN_GAME_COLOSSEUM);
    test_gen2_pokemon_forms(PKMN_GAME_COLOSSEUM);
    test_gen3_pokemon_forms(PKMN_GAME_COLOSSEUM);
}

void xd_form_test()
{
    test_gen1_pokemon_forms(PKMN_GAME_XD);
    test_gen2_pokemon_forms(PKMN_GAME_XD);
    test_gen3_pokemon_forms(PKMN_GAME_XD);
}
