/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmntest/config.hpp>
#include <pkmntest/util.hpp>

#include <pkmn/daycare.hpp>
#include <pkmn/exception.hpp>

#include <pksav/gen1.h>
#include <pksav/gen2.h>
#include <pksav/gba.h>

#include "private_exports.hpp"
#include "pksav/pksav_call.hpp"

#include "pkmgc/includes.hpp"

#include <gtest/gtest.h>

#include <cstring>
#include <string>

typedef std::tuple<
            pkmn::e_game, // game
            bool          // can_breed
        > test_params_t;

class daycare_test: public ::testing::TestWithParam<test_params_t> {};

TEST_P(daycare_test, test_empty_daycare)
{
    test_params_t test_params = GetParam();
    pkmn::e_game game = std::get<0>(test_params);
    bool can_breed = std::get<1>(test_params);

    pkmn::daycare::sptr daycare = pkmn::daycare::make(game);

    // Levelup Pokémon

    int levelup_capacity = daycare->get_levelup_pokemon_capacity();
    for(int levelup_pokemon_index = 0;
        levelup_pokemon_index < levelup_capacity;
        ++levelup_pokemon_index)
    {
        const pkmn::pokemon::sptr& pokemon = daycare->get_levelup_pokemon(levelup_pokemon_index);
        ASSERT_EQ(pkmn::e_species::NONE, pokemon->get_species());
    }

    // Test invalid indices.
    ASSERT_THROW(
        daycare->get_levelup_pokemon(-1);
    , std::out_of_range);
    ASSERT_THROW(
        daycare->get_levelup_pokemon(levelup_capacity);
    , std::out_of_range);

    const pkmn::pokemon_list_t& all_levelup_pokemon =
        daycare->get_levelup_pokemon_as_vector();
    ASSERT_EQ(
        static_cast<size_t>(levelup_capacity),
        all_levelup_pokemon.size()
    );

    // Breeding Pokémon

    if(can_breed)
    {
        int breeding_capacity = daycare->get_breeding_pokemon_capacity();
        for(int breeding_pokemon_index = 0;
            breeding_pokemon_index < breeding_capacity;
            ++breeding_pokemon_index)
        {
            const pkmn::pokemon::sptr& pokemon = daycare->get_breeding_pokemon(breeding_pokemon_index);
            ASSERT_EQ(pkmn::e_species::NONE, pokemon->get_species());
        }

        // Test invalid indices.
        ASSERT_THROW(
            daycare->get_breeding_pokemon(-1);
        , std::out_of_range);
        ASSERT_THROW(
            daycare->get_breeding_pokemon(breeding_capacity);
        , std::out_of_range);

        const pkmn::pokemon_list_t& all_breeding_pokemon =
            daycare->get_breeding_pokemon_as_vector();
        ASSERT_EQ(
            static_cast<size_t>(breeding_capacity),
            all_breeding_pokemon.size()
        );

        const pkmn::pokemon::sptr& egg = daycare->get_egg();
        ASSERT_EQ(pkmn::e_species::NONE, egg->get_species());
        ASSERT_TRUE(egg->is_egg());
    }
    else
    {
        ASSERT_EQ(0, daycare->get_breeding_pokemon_capacity());

        ASSERT_THROW(
            daycare->get_breeding_pokemon(0);
        , pkmn::feature_not_in_game_error);
        ASSERT_THROW(
            daycare->get_breeding_pokemon_as_vector();
        , pkmn::feature_not_in_game_error);

        ASSERT_THROW(
            daycare->set_breeding_pokemon(
                0,
                pkmn::pokemon::make(pkmn::e_species::BULBASAUR, game, "", 5)
            );
        , pkmn::feature_not_in_game_error);

        ASSERT_THROW(
            daycare->get_egg()
        , pkmn::feature_not_in_game_error);
    }
}

