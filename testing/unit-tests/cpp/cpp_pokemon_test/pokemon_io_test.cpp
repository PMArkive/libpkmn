/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "env.hpp"
#include "private_exports.hpp"
#include "types/rng.hpp"

#include <pkmntest/util.hpp>

#include <pkmn/pokemon.hpp>
#include <pkmn/database/lists.hpp>
#include <pkmn/utils/paths.hpp>

#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include <gtest/gtest.h>

#include <cstdlib>

namespace fs = boost::filesystem;

static const fs::path LIBPKMN_TEST_FILES(pkmn_getenv("LIBPKMN_TEST_FILES"));
static const fs::path TMP_DIR(pkmn::get_tmp_dir());

static std::string export_pokemon_to_tmp_file(
    pkmn::pokemon::sptr pokemon,
    const std::string& extension
)
{
    fs::path output_path = TMP_DIR / str(boost::format("%u.%s")
                                         % pkmn::rng<uint32_t>().rand()
                                         % extension);

    pokemon->export_to_file(output_path.string());

    return output_path.string();
}

static bool get_random_bool()
{
    return (rand() % 100) > 50;
}

static pkmn::pokemon::sptr get_random_pokemon(
    pkmn::e_game game
)
{
    int generation = pkmn::priv::game_enum_to_generation(game);
    pkmn::rng<uint32_t> rng;

    std::vector<pkmn::e_item> item_list = pkmn::database::get_item_list(game);
    std::vector<pkmn::e_move> move_list = pkmn::database::get_move_list(game);
    std::vector<pkmn::e_species> pokemon_list = pkmn::database::get_pokemon_list(generation, true);

    // Don't deal with Deoxys or Unown issues here.
    pkmn::e_species species = pkmn::e_species::NONE;
    if(generation == 3)
    {
        do
        {
            species = pokemon_list[rng.rand() % pokemon_list.size()];
        }
        while((species == pkmn::e_species::UNOWN) ||
              (species == pkmn::e_species::DEOXYS));
    }
    else
    {
        species = pokemon_list[rng.rand() % pokemon_list.size()];
    }
    pkmn::pokemon::sptr ret = pkmn::pokemon::make(
                                  species,
                                  game,
                                  "",
                                  ((rng.rand() % 99) + 2)
                              );
    for(int move_index = 0; move_index < 4; ++move_index)
    {
        pkmn::e_move move = pkmn::e_move::NONE;
        do
        {
            move = move_list[rng.rand() % move_list.size()];
        }
        while(move >= pkmn::e_move::SHADOW_RUSH);
        ret->set_move(move, move_index);
    }

    const std::map<pkmn::e_stat, int>& EVs = ret->get_EVs();
    for(auto iter = EVs.begin(); iter != EVs.end(); ++iter)
    {
        ret->set_EV(iter->first, (rand() % 256));
    }

    const std::map<pkmn::e_stat, int>& IVs = ret->get_IVs();
    for(auto iter = IVs.begin(); iter != IVs.end(); ++iter)
    {
        ret->set_IV(iter->first, (rand() % 16));
    }

    if(generation >= 2)
    {
        // Keep going until one is holdable
        while(ret->get_held_item() == pkmn::e_item::NONE)
        {
            try {
                ret->set_held_item(
                    item_list[rng.rand() % item_list.size()]
                );
            } catch(std::invalid_argument&) {}
        }

        ret->set_shininess(get_random_bool());
    }

    if(generation >= 3)
    {
        const std::map<pkmn::e_marking, bool>& markings = ret->get_markings();
        for(auto iter = markings.begin(); iter != markings.end(); ++iter)
        {
            ret->set_marking(iter->first, get_random_bool());
        }

        const std::map<std::string, bool>& ribbons = ret->get_ribbons();
        for(auto iter = ribbons.begin(); iter != ribbons.end(); ++iter)
        {
            ret->set_ribbon(iter->first, get_random_bool());
        }

        const std::map<pkmn::e_contest_stat, int>& contest_stats = ret->get_contest_stats();
        for(auto iter = contest_stats.begin(); iter != contest_stats.end(); ++iter)
        {
            ret->set_contest_stat(iter->first, (rand() % 256));
        }
    }

    return ret;
}

