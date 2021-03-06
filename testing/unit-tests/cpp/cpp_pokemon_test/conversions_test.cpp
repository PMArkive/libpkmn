/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "private_exports.hpp"
#include "pksav/enum_maps.hpp"
#include "types/rng.hpp"
#include "utils/misc.hpp"

#include <pkmntest/util.hpp>

#include <pkmn/pokemon.hpp>

#include <pkmn/database/item_entry.hpp>
#include <pkmn/database/lists.hpp>
#include <pkmn/enums/enum_to_string.hpp>

#include <gtest/gtest.h>

// Common to all generations
static const std::vector<pkmn::e_condition> CONDITIONS =
    pkmn::map_keys_to_vector<decltype(pksav::gb_condition_bimap_t::left), pkmn::e_condition>(
        pksav::get_gb_condition_bimap().left
    );

template <typename T>
static T random_value(const std::vector<T>& vec)
{
    static pkmn::rng<size_t> size_rng;
    return vec.at(size_rng.rand(0, vec.size()-1));
}

static std::string random_string(
    size_t len
)
{
    std::string ret;
    pkmn::rng<char> char_rng;

    for(size_t i = 0; i < len; ++i)
    {
        ret += char_rng.rand('a', 'z');
    }

    return ret;
}

bool random_bool()
{
    static pkmn::rng<int> int_rng;
    return (int_rng.rand(0, 99) >= 50);
}

typedef struct
{
    pkmn::e_species species;
    std::string form;

    pkmn::e_game origin_game;
    pkmn::e_game dest_game;
} conversions_test_params_t;

class conversions_test: public ::testing::TestWithParam<conversions_test_params_t> {};

