/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "../database/database_common.hpp"

#include <boost/config.hpp>

namespace pkmn { namespace conversions {

    int convert_pokemon_game_index(
        int pokemon_index,
        int from_game_id,
        int to_game_id
    )
    {
        static BOOST_CONSTEXPR const char* query =
            "SELECT game_index FROM pokemon_game_indices WHERE version_id=? "
            "AND pokemon_id=(SELECT pokemon_id FROM pokemon_game_indices WHERE "
            "game_index=? AND version_id=?)";

        return pkmn::database::query_db_bind3<int, int, int, int>(
                   query, to_game_id, pokemon_index, from_game_id
               );
    }

    int pokemon_index_to_base_friendship(
        int pokemon_index,
        int game_id
    )
    {
        static BOOST_CONSTEXPR const char* query =
            "SELECT base_happiness FROM pokemon_species WHERE id="
            "(SELECT species_id FROM pokemon WHERE id="
            "(SELECT pokemon_id FROM pokemon_game_indices WHERE "
            "version_id=? AND game_index=?))";

        return pkmn::database::query_db_bind2<int, int, int>(
                   query, game_id, pokemon_index
               );
    }

}}
