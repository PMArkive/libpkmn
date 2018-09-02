/*
 * Copyright (c) 2017,2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "enum_maps.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/personality.h>

#include <pkmn/calculations/personality.hpp>

enum pkmn_error pkmn_calculations_generate_personality(
    enum pkmn_species species,
    uint32_t trainer_id,
    bool shiny,
    enum pkmn_ability ability,
    enum pkmn_gender gender,
    enum pkmn_nature nature,
    uint32_t* p_personality_out
)
{
    enum pkmn_error error = pkmn::c::check_for_null_param(
                                p_personality_out,
                                "p_personality_out"
                            );
    if(!error)
    {
        auto impl = [&]()
        {
            *p_personality_out = pkmn::calculations::generate_personality(
                                     static_cast<pkmn::e_species>(species),
                                     trainer_id,
                                     shiny,
                                     static_cast<pkmn::e_ability>(ability),
                                     static_cast<pkmn::e_gender>(gender),
                                     static_cast<pkmn::e_nature>(nature)
                                 );
        };

        error = pkmn::c::handle_exceptions(impl);
    }

    return error;
}