static void compare_pokemon(
    pkmn::pokemon::sptr pokemon1,
    pkmn::pokemon::sptr pokemon2
)
{
    pkmn::e_game game = pokemon1->get_game();
    int generation = pkmn::priv::game_enum_to_generation(game);

    // There is no way to determine what game an imported Generation I-II
    // Pokémon comes from, so LibPKMN defaults to a default valid game.
    if(generation >= 3)
    {
        ASSERT_EQ(game, pokemon2->get_game());
    }

    EXPECT_EQ(pokemon1->get_species(), pokemon2->get_species());
    EXPECT_EQ(pokemon1->get_form(), pokemon2->get_form());
    EXPECT_EQ(pokemon1->get_original_trainer_id(), pokemon2->get_original_trainer_id());
    EXPECT_EQ(pokemon1->get_experience(), pokemon2->get_experience());
    EXPECT_EQ(pokemon1->get_level(), pokemon2->get_level());
    EXPECT_EQ(pokemon1->get_nickname(), pokemon2->get_nickname());
    EXPECT_EQ(pokemon1->get_original_trainer_name(), pokemon2->get_original_trainer_name());

    const std::map<pkmn::e_stat, int>& EVs1 = pokemon1->get_EVs();
    const std::map<pkmn::e_stat, int>& EVs2 = pokemon2->get_EVs();
    for(auto iter = EVs1.begin(); iter != EVs1.end(); ++iter)
    {
        EXPECT_EQ(iter->second, EVs2.at(iter->first));
    }

    const std::map<pkmn::e_stat, int>& IVs1 = pokemon1->get_IVs();
    const std::map<pkmn::e_stat, int>& IVs2 = pokemon2->get_IVs();
    for(auto iter = IVs1.begin(); iter != IVs1.end(); ++iter)
    {
        EXPECT_EQ(iter->second, IVs2.at(iter->first));
    }

    const std::map<pkmn::e_stat, int>& stats1 = pokemon1->get_stats();
    const std::map<pkmn::e_stat, int>& stats2 = pokemon2->get_stats();
    for(auto iter = stats1.begin(); iter != stats1.end(); ++iter)
    {
        EXPECT_EQ(iter->second, stats2.at(iter->first));
    }

    if(pokemon2->get_game() == game)
    {
        EXPECT_EQ(pokemon1->get_icon_filepath(), pokemon2->get_icon_filepath());
        EXPECT_EQ(pokemon1->get_sprite_filepath(), pokemon2->get_sprite_filepath());
    }

    if(generation >= 2)
    {
        EXPECT_EQ(pokemon1->get_original_trainer_gender(), pokemon2->get_original_trainer_gender());
        EXPECT_EQ(pokemon1->get_current_trainer_friendship(), pokemon2->get_current_trainer_friendship());
        EXPECT_EQ(pokemon1->get_gender(), pokemon2->get_gender());
        EXPECT_EQ(pokemon1->is_shiny(), pokemon2->is_shiny());
        EXPECT_EQ(pokemon1->get_held_item(), pokemon2->get_held_item());
        EXPECT_EQ(pokemon1->get_level_met(), pokemon2->get_level_met());
        EXPECT_EQ(pokemon1->get_location_met(false), pokemon2->get_location_met(false));
    }
    if(generation >= 3)
    {
        EXPECT_EQ(pokemon1->get_ability(), pokemon2->get_ability());
        EXPECT_EQ(pokemon1->get_ball(), pokemon2->get_ball());
        EXPECT_EQ(pokemon1->get_personality(), pokemon2->get_personality());
        EXPECT_EQ(pokemon1->get_ball(), pokemon2->get_ball());

        const std::map<pkmn::e_marking, bool>& markings1 = pokemon1->get_markings();
        const std::map<pkmn::e_marking, bool>& markings2 = pokemon2->get_markings();
        for(auto iter = markings1.begin(); iter != markings1.end(); ++iter)
        {
            EXPECT_EQ(iter->second, markings2.at(iter->first));
        }

        const std::map<std::string, bool>& ribbons1 = pokemon1->get_ribbons();
        const std::map<std::string, bool>& ribbons2 = pokemon2->get_ribbons();
        for(auto iter = ribbons1.begin(); iter != ribbons1.end(); ++iter)
        {
            EXPECT_EQ(iter->second, ribbons2.at(iter->first));
        }

        const std::map<pkmn::e_contest_stat, int>& contest_stats1 = pokemon1->get_contest_stats();
        const std::map<pkmn::e_contest_stat, int>& contest_stats2 = pokemon2->get_contest_stats();
        for(auto iter = contest_stats1.begin(); iter != contest_stats1.end(); ++iter)
        {
            EXPECT_EQ(iter->second, contest_stats2.at(iter->first));
        }
    }
    if(generation >= 4)
    {
        EXPECT_EQ(pokemon1->get_location_met(true), pokemon2->get_location_met(true));
    }
}