TEST_P(daycare_test, test_setting_pokemon)
{
    test_params_t test_params = GetParam();
    pkmn::e_game game = std::get<0>(test_params);

    pkmn::daycare::sptr daycare = pkmn::daycare::make(game);

    const pkmn::pokemon_list_t& levelup_pokemon =
        daycare->get_levelup_pokemon_as_vector();

    pkmn::pokemon::sptr venusaur = pkmn::pokemon::make(
                                       pkmn::e_species::VENUSAUR,
                                       game,
                                       "",
                                       50
                                   );

    daycare->set_levelup_pokemon(0, venusaur);
    ASSERT_EQ(
        pkmn::e_species::VENUSAUR,
        levelup_pokemon[0]->get_species()
    );
    ASSERT_NE(
        venusaur->get_native_pc_data(),
        levelup_pokemon[0]->get_native_pc_data()
    );

    if(daycare->get_levelup_pokemon_capacity() == 2)
    {
        pkmn::pokemon::sptr charizard = pkmn::pokemon::make(
                                            pkmn::e_species::CHARIZARD,
                                            game,
                                            "",
                                            50
                                        );

        daycare->set_levelup_pokemon(1, charizard);
        ASSERT_EQ(
            pkmn::e_species::CHARIZARD,
            levelup_pokemon[1]->get_species()
        );
        ASSERT_NE(
            charizard->get_native_pc_data(),
            levelup_pokemon[1]->get_native_pc_data()
        );
    }

    if(daycare->can_breed_pokemon())
    {
        int generation = pkmn::priv::game_enum_to_generation(game);

        const pkmn::pokemon_list_t& breeding_pokemon =
            daycare->get_breeding_pokemon_as_vector();

        if(generation >= 7)
        {
            ASSERT_NE(
                levelup_pokemon[0],
                breeding_pokemon[0]
            );
            ASSERT_NE(
                levelup_pokemon[1],
                breeding_pokemon[1]
            );
        }
        else
        {
            ASSERT_EQ(
                levelup_pokemon[0],
                breeding_pokemon[0]
            );
            ASSERT_EQ(
                levelup_pokemon[1],
                breeding_pokemon[1]
            );
        }

        // TODO: validate genders
        pkmn::pokemon::sptr blastoise = pkmn::pokemon::make(
                                            pkmn::e_species::BLASTOISE,
                                            game,
                                            "",
                                            50
                                        );
        daycare->set_breeding_pokemon(0, blastoise);
        ASSERT_EQ(
            pkmn::e_species::BLASTOISE,
            blastoise->get_species()
        );
        ASSERT_NE(
            blastoise->get_native_pc_data(),
            breeding_pokemon[0]->get_native_pc_data()
        );
        breeding_pokemon[0]->set_gender(pkmn::e_gender::FEMALE);

        pkmn::pokemon::sptr marowak = pkmn::pokemon::make(
                                          pkmn::e_species::MAROWAK,
                                          game,
                                          "",
                                          50
                                      );
        daycare->set_breeding_pokemon(1, marowak);
        ASSERT_EQ(
            pkmn::e_species::MAROWAK,
            marowak->get_species()
        );
        ASSERT_NE(
            marowak->get_native_pc_data(),
            breeding_pokemon[1]->get_native_pc_data()
        );
        breeding_pokemon[1]->set_gender(pkmn::e_gender::MALE);

        ASSERT_TRUE(daycare->get_egg()->is_egg());
    }
}

// TODO: test breeding

static const std::vector<test_params_t> TEST_PARAMS =
{
    // Generation I
    test_params_t(pkmn::e_game::RED,   false),
    test_params_t(pkmn::e_game::BLUE,  false),
    test_params_t(pkmn::e_game::YELLOW, false),

    // Generation II
    test_params_t(pkmn::e_game::GOLD,    true),
    test_params_t(pkmn::e_game::SILVER,  true),
    test_params_t(pkmn::e_game::CRYSTAL, true),

    // Gamecube
    test_params_t(pkmn::e_game::COLOSSEUM, false),
    test_params_t(pkmn::e_game::XD,        false),
};

INSTANTIATE_TEST_CASE_P(
    cpp_daycare_test,
    daycare_test,
    ::testing::ValuesIn(TEST_PARAMS)
);
