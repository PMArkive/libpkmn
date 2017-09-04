/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_CALCULATIONS_DAMAGE_MOVES_HPP
#define PKMN_CALCULATIONS_DAMAGE_MOVES_HPP

#include <pkmn/config.hpp>

namespace pkmn { namespace calculations {

    PKMN_API int crush_grip_power(
        int target_current_hp,
        int target_max_hp,
        bool is_generation4
    );

    PKMN_API int electro_ball_power(
        int attacker_speed,
        int target_speed
    );

    PKMN_INLINE int eruption_power(
        int attacker_hp,
        int target_hp
    )
    {
        return std::max<int>(
                   1,
                   int(150.0f * (float(attacker_hp) / float(target_hp)))
               );
    }

    PKMN_API int flail_power(
        int attacker_current_hp,
        int attacker_max_hp
    );

    PKMN_API int fling_power(
        const std::string& held_item
    );

    PKMN_API int frustration_power(
        int friendship
    );

    PKMN_API int grass_knot_power(
        float target_weight
    );

    PKMN_CONSTEXPR_OR_INLINE int gyro_ball_power(
        int attacker_speed,
        int target_speed
    )
    {
        return int(25.0f * (float(target_speed) / float(attacker_speed)));
    }

    PKMN_API int heat_crash_power(
        float attacker_weight,
        float target_weight
    );

    PKMN_API int heavy_slam_power(
        float attacker_weight,
        float target_weight
    );

    PKMN_API int low_kick_power(
        int generation,
        float target_weight
    );

    // TODO: Natural Gift

    PKMN_API int power_trip_power(
        int attack_stat_stage,
        int defense_stat_stage,
        int special_attack_stat_stage,
        int special_defense_stat_stage,
        int speed_stat_stage,
        int evasion_stat_stage,
        int accuracy_stat_stage
    );

    PKMN_API int punishment_power(
        int attack_stat_stage,
        int defense_stat_stage,
        int special_attack_stat_stage,
        int special_defense_stat_stage,
        int speed_stat_stage,
        int evasion_stat_stage,
        int accuracy_stat_stage
    );

    PKMN_API int return_power(
        int friendship
    );

    PKMN_API int reversal_power(
        int attacker_current_hp,
        int attacker_max_hp
    );

    PKMN_API int spit_up_power(
        int stockpile_amount
    );

    PKMN_API int stored_power_power(
        int attack_stat_stage,
        int defense_stat_stage,
        int special_attack_stat_stage,
        int special_defense_stat_stage,
        int speed_stat_stage,
        int evasion_stat_stage,
        int accuracy_stat_stage
    );

    PKMN_API int trump_card_power(
        int pp_remaining_after_use
    );

    PKMN_API int water_spout_power(
        int attacker_current_hp,
        int attacker_max_hp
    );

    PKMN_API int wring_out_power(
        int target_current_hp,
        int target_max_hp,
        bool is_generation4
    );

}}
#endif /* PKMN_CALCULATIONS_DAMAGE_MOVES_HPP */