// TODO: specific met locations
TEST_P(conversions_test, conversions_test)
{
    conversions_test_params_t params = GetParam();

    pkmn::pokemon::sptr first_pokemon = pkmn::pokemon::make(
                                            params.species,
                                            params.origin_game,
                                            params.form,
                                            50
                                        );

    int origin_generation = pkmn::priv::game_enum_to_generation(params.origin_game);
    int dest_generation = pkmn::priv::game_enum_to_generation(params.dest_game);
    int min_generation = std::min<int>(origin_generation, dest_generation);
    pkmn::e_game game_for_lists = (min_generation == origin_generation) ? params.origin_game : params.dest_game;

    // Set random values. TODO: EVs, IVs
    pkmn::rng<int> int_rng;
    pkmn::rng<uint8_t> uint8_rng;
    pkmn::rng<uint16_t> uint16_rng;
    pkmn::rng<uint32_t> uint32_rng;
    pkmn::rng<size_t> size_rng;

    std::vector<pkmn::e_item> items = pkmn::database::get_item_list(game_for_lists);
    std::vector<pkmn::e_move> moves = pkmn::database::get_move_list(game_for_lists);

    for(int move_index = 0; move_index < 4; ++move_index)
    {
        pkmn::e_move move = pkmn::e_move::NONE;
        do
        {
            move = moves[size_rng.rand(0, moves.size()-1)];
        }
        while(move >= pkmn::e_move::SHADOW_RUSH);

        first_pokemon->set_move(
            move,
            move_index
        );
    }

    if(origin_generation >= 3)
    {
        first_pokemon->set_original_trainer_id(uint32_rng.rand());

        pkmn::ability_pair_t abilities = first_pokemon->get_database_entry().get_abilities();
        if(abilities.second != pkmn::e_ability::NONE)
        {
            first_pokemon->set_ability(random_bool() ? abilities.first : abilities.second);
        }

        // TODO: random ball

        first_pokemon->set_personality(uint32_rng.rand());
    }
    else
    {
        first_pokemon->set_original_trainer_id(uint16_rng.rand());
    }

    if(min_generation >= 2)
    {
        /*
         * Make sure the item is holdable. For Generation III, no GCN-exclusive items appear
         * to be holdable.
         */
        pkmn::e_item held_item = pkmn::e_item::NONE;
        do
        {
            held_item = items[size_rng.rand(0, items.size()-1)];
        }
        while(!pkmn::database::item_entry(held_item, params.origin_game).holdable() ||
              (held_item == pkmn::e_item::JOY_SCENT) || (held_item == pkmn::e_item::VIVID_SCENT) ||
              (held_item == pkmn::e_item::EXCITE_SCENT));

        first_pokemon->set_held_item(held_item);
    }
    if(origin_generation >= 2)
    {
        first_pokemon->set_gender(random_bool() ? pkmn::e_gender::MALE : pkmn::e_gender::FEMALE);
        first_pokemon->set_shininess(random_bool());
        first_pokemon->set_current_trainer_friendship(uint8_rng.rand());

        if((params.origin_game != pkmn::e_game::GOLD) and (params.origin_game != pkmn::e_game::SILVER))
        {
            first_pokemon->set_original_trainer_gender(random_bool() ? pkmn::e_gender::MALE : pkmn::e_gender::FEMALE);
        }

        // The max level met value in Generation II is 63.
        first_pokemon->set_level_met(int_rng.rand(2, (origin_generation == 2) ? 63 : 100));
    }
    if(origin_generation >= 3)
    {
        // Randomize markings, ribbons, and contest stats.
        for(const auto& markings_iter: first_pokemon->get_markings())
        {
            first_pokemon->set_marking(
                markings_iter.first,
                random_bool()
            );
        }
        for(const auto& ribbons_iter: first_pokemon->get_ribbons())
        {
            first_pokemon->set_ribbon(
                ribbons_iter.first,
                random_bool()
            );
        }
        for(const auto& contest_stats_iter: first_pokemon->get_contest_stats())
        {
            first_pokemon->set_contest_stat(
                contest_stats_iter.first,
                int_rng.rand(0, 255)
            );
        }

        first_pokemon->set_pokerus_duration(int_rng.rand(0, 15));
    }

    first_pokemon->set_condition(random_value(CONDITIONS));

    first_pokemon->set_nickname(random_string(10));
    first_pokemon->set_original_trainer_name(random_string(7));

    // The max level met value in Generation II is 63, which restricts this as well.
    first_pokemon->set_level(int_rng.rand(2, (dest_generation == 2) ? 63 : 100));

    // Convert to the second game and compare.
    pkmn::pokemon::sptr second_pokemon = first_pokemon->to_game(params.dest_game);

    EXPECT_EQ(first_pokemon->get_species(), second_pokemon->get_species());
    EXPECT_EQ(params.dest_game, second_pokemon->get_game());
    EXPECT_EQ(first_pokemon->get_condition(), second_pokemon->get_condition());
    EXPECT_EQ(first_pokemon->get_form(), second_pokemon->get_form());
    EXPECT_EQ(first_pokemon->get_nickname(), second_pokemon->get_nickname());
    EXPECT_EQ(first_pokemon->get_original_trainer_name(), second_pokemon->get_original_trainer_name());
    EXPECT_EQ(first_pokemon->get_original_trainer_id(), second_pokemon->get_original_trainer_id());
    EXPECT_EQ(first_pokemon->get_original_trainer_public_id(), second_pokemon->get_original_trainer_public_id());
    EXPECT_EQ(first_pokemon->get_experience(), second_pokemon->get_experience());
    EXPECT_EQ(first_pokemon->get_level(), second_pokemon->get_level());

    const pkmn::move_slots_t& first_pokemon_move_slots = first_pokemon->get_moves();
    const pkmn::move_slots_t& second_pokemon_move_slots = second_pokemon->get_moves();
    for(size_t i = 0; i < 4; ++i)
    {
        EXPECT_EQ(first_pokemon_move_slots[i].move, second_pokemon_move_slots[i].move);
        EXPECT_EQ(first_pokemon_move_slots[i].pp, second_pokemon_move_slots[i].pp);
    }

    if(min_generation >= 3)
    {
        EXPECT_EQ(first_pokemon->get_original_trainer_secret_id(), second_pokemon->get_original_trainer_secret_id());
        EXPECT_EQ(first_pokemon->get_ability(), second_pokemon->get_ability());
        EXPECT_EQ(first_pokemon->get_ball(), second_pokemon->get_ball());
        EXPECT_EQ(first_pokemon->get_original_game(), second_pokemon->get_original_game());
        EXPECT_EQ(first_pokemon->get_personality(), second_pokemon->get_personality());
        EXPECT_EQ(first_pokemon->get_pokerus_duration(), second_pokemon->get_pokerus_duration());

        if(origin_generation == dest_generation)
        {
            EXPECT_EQ(first_pokemon->get_markings(), second_pokemon->get_markings());
            EXPECT_EQ(first_pokemon->get_ribbons(), second_pokemon->get_ribbons());
            EXPECT_EQ(first_pokemon->get_contest_stats(), second_pokemon->get_contest_stats());
        }
        // TODO: else specific functions to check
    }
    if(min_generation >= 2)
    {
        EXPECT_EQ(first_pokemon->get_original_trainer_gender(), second_pokemon->get_original_trainer_gender());
        EXPECT_EQ(first_pokemon->get_gender(), second_pokemon->get_gender());
        EXPECT_EQ(first_pokemon->is_shiny(), second_pokemon->is_shiny());
        EXPECT_EQ(first_pokemon->get_held_item(), second_pokemon->get_held_item());
        EXPECT_EQ(first_pokemon->get_current_trainer_friendship(), second_pokemon->get_current_trainer_friendship());
        EXPECT_EQ(first_pokemon->get_level(), second_pokemon->get_level_met());
    }
}