// Actual tests

using pokemon_io_test_params_t = std::tuple<
    pkmn::e_game, // game
    std::string   // extension
>;

class pokemon_io_test: public ::testing::TestWithParam<pokemon_io_test_params_t> {};

TEST_P(pokemon_io_test, test_saving_and_loading)
{
    pokemon_io_test_params_t test_params = GetParam();
    pkmn::e_game game = std::get<0>(test_params);
    const std::string& extension = std::get<1>(test_params);

    pkmn::pokemon::sptr random_pokemon = get_random_pokemon(game);
    std::string tmp_path = export_pokemon_to_tmp_file(
                               random_pokemon,
                               extension
                           );
    pkmn::pokemon::sptr imported_pokemon = pkmn::pokemon::from_file(tmp_path);

    compare_pokemon(random_pokemon, imported_pokemon);

    std::remove(tmp_path.c_str());
}

static const std::vector<pokemon_io_test_params_t> TEST_PARAMS =
{
    // Generation I
    std::make_tuple(pkmn::e_game::RED,    "pk1"),
    std::make_tuple(pkmn::e_game::BLUE,   "pk1"),
    std::make_tuple(pkmn::e_game::YELLOW, "pk1"),

    // Generation II
    std::make_tuple(pkmn::e_game::GOLD,    "pk2"),
    std::make_tuple(pkmn::e_game::SILVER,  "pk2"),
    std::make_tuple(pkmn::e_game::CRYSTAL, "pk2"),

    // Game Boy Advance
    std::make_tuple(pkmn::e_game::RUBY,      "3gpkm"),
    std::make_tuple(pkmn::e_game::RUBY,      "pk3"),
    std::make_tuple(pkmn::e_game::SAPPHIRE,  "3gpkm"),
    std::make_tuple(pkmn::e_game::SAPPHIRE,  "pk3"),
    std::make_tuple(pkmn::e_game::EMERALD,   "3gpkm"),
    std::make_tuple(pkmn::e_game::EMERALD,   "pk3"),
    std::make_tuple(pkmn::e_game::FIRERED,   "3gpkm"),
    std::make_tuple(pkmn::e_game::FIRERED,   "pk3"),
    std::make_tuple(pkmn::e_game::LEAFGREEN, "3gpkm"),
    std::make_tuple(pkmn::e_game::LEAFGREEN, "pk3"),

    // Gamecube
    std::make_tuple(pkmn::e_game::COLOSSEUM, "ck3"),
    std::make_tuple(pkmn::e_game::XD,        "xk3"),
};

INSTANTIATE_TEST_CASE_P(
    pokemon_io_test,
    pokemon_io_test,
    ::testing::ValuesIn(TEST_PARAMS)
);

