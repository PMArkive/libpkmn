/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/calculations/damage/critical_hit.h>

#include <pkmn/calculations/damage/critical_hit.hpp>

pkmn_error_t pkmn_calculations_gen1_critical_hit_chance(
    int speed,
    bool rate_increased,
    bool high_rate_move,
    float* critical_hit_chance_out
)
{
    PKMN_CHECK_NULL_PARAM(critical_hit_chance_out);

    PKMN_CPP_TO_C(
        *critical_hit_chance_out = pkmn::calculations::gen1_critical_hit_chance(
                                       speed,
                                       rate_increased,
                                       high_rate_move
                                   );
    )
}

pkmn_error_t pkmn_calculations_stadium_critical_hit_chance(
    int speed,
    bool rate_increased,
    bool high_rate_move,
    float* critical_hit_chance_out
)
{
    PKMN_CHECK_NULL_PARAM(critical_hit_chance_out);

    PKMN_CPP_TO_C(
        *critical_hit_chance_out = pkmn::calculations::stadium_critical_hit_chance(
                                       speed,
                                       rate_increased,
                                       high_rate_move
                                   );
    )
}

pkmn_error_t pkmn_calculations_gen1_critical_hit_modifier(
    int attacker_level,
    float* critical_hit_modifier_out
)
{
    PKMN_CHECK_NULL_PARAM(critical_hit_modifier_out);

    PKMN_CPP_TO_C(
        *critical_hit_modifier_out = pkmn::calculations::gen1_critical_hit_modifier(
                                         attacker_level
                                     );
    )
}

pkmn_error_t pkmn_calculations_critical_hit_modifier(
    bool before_gen6,
    float* critical_hit_modifier_out
)
{
    PKMN_CHECK_NULL_PARAM(critical_hit_modifier_out);

    PKMN_CPP_TO_C(
        *critical_hit_modifier_out = pkmn::calculations::critical_hit_modifier(
                                         before_gen6
                                     );
    )
}