static const conversions_test_params_t TEST_PARAMS[] =
{
    // Generation I -> Generation I
    {pkmn::e_species::BULBASAUR, "", pkmn::e_game::RED, pkmn::e_game::YELLOW},

    // Generation I -> Generation II
    {pkmn::e_species::SQUIRTLE, "", pkmn::e_game::BLUE, pkmn::e_game::GOLD},

    // Generation II -> Generation II
    {pkmn::e_species::CYNDAQUIL, "", pkmn::e_game::GOLD, pkmn::e_game::CRYSTAL},
    {pkmn::e_species::TOTODILE, "", pkmn::e_game::CRYSTAL, pkmn::e_game::GOLD},

    // Generation II -> Generation I
    {pkmn::e_species::CHARMANDER, "", pkmn::e_game::SILVER, pkmn::e_game::BLUE},

    // GBA -> GBA
    {pkmn::e_species::TORCHIC, "", pkmn::e_game::RUBY, pkmn::e_game::SAPPHIRE},
    {pkmn::e_species::MUDKIP, "", pkmn::e_game::RUBY, pkmn::e_game::EMERALD},
    {pkmn::e_species::TREECKO, "", pkmn::e_game::RUBY, pkmn::e_game::FIRERED},
    {pkmn::e_species::TORCHIC, "", pkmn::e_game::EMERALD, pkmn::e_game::SAPPHIRE},
    {pkmn::e_species::MUDKIP, "", pkmn::e_game::EMERALD, pkmn::e_game::EMERALD},
    {pkmn::e_species::TREECKO, "", pkmn::e_game::EMERALD, pkmn::e_game::FIRERED},
    {pkmn::e_species::CHARMANDER, "", pkmn::e_game::FIRERED, pkmn::e_game::SAPPHIRE},
    {pkmn::e_species::SQUIRTLE, "", pkmn::e_game::FIRERED, pkmn::e_game::EMERALD},
    {pkmn::e_species::BULBASAUR, "", pkmn::e_game::FIRERED, pkmn::e_game::FIRERED},

    // GBA -> GCN
    {pkmn::e_species::EEVEE, "", pkmn::e_game::RUBY, pkmn::e_game::COLOSSEUM},
    {pkmn::e_species::ESPEON, "", pkmn::e_game::EMERALD, pkmn::e_game::COLOSSEUM},
    {pkmn::e_species::UMBREON, "", pkmn::e_game::FIRERED, pkmn::e_game::COLOSSEUM},
    {pkmn::e_species::EEVEE, "", pkmn::e_game::RUBY, pkmn::e_game::XD},
    {pkmn::e_species::ESPEON, "", pkmn::e_game::EMERALD, pkmn::e_game::XD},
    {pkmn::e_species::UMBREON, "", pkmn::e_game::FIRERED, pkmn::e_game::XD},

    // GCN -> GBA
    {pkmn::e_species::EEVEE, "", pkmn::e_game::COLOSSEUM, pkmn::e_game::SAPPHIRE},
    {pkmn::e_species::ESPEON, "", pkmn::e_game::COLOSSEUM, pkmn::e_game::EMERALD},
    {pkmn::e_species::UMBREON, "", pkmn::e_game::COLOSSEUM, pkmn::e_game::LEAFGREEN},
    {pkmn::e_species::EEVEE, "", pkmn::e_game::XD, pkmn::e_game::SAPPHIRE},
    {pkmn::e_species::ESPEON, "", pkmn::e_game::XD, pkmn::e_game::EMERALD},
    {pkmn::e_species::UMBREON, "", pkmn::e_game::XD, pkmn::e_game::LEAFGREEN},

    // GCN -> GCN
    {pkmn::e_species::VAPOREON, "", pkmn::e_game::COLOSSEUM, pkmn::e_game::COLOSSEUM},
    {pkmn::e_species::JOLTEON, "", pkmn::e_game::COLOSSEUM, pkmn::e_game::XD},
    {pkmn::e_species::VAPOREON, "", pkmn::e_game::XD, pkmn::e_game::XD},
    {pkmn::e_species::JOLTEON, "", pkmn::e_game::XD, pkmn::e_game::COLOSSEUM}
};

INSTANTIATE_TEST_CASE_P(
    cpp_conversions_test,
    conversions_test,
    ::testing::ValuesIn(TEST_PARAMS)
);
