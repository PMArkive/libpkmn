/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_DATABASE_DATABASE_COMMON_HPP
#define PKMN_DATABASE_DATABASE_COMMON_HPP

#include <sstream>
#include <stdexcept>
#include <string>

/*
 * TODO:
 *
 * Get valid database connection


#define GET_DATABASE_CONNECTION(db) \
    if(!db) { \
        db = pkmn::database::get_database_connection(); \
    }
*/

/*
 * TODO: pass database sptr into function
 */

template <typename ret_type>
ret_type pkmn_db_query(
    const char* query
) {
    (void)query;

    ret_type ret;
    (void)ret;
    return ret;
}

template <typename ret_type, typename bind1_type>
ret_type pkmn_db_query_bind1(
    const char* query,
    bind1_type bind1
) {
    (void)query;
    (void)bind1;

    ret_type ret;
    (void)ret;
    return ret;
}

template <typename ret_type, typename bind1_type, typename bind2_type>
ret_type pkmn_db_query_bind2(
    const char* query,
    bind1_type bind1,
    bind2_type bind2
) {
    (void)query;
    (void)bind1;
    (void)bind2;

    ret_type ret;
    (void)ret;
    return ret;
}

template <typename ret_type, typename bind1_type, typename bind2_type, typename bind3_type>
ret_type pkmn_db_query_bind3(
    const char* query,
    bind1_type bind1,
    bind2_type bind2,
    bind3_type bind3
) {
    (void)query;
    (void)bind1;
    (void)bind2;
    (void)bind3;

    ret_type ret;
    (void)ret;
    return ret;
}

namespace pkmn { namespace database {

    /*
     * TODO:
     *  * get_database_connection
     *  * Templated functions for querying
     */

    /*
     * Workarounds for Veekun database oddities
     */

    std::string fix_veekun_whitespace(
        const std::string &input
    );

    std::string fix_location_string(
        const std::string &original_string,
        int location_id,
        int game_id,
        bool whole_generation
    );

}}

#endif /* PKMN_DATABASE_DATABASE_COMMON_HPP */