// Test loading a known .3gpkm file from outside LibPKMN.
TEST(pokemon_io_test, test_outside_3gpkm) {
    const fs::path _3GPKM_DIR = LIBPKMN_TEST_FILES / "3gpkm";

    pkmn::pokemon::sptr mightyena = pkmn::pokemon::from_file(
                                        (_3GPKM_DIR / "MIGHTYENA.3gpkm").string()
                                    );
    EXPECT_EQ(pkmn::e_species::MIGHTYENA, mightyena->get_species());
    EXPECT_EQ(pkmn::e_game::EMERALD, mightyena->get_game());
    EXPECT_EQ("Standard", mightyena->get_form());
    EXPECT_EQ("MIGHTYENA", mightyena->get_nickname());
    EXPECT_FALSE(mightyena->is_shiny());
    EXPECT_EQ(pkmn::e_condition::NONE, mightyena->get_condition());
    EXPECT_EQ(pkmn::e_item::HEART_SCALE, mightyena->get_held_item());
    EXPECT_EQ("A", mightyena->get_original_trainer_name());
    EXPECT_EQ(61415, mightyena->get_original_trainer_public_id());
    EXPECT_EQ(3417, mightyena->get_original_trainer_secret_id());
    EXPECT_EQ(223997927, mightyena->get_original_trainer_id());
    EXPECT_EQ(pkmn::e_gender::FEMALE, mightyena->get_original_trainer_gender());
    EXPECT_EQ(254, mightyena->get_current_trainer_friendship());
    EXPECT_EQ(pkmn::e_ability::INTIMIDATE, mightyena->get_ability());
    EXPECT_EQ(pkmn::e_ball::GREAT_BALL, mightyena->get_ball());
    EXPECT_EQ(25, mightyena->get_level_met());
    EXPECT_EQ("Route 120", mightyena->get_location_met(false));
    EXPECT_EQ(pkmn::e_game::EMERALD, mightyena->get_original_game());
    EXPECT_EQ(3557601241, mightyena->get_personality());
    EXPECT_EQ(128734, mightyena->get_experience());
    EXPECT_EQ(50, mightyena->get_level());

    const std::map<pkmn::e_marking, bool>& mightyena_markings = mightyena->get_markings();
    EXPECT_EQ(4, mightyena_markings.size());
    for(const auto& marking_iter: mightyena_markings)
    {
        EXPECT_FALSE(marking_iter.second);
    }

    const std::map<std::string, bool>& mightyena_ribbons = mightyena->get_ribbons();
    EXPECT_EQ(32, mightyena_ribbons.size());
    for(auto iter = mightyena_ribbons.begin(); iter != mightyena_ribbons.end(); ++iter) {
        if(iter->first == "Champion") {
            EXPECT_TRUE(iter->second);
        } else {
            EXPECT_FALSE(iter->second);
        }
    }

    const std::map<pkmn::e_contest_stat, int>& mightyena_contest_stats = mightyena->get_contest_stats();
    EXPECT_EQ(6, mightyena_contest_stats.size());
    for(auto iter = mightyena_contest_stats.begin(); iter != mightyena_contest_stats.end(); ++iter) {
        EXPECT_EQ(0, iter->second);
    }

    static const std::vector<pkmn::e_move> expected_mightyena_moves =
    {
        pkmn::e_move::CRUNCH,
        pkmn::e_move::STRENGTH,
        pkmn::e_move::SHADOW_BALL,
        pkmn::e_move::DOUBLE_EDGE
    };

    const pkmn::move_slots_t mightyena_moves = mightyena->get_moves();
    EXPECT_EQ(4, mightyena_moves.size());
    for(int move_index = 0; move_index < 4; ++move_index)
    {
        EXPECT_EQ(
            expected_mightyena_moves[move_index],
            mightyena_moves[move_index].move
        );
    }

    const std::map<pkmn::e_stat, int>& mightyena_EVs = mightyena->get_EVs();
    EXPECT_EQ(6, mightyena_EVs.size());
    EXPECT_EQ(30, mightyena_EVs.at(pkmn::e_stat::HP));
    EXPECT_EQ(110, mightyena_EVs.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(32, mightyena_EVs.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(48, mightyena_EVs.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(17, mightyena_EVs.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(83, mightyena_EVs.at(pkmn::e_stat::SPECIAL_DEFENSE));

    const std::map<pkmn::e_stat, int>& mightyena_IVs = mightyena->get_IVs();
    EXPECT_EQ(6, mightyena_IVs.size());
    EXPECT_EQ(26, mightyena_IVs.at(pkmn::e_stat::HP));
    EXPECT_EQ(28, mightyena_IVs.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(4, mightyena_IVs.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(13, mightyena_IVs.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(25, mightyena_IVs.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(26, mightyena_IVs.at(pkmn::e_stat::SPECIAL_DEFENSE));

    const std::map<pkmn::e_stat, int>& mightyena_stats = mightyena->get_stats();
    EXPECT_EQ(6, mightyena_stats.size());
    EXPECT_EQ(146, mightyena_stats.at(pkmn::e_stat::HP));
    EXPECT_EQ(122, mightyena_stats.at(pkmn::e_stat::ATTACK));
    EXPECT_EQ(81, mightyena_stats.at(pkmn::e_stat::DEFENSE));
    EXPECT_EQ(87, mightyena_stats.at(pkmn::e_stat::SPEED));
    EXPECT_EQ(79, mightyena_stats.at(pkmn::e_stat::SPECIAL_ATTACK));
    EXPECT_EQ(88, mightyena_stats.at(pkmn::e_stat::SPECIAL_DEFENSE));
}

// These tests makes sure that when a Pokémon is exported and re-imported, that
// its form is preserved.

typedef struct
{
    pkmn::e_species species;
    std::string file_extension;
    std::vector<pkmn::e_game> games;
} io_form_test_params_t;

class io_form_test: public ::testing::TestWithParam<io_form_test_params_t> {};

TEST_P(io_form_test, test_form_is_preserved_after_io)
{
    io_form_test_params_t test_params = GetParam();

    for(pkmn::e_game game: test_params.games)
    {
        std::vector<std::string> forms = pkmn::database::pokemon_entry(
                                             test_params.species,
                                             game,
                                             ""
                                         ).get_forms();

        // Instantiate a Pokémon in the given form, export and reimport it,
        // and make sure the form is preserved.
        for(const std::string& form: forms)
        {
            pkmn::pokemon::sptr pokemon_of_form = pkmn::pokemon::make(
                                                      test_params.species,
                                                      game,
                                                      form,
                                                      5
                                                  );
            ASSERT_EQ(test_params.species, pokemon_of_form->get_species());
            ASSERT_EQ(game, pokemon_of_form->get_game());
            ASSERT_EQ(form, pokemon_of_form->get_form());

            std::string tmp_path = export_pokemon_to_tmp_file(
                                       pokemon_of_form,
                                       test_params.file_extension
                                   );

            // Don't check game, as it's not guaranteed to match in all cases.
            pkmn::pokemon::sptr imported_pokemon = pkmn::pokemon::from_file(
                                                       tmp_path
                                                   );
            ASSERT_EQ(test_params.species, imported_pokemon->get_species());
            ASSERT_EQ(form, imported_pokemon->get_form());

            std::remove(tmp_path.c_str());
        }

        // Instantiate a Pokémon in the default form, set it to each form,
        // export and reimport it, and make sure the form is preserved.
        pkmn::pokemon::sptr test_pokemon = pkmn::pokemon::make(
                                               test_params.species,
                                               game,
                                               "",
                                               5
                                           );
        ASSERT_EQ(test_params.species, test_pokemon->get_species());
        ASSERT_EQ(game, test_pokemon->get_game());

        for(const std::string& form: forms)
        {
            test_pokemon->set_form(form);

            std::string tmp_path = export_pokemon_to_tmp_file(
                                       test_pokemon,
                                       test_params.file_extension
                                   );

            // Don't check game, as it's not guaranteed to match in all cases.
            pkmn::pokemon::sptr imported_pokemon = pkmn::pokemon::from_file(
                                                       tmp_path
                                                   );
            ASSERT_EQ(test_params.species, imported_pokemon->get_species());
            ASSERT_EQ(form, imported_pokemon->get_form());

            std::remove(tmp_path.c_str());
        }
    }
}

// Generation II

static const std::vector<io_form_test_params_t> GEN2_IO_FORM_TEST_PARAMS =
{
    {
        pkmn::e_species::UNOWN,
        "pk2",
        {
            pkmn::e_game::GOLD,
            pkmn::e_game::SILVER,
            pkmn::e_game::CRYSTAL
        }
    }
};

INSTANTIATE_TEST_CASE_P(
    gen2_io_form_test,
    io_form_test,
    ::testing::ValuesIn(GEN2_IO_FORM_TEST_PARAMS)
);

static const std::vector<io_form_test_params_t> GEN3_IO_FORM_TEST_PARAMS =
{
    {
        pkmn::e_species::UNOWN,
        "3gpkm",
        {
            pkmn::e_game::RUBY,
            pkmn::e_game::SAPPHIRE,
            pkmn::e_game::EMERALD,
            pkmn::e_game::FIRERED,
            pkmn::e_game::LEAFGREEN
        }
    },
    {
        pkmn::e_species::UNOWN,
        "pk3",
        {
            pkmn::e_game::RUBY,
            pkmn::e_game::SAPPHIRE,
            pkmn::e_game::EMERALD,
            pkmn::e_game::FIRERED,
            pkmn::e_game::LEAFGREEN
        }
    },
    {
        pkmn::e_species::UNOWN,
        "ck3",
        {pkmn::e_game::COLOSSEUM}
    },
    {
        pkmn::e_species::UNOWN,
        "xk3",
        {pkmn::e_game::XD}
    },
};

INSTANTIATE_TEST_CASE_P(
    gen3_io_form_test,
    io_form_test,
    ::testing::ValuesIn(GEN3_IO_FORM_TEST_PARAMS)
);
