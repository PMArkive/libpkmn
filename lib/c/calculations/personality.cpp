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
    const char* p_species,
    uint32_t trainer_id,
    bool shiny,
    const char* p_ability,
    enum pkmn_gender gender,
    const char* p_nature,
    uint32_t* p_personality_out
)
{
    PKMN_CHECK_NULL_PARAM(p_species);
    PKMN_CHECK_NULL_PARAM(p_ability);
    PKMN_CHECK_NULL_PARAM(p_nature);
    PKMN_CHECK_NULL_PARAM(p_personality_out);

    PKMN_CPP_TO_C(
        *p_personality_out = pkmn::calculations::generate_personality(
                                 p_species,
                                 trainer_id,
                                 shiny,
                                 p_ability,
                                 static_cast<pkmn::e_gender>(gender),
                                 p_nature
                             );
    )
}
