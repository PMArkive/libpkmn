/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"

#include <pkmn/database/lists.hpp>

#include <boost/config.hpp>

#include <stdexcept>

namespace pkmn { namespace database {

    // TODO
    template <typename ret_type, typename bind1_type>
    void pkmn_db_query_list_bind1(
        const char* query,
        std::vector<ret_type> &ret_vec,
        bind1_type bind1
    ) {
        (void)query;
        (void)ret_vec;
        (void)bind1;
    }

    // TODO
    template <typename ret_type, typename bind1_type, typename bind2_type>
    void pkmn_db_query_list_bind2(
        const char* query,
        std::vector<ret_type> &ret_vec,
        bind1_type bind1,
        bind2_type bind2
    ) {
        (void)query;
        (void)ret_vec;
        (void)bind1;
        (void)bind2;
    }

    // TODO
    template <typename ret_type, typename bind1_type, typename bind2_type, typename bind3_type>
    void pkmn_db_query_list_bind3(
        const char* query,
        std::vector<ret_type> &ret_vec,
        bind1_type bind1,
        bind1_type bind2,
        bind1_type bind3
    ) {
        (void)query;
        (void)ret_vec;
        (void)bind1;
        (void)bind2;
        (void)bind3;
    }

    std::vector<std::string> get_ability_list(
        int latest_generation,
        const std::string &language
    ) {
        if(latest_generation < 3 or latest_generation > 6) {
            throw std::out_of_range("latest_generation: valid range 3-6");
        }

        // TODO: valid language check
        (void)language;
        int language_id = 9;

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM ability_names WHERE local_language_id=? AND "
            "ability_id IN (SELECT id FROM abilities WHERE generation_id<=? "
            "AND is_main_series=1) ORDER BY ability_id";

        std::vector<std::string> ret;
        pkmn_db_query_list_bind2<std::string, int, int>(
            query, ret, language_id, latest_generation
        );

        return ret;
    }

    std::vector<std::string> get_game_list(
        int generation,
        bool include_previous,
        const std::string &language
    ) {
        if(generation < 1 or generation > 6) {
            throw std::out_of_range("generation: valid range 1-6");
        }

        // TODO: valid language check
        (void)language;
        int language_id = 9;

        static BOOST_CONSTEXPR const char* with_previous_query = \
            "SELECT name FROM version_names WHERE local_language_id=? AND version_id IN "
            "(SELECT id FROM versions WHERE version_group_id IN "
            "(SELECT id FROM version_groups WHERE generation_id<=?))";

        static BOOST_CONSTEXPR const char* no_previous_query = \
            "SELECT name FROM version_names WHERE local_language_id=? AND version_id IN "
            "(SELECT id FROM versions WHERE version_group_id IN "
            "(SELECT id FROM version_groups WHERE generation_id=?))";

        std::vector<std::string> ret;
        pkmn_db_query_list_bind1<std::string, int>(
            (include_previous ? with_previous_query : no_previous_query),
            ret, language_id
        );

        return ret;
    }

    std::vector<std::string> get_item_list(
        const std::string &game,
        const std::string &language
    ) {
        (void)game;
        return std::vector<std::string>();
    }

    std::vector<std::string> get_location_list(
        const std::string &game,
        bool whole_generation,
        const std::string &language
    ) {
        (void)game;
        (void)whole_generation;
        return std::vector<std::string>();
    }

    std::vector<std::string> get_move_list(
        int latest_generation,
        const std::string &language
    ) {
        (void)latest_generation;
        return std::vector<std::string>();
    }

    std::vector<std::string> get_nature_list(
        const std::string &language
    ) {
        return std::vector<std::string>();
    }

    std::vector<std::string> get_pokemon_list(
        int generation,
        bool include_previous,
        const std::string &language
    ) {
        (void)generation;
        (void)include_previous;
        return std::vector<std::string>();
    }

    std::vector<std::string> get_region_list(
        const std::string &language
    ) {
        // TODO: valid language check
        (void)language;
        int language_id = 9;

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM region_names WHERE local_language_id=9 "
            "ORDER BY region_id";

        std::vector<std::string> ret;
        pkmn_db_query_list_bind1<std::string, int>(
            query, ret, language_id
        );

        return ret;
    }

    std::vector<std::string> get_ribbon_list(
        int generation,
        const std::string &language
    ) {
        (void)generation;
        return std::vector<std::string>();
    }

    std::vector<std::string> get_super_training_medal_list(
        const std::string &language
    ) {
        return std::vector<std::string>();
    }

    std::vector<std::string> get_type_list(
        int generation,
        const std::string &language
    ) {
        (void)generation;
        return std::vector<std::string>();
    }
}}
