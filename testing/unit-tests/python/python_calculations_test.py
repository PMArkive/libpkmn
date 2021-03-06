#!/usr/bin/env python
#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

import pkmn

import random
import sys
import unittest

class calculations_test(unittest.TestCase):

    def test_stab_modifier(self):
        self.assertEqual(
            pkmn.calculations.STAB_MODIFIER,
            1.5
        )

    def test_brine_power(self):
        # Test invalid inputs.

        with self.assertRaises(IndexError):
            pkmn.calculations.brine_power(0, 2)
        with self.assertRaises(IndexError):
            pkmn.calculations.brine_power(0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.brine_power(-2, -1)
        with self.assertRaises(IndexError):
            pkmn.calculations.brine_power(2, 1)

        # Test expected results.

        max_hp = 10
        half_hp = max_hp // 2

        self.assertEqual(
            pkmn.calculations.brine_power(half_hp-1, max_hp),
            130
        )
        self.assertEqual(
            pkmn.calculations.brine_power(half_hp, max_hp),
            130
        )
        self.assertEqual(
            pkmn.calculations.brine_power(half_hp+1, max_hp),
            65
        )

    def test_crush_grip_power(self):
        # Test invalid inputs.

        with self.assertRaises(IndexError):
            pkmn.calculations.crush_grip_power(0, 1, 5)
        with self.assertRaises(IndexError):
            pkmn.calculations.crush_grip_power(2, 1, 5)
        with self.assertRaises(IndexError):
            pkmn.calculations.crush_grip_power(1, 2, 3)
        with self.assertRaises(IndexError):
            pkmn.calculations.crush_grip_power(1, 2, 7)

        # Test expected results.

        self.assertEqual(
            pkmn.calculations.crush_grip_power(1, 200, 4),
            1
        )
        self.assertEqual(
            pkmn.calculations.crush_grip_power(200, 200, 4),
            121
        )

        for generation in [5,6]:
            self.assertEqual(
                pkmn.calculations.crush_grip_power(1, 200, generation),
                1
            )
            self.assertEqual(
                pkmn.calculations.crush_grip_power(200, 200, generation),
                120
            )

    def test_echoed_voice_powers(self):
        self.assertEquals(
            list(pkmn.calculations.echoed_voice_powers()),
            [40, 80, 120, 160, 200]
        )

    def test_electro_ball_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.electro_ball_power(0, 100)
        with self.assertRaises(IndexError):
            pkmn.calculations.electro_ball_power(100, 0)

        # Test expected results.
        attacker_speeds = [100, 100, 100, 100, 100, 100, 100, 100]
        target_speeds = [101, 100, 51, 50, 34, 33, 32, 25]
        expected_powers = [40, 60, 60, 80, 80, 120, 120, 150]
        test_cases = zip(attacker_speeds, target_speeds, expected_powers)

        for attacker_speed, target_speed, expected_power in test_cases:
            self.assertEqual(
                pkmn.calculations.electro_ball_power(
                    attacker_speed,
                    target_speed
                ),
                expected_power
            )

    def test_eruption_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.eruption_power(0, 1)
        with self.assertRaises(IndexError):
            pkmn.calculations.eruption_power(1, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.eruption_power(2, 1)

        # Test expected results.

        self.assertEqual(pkmn.calculations.eruption_power(1, 500), 1)
        self.assertEqual(pkmn.calculations.eruption_power(250, 500), 75)
        self.assertEqual(pkmn.calculations.eruption_power(500, 500), 150)

    def test_flail_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.flail_power(0, 1)
        with self.assertRaises(IndexError):
            pkmn.calculations.flail_power(1, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.flail_power(2, 1)

        attacker_current_HPs = [1, 20, 21, 52, 53, 104, 105, 177, 178, 343, 344, 500]
        attacker_max_HPs = [500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500, 500]
        expected_powers = [200, 200, 150, 150, 100, 100, 80, 80, 40, 40, 20, 20]
        test_cases = zip(attacker_current_HPs, attacker_max_HPs, expected_powers)

        for attacker_current_hp, attacker_max_hp, expected_power in test_cases:
            self.assertEqual(
                pkmn.calculations.flail_power(
                    attacker_current_hp,
                    attacker_max_hp
                ),
                expected_power
            )

    def test_fling_power(self):
        # Test invalid parameters.

        with self.assertRaises(ValueError):
            pkmn.calculations.fling_power(pkmn.item.INVALID)

        # Test expected results.
        items = [
            pkmn.item.ORAN_BERRY,
            pkmn.item.HEALTH_WING,
            pkmn.item.POTION,
            pkmn.item.ICY_ROCK,
            pkmn.item.DUBIOUS_DISC,
            pkmn.item.DAMP_ROCK,
            pkmn.item.DRAGON_FANG,
            pkmn.item.DUSK_STONE,
            pkmn.item.THICK_CLUB,
            pkmn.item.RARE_BONE,
            pkmn.item.IRON_BALL
        ]
        expected_powers = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 130]
        test_cases = zip(items, expected_powers)

        for item, expected_power in test_cases:
            self.assertEqual(
                pkmn.calculations.fling_power(item),
                expected_power
            )

    def test_frustration_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.frustration_power(-1)
        with self.assertRaises(IndexError):
            pkmn.calculations.frustration_power(256)

        # Test expected values.

        self.assertEqual(pkmn.calculations.frustration_power(0), 102)
        self.assertEqual(pkmn.calculations.frustration_power(255), 1)

    def test_fury_cutter_powers(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.fury_cutter_powers(1)
        with self.assertRaises(IndexError):
            pkmn.calculations.fury_cutter_powers(10)

        expected_gen_2_4_powers = [10, 20, 40, 80, 160]
        expected_gen5_powers = [20, 40, 80, 160]
        expected_gen6_powers = [40, 80, 160]

        for generation in range(2,5):
            self.assertEqual(
                list(pkmn.calculations.fury_cutter_powers(generation)),
                expected_gen_2_4_powers
            )

        self.assertEqual(
            list(pkmn.calculations.fury_cutter_powers(5)),
            expected_gen5_powers
        )
        self.assertEqual(
            list(pkmn.calculations.fury_cutter_powers(6)),
            expected_gen6_powers
        )

    def test_grass_knot_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.grass_knot_power(-1.0)

        # Test expected results.

        target_weights = [5.0, 15.0, 35.0, 75.0, 150.0, 250.0]
        expected_powers = [20, 40, 60, 80, 100, 120]
        test_cases = zip(target_weights, expected_powers)

        for target_weight, expected_power in test_cases:
            self.assertEqual(
                pkmn.calculations.grass_knot_power(target_weight),
                expected_power
            )

    def test_gyro_ball_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.gyro_ball_power(0, 50)
        with self.assertRaises(IndexError):
            pkmn.calculations.gyro_ball_power(50, 0)

        # Test expected values.

        self.assertEqual(
            pkmn.calculations.gyro_ball_power(1, 500),
            150
        )
        self.assertEqual(
            pkmn.calculations.gyro_ball_power(100, 200),
            50
        )
        self.assertEqual(
            pkmn.calculations.gyro_ball_power(500, 1),
            1
        )

    def test_heat_crash_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.heat_crash_power(0.0, 1.0)
        with self.assertRaises(IndexError):
            pkmn.calculations.heat_crash_power(1.0, 0.0)

        # Test expected values.

        attacker_weights = [200.0, 200.0, 200.0,
                            200.0, 200.0, 200.0,
                            200.0, 200.0, 200.0]
        target_weights = [200.0, 100.1, 100.0,
                          66.7, 66.6, 50.1,
                          50.0, 40.1, 40.0]
        expected_powers = [40, 40, 60,
                           60, 80, 80,
                           100, 100, 120]
        test_cases = zip(attacker_weights, target_weights, expected_powers)

        for attacker_weight, target_weight, expected_power in test_cases:
            self.assertEqual(
                pkmn.calculations.heat_crash_power(
                    attacker_weight,
                    target_weight
                ),
                expected_power
            )

    def test_heavy_slam_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.heavy_slam_power(0.0, 1.0)
        with self.assertRaises(IndexError):
            pkmn.calculations.heavy_slam_power(1.0, 0.0)

        # Test expected values.

        attacker_weights = [200.0, 200.0, 200.0,
                            200.0, 200.0, 200.0,
                            200.0, 200.0, 200.0]
        target_weights = [200.0, 100.1, 100.0,
                          66.7, 66.6, 50.1,
                          50.0, 40.1, 40.0]
        expected_powers = [40, 40, 60,
                           60, 80, 80,
                           100, 100, 120]
        test_cases = zip(attacker_weights, target_weights, expected_powers)

        for attacker_weight, target_weight, expected_power in test_cases:
            self.assertEqual(
                pkmn.calculations.heavy_slam_power(
                    attacker_weight,
                    target_weight
                ),
                expected_power
            )

    def test_ice_ball_powers(self):
        self.assertEquals(
            list(pkmn.calculations.ice_ball_powers()),
            [30, 60, 120, 240, 480]
        )

    def test_low_kick_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.low_kick_power(-1.0, 1)
        with self.assertRaises(IndexError):
            pkmn.calculations.low_kick_power(1.0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.low_kick_power(1.0, 10)

        # Test expected values.

        target_weights = [0.1, 9.9, 10.0, 24.9, 25.0, 49.9,
                          50.0, 99.9, 100.0, 199.9, 200.0, 1000.0];
        expected_powers = [20, 20, 40, 40, 60, 60,
                           80, 80, 100, 100, 120, 120];
        test_cases = zip(target_weights, expected_powers)

        for target_weight, expected_power in test_cases:
            for generation in range(1, 7):
                if generation <= 2:
                    # Generation I has only one set power, regardless of weight.
                    self.assertEqual(
                        pkmn.calculations.low_kick_power(
                            target_weight,
                            generation
                        ),
                        50
                    )
                else:
                    self.assertEqual(
                        pkmn.calculations.low_kick_power(
                            target_weight,
                            generation
                        ),
                        expected_power
                    )

    def test_power_trip_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.power_trip_power(-1, 0, 0, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.power_trip_power(7, 0, 0, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.power_trip_power(0, -1, 0, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.power_trip_power(0, 7, 0, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.power_trip_power(0, 0, -1, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.power_trip_power(0, 0, 7, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.power_trip_power(0, 0, 0, -1, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.power_trip_power(0, 0, 0, 7, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.power_trip_power(0, 0, 0, 0, -1, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.power_trip_power(0, 0, 0, 0, 7, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.power_trip_power(0, 0, 0, 0, 0, -1, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.power_trip_power(0, 0, 0, 0, 0, 7, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.power_trip_power(0, 0, 0, 0, 0, 0, -1)
        with self.assertRaises(IndexError):
            pkmn.calculations.power_trip_power(0, 0, 0, 0, 0, 0, 7)

        # Test expected results.

        self.assertEqual(
            pkmn.calculations.power_trip_power(0, 0, 0, 0, 0, 0, 0),
            20
        )
        self.assertEqual(
            pkmn.calculations.power_trip_power(0, 1, 2, 3, 4, 5, 6),
            440
        )
        self.assertEqual(
            pkmn.calculations.power_trip_power(6, 6, 6, 6, 6, 6, 6),
            860
        )

    def test_punishment_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.punishment_power(-1, 0, 0, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.punishment_power(7, 0, 0, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.punishment_power(0, -1, 0, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.punishment_power(0, 7, 0, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.punishment_power(0, 0, -1, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.punishment_power(0, 0, 7, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.punishment_power(0, 0, 0, -1, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.punishment_power(0, 0, 0, 7, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.punishment_power(0, 0, 0, 0, -1, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.punishment_power(0, 0, 0, 0, 7, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.punishment_power(0, 0, 0, 0, 0, -1, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.punishment_power(0, 0, 0, 0, 0, 7, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.punishment_power(0, 0, 0, 0, 0, 0, -1)
        with self.assertRaises(IndexError):
            pkmn.calculations.punishment_power(0, 0, 0, 0, 0, 0, 7)

        # Test expected results.

        self.assertEqual(
            pkmn.calculations.punishment_power(0, 0, 0, 0, 0, 0, 0),
            60
        )
        self.assertEqual(
            pkmn.calculations.punishment_power(0, 1, 2, 3, 4, 5, 6),
            200
        )
        self.assertEqual(
            pkmn.calculations.punishment_power(6, 6, 6, 6, 6, 6, 6),
            200
        )

    def test_return_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.return_power(-1)
        with self.assertRaises(IndexError):
            pkmn.calculations.return_power(256)

        # Test expected results.

        self.assertEqual(pkmn.calculations.return_power(0), 1)
        self.assertEqual(pkmn.calculations.return_power(255), 102)

    def test_reversal_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.reversal_power(0, 1)
        with self.assertRaises(IndexError):
            pkmn.calculations.reversal_power(1, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.reversal_power(2, 1)

        # Test expected results.

        attacker_current_HPs = [1, 20, 21, 52, 53, 104,
                                105, 177, 178, 343, 344, 500]
        attacker_max_HPs = [500, 500, 500, 500, 500, 500,
                            500, 500, 500, 500, 500, 500]
        expected_powers = [200, 200, 150, 150, 100, 100,
                           80, 80, 40, 40, 20, 20]
        test_cases = zip(attacker_current_HPs, attacker_max_HPs, expected_powers)

        for attacker_current_hp, attacker_max_hp, expected_power in test_cases:
            self.assertEqual(
                pkmn.calculations.reversal_power(
                    attacker_current_hp,
                    attacker_max_hp
                ),
                expected_power
            )

    def test_rollout_powers(self):
        self.assertEquals(
            list(pkmn.calculations.rollout_powers()),
            [30, 60, 120, 240, 480]
        )

    def test_spit_up_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.spit_up_power(-1)
        with self.assertRaises(IndexError):
            pkmn.calculations.spit_up_power(4)

        # Test expected results.

        self.assertEqual(pkmn.calculations.spit_up_power(0), 0)
        self.assertEqual(pkmn.calculations.spit_up_power(1), 100)
        self.assertEqual(pkmn.calculations.spit_up_power(2), 200)
        self.assertEqual(pkmn.calculations.spit_up_power(3), 300)

    def test_stored_power_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.stored_power_power(-1, 0, 0, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.stored_power_power(7, 0, 0, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.stored_power_power(0, -1, 0, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.stored_power_power(0, 7, 0, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.stored_power_power(0, 0, -1, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.stored_power_power(0, 0, 7, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.stored_power_power(0, 0, 0, -1, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.stored_power_power(0, 0, 0, 7, 0, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.stored_power_power(0, 0, 0, 0, -1, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.stored_power_power(0, 0, 0, 0, 7, 0, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.stored_power_power(0, 0, 0, 0, 0, -1, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.stored_power_power(0, 0, 0, 0, 0, 7, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.stored_power_power(0, 0, 0, 0, 0, 0, -1)
        with self.assertRaises(IndexError):
            pkmn.calculations.stored_power_power(0, 0, 0, 0, 0, 0, 7)

        # Test expected results.

        self.assertEqual(
            pkmn.calculations.stored_power_power(0, 0, 0, 0, 0, 0, 0),
            20
        )
        self.assertEqual(
            pkmn.calculations.stored_power_power(0, 1, 2, 3, 4, 5, 6),
            440
        )
        self.assertEqual(
            pkmn.calculations.stored_power_power(6, 6, 6, 6, 6, 6, 6),
            860
        )

    def test_triple_kick_powers(self):
        self.assertEquals(
            list(pkmn.calculations.triple_kick_powers()),
            [10, 20, 30]
        )

    def test_trump_card_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.trump_card_power(-1)
        with self.assertRaises(IndexError):
            pkmn.calculations.trump_card_power(9)

        # Test expected results.

        expected_results = [200, 80, 60, 50, 40, 40, 40, 40, 40]

        for remaining_pp in range(len(expected_results)):
            self.assertEqual(
                pkmn.calculations.trump_card_power(remaining_pp),
                expected_results[remaining_pp]
            )

    def test_water_spout_power(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.water_spout_power(0, 1)
        with self.assertRaises(IndexError):
            pkmn.calculations.water_spout_power(1, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.water_spout_power(2, 1)

        # Test expected results.

        self.assertEqual(pkmn.calculations.water_spout_power(1, 500), 1)
        self.assertEqual(pkmn.calculations.water_spout_power(250, 500), 75)
        self.assertEqual(pkmn.calculations.water_spout_power(500, 500), 150)

    def test_wring_out_power(self):
        # Test invalid inputs.

        with self.assertRaises(IndexError):
            pkmn.calculations.wring_out_power(0, 1, 5)
        with self.assertRaises(IndexError):
            pkmn.calculations.wring_out_power(2, 1, 5)
        with self.assertRaises(IndexError):
            pkmn.calculations.wring_out_power(1, 2, 3)
        with self.assertRaises(IndexError):
            pkmn.calculations.wring_out_power(1, 2, 7)

        # Test expected results.

        self.assertEqual(
            pkmn.calculations.wring_out_power(1, 200, 4),
            1
        )
        self.assertEqual(
            pkmn.calculations.wring_out_power(200, 200, 4),
            121
        )

        for generation in [5,6]:
            self.assertEqual(
                pkmn.calculations.wring_out_power(1, 200, generation),
                1
            )
            self.assertEqual(
                pkmn.calculations.wring_out_power(200, 200, generation),
                120
            )

    def test_gen1_critical_hit_chance(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.gen1_critical_hit_chance(0, False, False)
        with self.assertRaises(IndexError):
            pkmn.calculations.gen1_critical_hit_chance(256, False, False)

        # In Generation I, make sure there is never a guaranteed chance for
        # a critical hit.

        self.assertLessEqual(
            pkmn.calculations.gen1_critical_hit_chance(255, True, True),
            (255.0 / 256.0)
        )

    def test_critical_hit_chance(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.critical_hit_chance(1, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.critical_hit_chance(10, 0)
        with self.assertRaises(IndexError):
            pkmn.calculations.critical_hit_chance(2, -1)

        # Test expected values.

        generations = [2, 2, 2, 2, 2,
                       3, 3, 3, 3, 3,
                       4, 4, 4, 4, 4,
                       5, 5, 5, 5, 5,
                       6, 6, 6, 6, 6]
        stages = [0, 1, 2, 3, 4,
                  0, 1, 2, 3, 4,
                  0, 1, 2, 3, 4,
                  0, 1, 2, 3, 4,
                  0, 1, 2, 3, 4]
        expected_results = [0.0625, 0.125, 0.25, 0.333, 0.5,
                            0.0625, 0.125, 0.25, 0.333, 0.5,
                            0.0625, 0.125, 0.25, 0.333, 0.5,
                            0.0625, 0.125, 0.25, 0.333, 0.5,
                            0.0625, 0.125, 0.5,  1.0,   1.0]
        test_cases = zip(generations, stages, expected_results)

        for generation, stage, expected_result in test_cases:
            self.assertAlmostEqual(
                pkmn.calculations.critical_hit_chance(
                    generation,
                    stage
                ),
                expected_result
            )

    def test_gen1_critical_hit_modifier(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.gen1_critical_hit_modifier(-1)
        with self.assertRaises(IndexError):
            pkmn.calculations.gen1_critical_hit_modifier(256)

        # Test expected results.

        self.assertAlmostEqual(
            pkmn.calculations.gen1_critical_hit_modifier(5),
            1.5
        )
        self.assertAlmostEqual(
            pkmn.calculations.gen1_critical_hit_modifier(20),
            1.8
        )
        self.assertAlmostEqual(
            pkmn.calculations.gen1_critical_hit_modifier(95),
            1.95
        )

    def test_critical_hit_modifier(self):
        # Test invalid parameters.

        with self.assertRaises(IndexError):
            pkmn.calculations.critical_hit_modifier(-1)
        with self.assertRaises(IndexError):
            pkmn.calculations.critical_hit_modifier(10)

        # Test expected results.

        self.assertAlmostEqual(
            pkmn.calculations.critical_hit_modifier(2),
            2.0
        )
        self.assertAlmostEqual(
            pkmn.calculations.critical_hit_modifier(3),
            2.0
        )
        self.assertAlmostEqual(
            pkmn.calculations.critical_hit_modifier(4),
            2.0
        )
        self.assertAlmostEqual(
            pkmn.calculations.critical_hit_modifier(5),
            2.0
        )
        self.assertAlmostEqual(
            pkmn.calculations.critical_hit_modifier(6),
            1.5
        )

    def test_damage(self):
        # Source: https://bulbapedia.bulbagarden.net/wiki/Damage#Example

        # Only taking types into account
        #
        # "Imagine a level 75 Glaceon...with an effective Attack stat of 123
        # uses Ice Fang (an Ice-type physical move with a power of 65) against
        # a Garchomp with an effective Defense stat of 163 in Generation VI,
        # and does not land a critical hit."
        #
        # The article itself results in the wrong value, but the value I'm
        # testing for below was based on its equations.

        level = 75
        ice_fang_base_power = 65
        modifier_against_dragon_ground = 6.0
        glaceon_l75_attack = 123
        garchomp_l75_defense = 163

        self.assertEqual(
            pkmn.calculations.damage(
                level,
                ice_fang_base_power,
                glaceon_l75_attack,
                garchomp_l75_defense,
                modifier_against_dragon_ground
            ),
            200
        )

    def test_type_damage_modifier(self):
        # Test invalid inputs.

        # Invalid generation
        with self.assertRaises(IndexError):
            pkmn.calculations.type_damage_modifier(
                -1,
                pkmn.pokemon_type.NORMAL,
                pkmn.pokemon_type.NORMAL
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.type_damage_modifier(
                8,
                pkmn.pokemon_type.NORMAL,
                pkmn.pokemon_type.NORMAL
            )

        # Invalid types for a given generation
        generations = [1, 1, 5, 3, 5, 2, 4]
        types = [
            pkmn.pokemon_type.DARK,
            pkmn.pokemon_type.STEEL,
            pkmn.pokemon_type.FAIRY,
            pkmn.pokemon_type.QUESTION_MARK,
            pkmn.pokemon_type.QUESTION_MARK,
            pkmn.pokemon_type.SHADOW,
            pkmn.pokemon_type.SHADOW
        ]
        test_cases = zip(generations, types)

        for generation, type_name in test_cases:
            # Invalid attacking type
            with self.assertRaises(ValueError):
                pkmn.calculations.type_damage_modifier(
                    generation,
                    type_name,
                    pkmn.pokemon_type.NORMAL
                )

            # Invalid defending type
            with self.assertRaises(ValueError):
                pkmn.calculations.type_damage_modifier(
                    generation,
                    pkmn.pokemon_type.NORMAL,
                    type_name
                )

        # Check that changes between generations are properly implemented.

        attacking_types = [
            pkmn.pokemon_type.BUG,
            pkmn.pokemon_type.POISON,
            pkmn.pokemon_type.GHOST,
            pkmn.pokemon_type.ICE,
            pkmn.pokemon_type.GHOST,
            pkmn.pokemon_type.DARK
        ]
        defending_types = [
            pkmn.pokemon_type.POISON,
            pkmn.pokemon_type.BUG,
            pkmn.pokemon_type.PSYCHIC,
            pkmn.pokemon_type.FIRE,
            pkmn.pokemon_type.STEEL,
            pkmn.pokemon_type.STEEL
        ]
        old_generations = [1, 1, 1, 1, 5, 5]
        old_modifiers = [2.0, 2.0, 0.0, 1.0, 0.5, 0.5]
        new_generations = [2, 2, 2, 2, 6, 6]
        new_modifiers = [0.5, 1.0, 2.0, 0.5, 1.0, 1.0]
        test_cases = zip(attacking_types, defending_types, old_generations,
                         old_modifiers, new_generations, new_modifiers)

        for attacking_type, defending_type, old_generation, old_modifier, \
            new_generation, new_modifier in test_cases:

            self.assertAlmostEqual(
                pkmn.calculations.type_damage_modifier(
                    old_generation,
                    attacking_type,
                    defending_type
                ),
                old_modifier
            )
            self.assertAlmostEqual(
                pkmn.calculations.type_damage_modifier(
                    new_generation,
                    attacking_type,
                    defending_type
                ),
                new_modifier
            )

    def test_gen2_unown_form(self):
        # Make sure expected errors are raised.
        with self.assertRaises(IndexError):
            form = pkmn.calculations.gen2_unown_form(-1, 0, 0, 0)
        with self.assertRaises(IndexError):
            form = pkmn.calculations.gen2_unown_form(16, 0, 0, 0)
        with self.assertRaises(IndexError):
            form = pkmn.calculations.gen2_unown_form(0, -1, 0, 0)
        with self.assertRaises(IndexError):
            form = pkmn.calculations.gen2_unown_form(0, 16, 0, 0)
        with self.assertRaises(IndexError):
            form = pkmn.calculations.gen2_unown_form(0, 0, -1, 0)
        with self.assertRaises(IndexError):
            form = pkmn.calculations.gen2_unown_form(0, 0, 16, 0)
        with self.assertRaises(IndexError):
            form = pkmn.calculations.gen2_unown_form(0, 0, 0, -1)
        with self.assertRaises(IndexError):
            form = pkmn.calculations.gen2_unown_form(0, 0, 0, 16)

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Unown.27s_letter
        #
        self.assertEqual(pkmn.calculations.gen2_unown_form(10, 9, 1, 14), "G")
        self.assertEqual(pkmn.calculations.gen2_unown_form(5, 15, 10, 5), "S")

    def test_gen3_unown_form(self):
        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://www.smogon.com/ingame/rng/pid_iv_creation#unown_shape
        #
        self.assertEqual(pkmn.calculations.gen3_unown_form(0x4C07DE71), "B")

    def test_wurmple_becomes_silcoon(self):
        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://www.smogon.com/ingame/rng/pid_iv_creation#silcoon_cascoon
        #
        self.assertTrue(pkmn.calculations.wurmple_becomes_silcoon(0x5CF4091C, True))
        self.assertFalse(pkmn.calculations.wurmple_becomes_silcoon(0x091C5CF4, True))
        self.assertTrue(pkmn.calculations.wurmple_becomes_silcoon(0x091C5CF4, False))
        self.assertFalse(pkmn.calculations.wurmple_becomes_silcoon(0x5CF4091C, False))

    def test_gen2_gender(self):
        # Make sure expected errors are raised.
        with self.assertRaises(ValueError):
            gender = pkmn.calculations.gen2_pokemon_gender(pkmn.species.NONE, 0)
        with self.assertRaises(IndexError):
            gender = pkmn.calculations.gen2_pokemon_gender(pkmn.species.BULBASAUR, -1)
        with self.assertRaises(IndexError):
            gender = pkmn.calculations.gen2_pokemon_gender(pkmn.species.BULBASAUR, 16)

        # Make sure known good inputs result in expected results.

        # All male
        self.assertEquals(pkmn.calculations.gen2_pokemon_gender(pkmn.species.NIDORINO, 0), pkmn.gender.MALE)
        self.assertEquals(pkmn.calculations.gen2_pokemon_gender(pkmn.species.NIDORINO, 15), pkmn.gender.MALE)

        # 25% male, 75% female
        self.assertEquals(pkmn.calculations.gen2_pokemon_gender(pkmn.species.VULPIX, 11), pkmn.gender.FEMALE)
        self.assertEquals(pkmn.calculations.gen2_pokemon_gender(pkmn.species.VULPIX, 12), pkmn.gender.MALE)

        # All female
        self.assertEquals(pkmn.calculations.gen2_pokemon_gender(pkmn.species.NIDORINA, 0), pkmn.gender.FEMALE)
        self.assertEquals(pkmn.calculations.gen2_pokemon_gender(pkmn.species.NIDORINA, 15), pkmn.gender.FEMALE)

        # Genderless
        self.assertEquals(pkmn.calculations.gen2_pokemon_gender(pkmn.species.MAGNEMITE, 0), pkmn.gender.GENDERLESS)
        self.assertEquals(pkmn.calculations.gen2_pokemon_gender(pkmn.species.MAGNEMITE, 15), pkmn.gender.GENDERLESS)

    def test_modern_gender(self):
        # Make sure expected errors are raised.
        with self.assertRaises(ValueError):
            gender = pkmn.calculations.modern_pokemon_gender(pkmn.species.NONE, 0)

        # All male
        self.assertEquals(pkmn.calculations.modern_pokemon_gender(pkmn.species.NIDORINO, 0), pkmn.gender.MALE)
        self.assertEquals(pkmn.calculations.modern_pokemon_gender(pkmn.species.NIDORINO, 0xFFFFFFFF), pkmn.gender.MALE)

        # 25% male, 75% female
        self.assertEquals(pkmn.calculations.modern_pokemon_gender(pkmn.species.VULPIX, 190), pkmn.gender.FEMALE)
        self.assertEquals(pkmn.calculations.modern_pokemon_gender(pkmn.species.VULPIX, 191), pkmn.gender.MALE)

        # All female
        self.assertEquals(pkmn.calculations.modern_pokemon_gender(pkmn.species.NIDORINA, 0), pkmn.gender.FEMALE)
        self.assertEquals(pkmn.calculations.modern_pokemon_gender(pkmn.species.NIDORINA, 0xFFFFFFFF), pkmn.gender.FEMALE)

        # Genderless
        self.assertEquals(pkmn.calculations.modern_pokemon_gender(pkmn.species.MAGNEMITE, 0), pkmn.gender.GENDERLESS)
        self.assertEquals(pkmn.calculations.modern_pokemon_gender(pkmn.species.MAGNEMITE, 0xFFFFFFFF), pkmn.gender.GENDERLESS)

    def test_gen2_hidden_power(self):
        # Make sure expected errors are raised.
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.gen2_hidden_power(-1, 0, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.gen2_hidden_power(16, 0, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.gen2_hidden_power(0, -1, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.gen2_hidden_power(0, 16, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.gen2_hidden_power(0, 0, -1, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.gen2_hidden_power(0, 0, 16, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.gen2_hidden_power(0, 0, 0, -1)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.gen2_hidden_power(0, 0, 0, 16)

        #
        # Make sure known good inputs result in expected results, and test (in)equality operators.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_II
        #
        expected_hidden_power = pkmn.calculations.hidden_power(pkmn.pokemon_type.DARK, 69)

        hidden_power = pkmn.calculations.gen2_hidden_power(15, 15, 15, 14)
        self.assertEqual(hidden_power, expected_hidden_power)

    def test_modern_hidden_power(self):
        # Make sure expected errors are raised.
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(-1, 0, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(32, 0, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, -1, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 32, 0, 0, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 0, -1, 0, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 0, 32, 0, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 0, 0, -1, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 0, 0, 32, 0, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 0, 0, 0, -1, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 0, 0, 0, 32, 0)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 0, 0, 0, 0, -1)
        with self.assertRaises(IndexError):
            hidden_power = pkmn.calculations.modern_hidden_power(0, 0, 0, 0, 0, 32)

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Hidden_Power_(move)/Calculation#Generation_III_to_VI
        #
        expected_hidden_power = pkmn.calculations.hidden_power(pkmn.pokemon_type.GRASS, 70)

        hidden_power = pkmn.calculations.modern_hidden_power(30, 31, 31, 31, 30, 31)
        self.assertEqual(hidden_power, expected_hidden_power)

    def test_natural_gift(self):
        # Test invalid generations.

        with self.assertRaises(IndexError):
            pkmn.calculations.natural_gift_stats(pkmn.item.CHERI_BERRY, 3)
        with self.assertRaises(IndexError):
            pkmn.calculations.natural_gift_stats(pkmn.item.CHERI_BERRY, 10)

        # Test invalid items.

        with self.assertRaises(ValueError):
            pkmn.calculations.natural_gift_stats(pkmn.item.POTION, 4)

        # Test expected values.

        items = [
            pkmn.item.CHERI_BERRY,
            pkmn.item.NANAB_BERRY,
            pkmn.item.BELUE_BERRY
        ]
        types = [
            pkmn.pokemon_type.FIRE,
            pkmn.pokemon_type.WATER,
            pkmn.pokemon_type.ELECTRIC
        ]
        gen4_powers = [60, 70, 80]
        gen5_powers = [60, 70, 80]
        gen6_powers = [80, 90, 100]
        test_cases = zip(items, types, gen4_powers, gen5_powers, gen6_powers)

        for item, type_name, gen4_power, gen5_power, gen6_power in test_cases:
            natural_gift = pkmn.calculations.natural_gift_stats(item, 4)
            self.assertEqual(natural_gift.type, type_name)
            self.assertEqual(natural_gift.base_power, gen4_power)

            natural_gift = pkmn.calculations.natural_gift_stats(item, 5)
            self.assertEqual(natural_gift.type, type_name)
            self.assertEqual(natural_gift.base_power, gen5_power)

            natural_gift = pkmn.calculations.natural_gift_stats(item, 6)
            self.assertEqual(natural_gift.type, type_name)
            self.assertEqual(natural_gift.base_power, gen6_power)

    def test_nature(self):
        natures = [
            pkmn.nature.HARDY,
            pkmn.nature.LONELY,
            pkmn.nature.BRAVE,
            pkmn.nature.ADAMANT,
            pkmn.nature.NAUGHTY,
            pkmn.nature.BOLD,
            pkmn.nature.DOCILE,
            pkmn.nature.RELAXED,
            pkmn.nature.IMPISH,
            pkmn.nature.LAX,
            pkmn.nature.TIMID,
            pkmn.nature.HASTY,
            pkmn.nature.SERIOUS,
            pkmn.nature.JOLLY,
            pkmn.nature.NAIVE,
            pkmn.nature.MODEST,
            pkmn.nature.MILD,
            pkmn.nature.QUIET,
            pkmn.nature.BASHFUL,
            pkmn.nature.RASH,
            pkmn.nature.CALM,
            pkmn.nature.GENTLE,
            pkmn.nature.SASSY,
            pkmn.nature.CAREFUL,
            pkmn.nature.QUIRKY
        ]

        # Make sure the SWIG wrapper keeps personality within the proper bounds.
        # Which error applies depends on the SWIG version.
        try:
            with self.assertRaises(OverflowError):
                pkmn.calculations.nature(-1)
        except:
            with self.assertRaises(TypeError):
                pkmn.calculations.nature(0xFFFFFFFF+1)

        for i in range(len(natures)):
            self.assertEqual(
                pkmn.calculations.nature((random.randint(0, 50000) * 1000) + i),
                natures[i]
            )

    #
    # Given the amount of time the C++ test takes, this will just verify
    # the API wrapper.
    #
    def test_personality(self):
        # Test invalid ability.
        with self.assertRaises(ValueError):
            personality = pkmn.calculations.generate_personality(
                              pkmn.species.CHARMANDER,
                              pkmn.pokemon.DEFAULT_TRAINER_ID,
                              True,
                              pkmn.ability.TORRENT,
                              pkmn.gender.MALE,
                              pkmn.nature.QUIET
                          )

        # Test invalid gender.
        with self.assertRaises(ValueError):
            personality = pkmn.calculations.generate_personality(
                              pkmn.species.CHARMANDER,
                              pkmn.pokemon.DEFAULT_TRAINER_ID,
                              True,
                              pkmn.ability.BLAZE,
                              pkmn.gender.NONE,
                              pkmn.nature.QUIET
                          )

        # Test invalid nature.
        with self.assertRaises(ValueError):
            personality = pkmn.calculations.generate_personality(
                              pkmn.species.CHARMANDER,
                              pkmn.pokemon.DEFAULT_TRAINER_ID,
                              True,
                              pkmn.ability.BLAZE,
                              pkmn.gender.MALE,
                              pkmn.nature.NONE
                          )

        # Make sure the SWIG wrapper keeps personality within the proper bounds.
        # Which error applies depends on the SWIG version.
        try:
            with self.assertRaises(OverflowError):
                personality = pkmn.calculations.generate_personality(
                                  pkmn.species.CHARMANDER,
                                  -1,
                                  True,
                                  pkmn.ability.BLAZE,
                                  pkmn.gender.MALE,
                                  pkmn.nature.QUIET
                              )
        except:
            with self.assertRaises(TypeError):
                personality = pkmn.calculations.generate_personality(
                                  pkmn.species.CHARMANDER,
                                  -1,
                                  True,
                                  pkmn.ability.BLAZE,
                                  pkmn.gender.MALE,
                                  pkmn.nature.QUIET
                              )
        try:
            with self.assertRaises(OverflowError):
                personality = pkmn.calculations.generate_personality(
                                  pkmn.species.CHARMANDER,
                                  0xFFFFFFFF+1,
                                  True,
                                  pkmn.ability.BLAZE,
                                  pkmn.gender.MALE,
                                  pkmn.nature.QUIET
                              )
        except:
            with self.assertRaises(TypeError):
                personality = pkmn.calculations.generate_personality(
                                  pkmn.species.CHARMANDER,
                                  0xFFFFFFFF+1,
                                  True,
                                  pkmn.ability.BLAZE,
                                  pkmn.gender.MALE,
                                  pkmn.nature.QUIET
                              )

        # Test and validate a valid call.
        personality = pkmn.calculations.generate_personality(
                          pkmn.species.CHARMANDER,
                          pkmn.pokemon.DEFAULT_TRAINER_ID,
                          True,
                          pkmn.ability.BLAZE,
                          pkmn.gender.MALE,
                          pkmn.nature.QUIET
                      )
        self.assertTrue(
            pkmn.calculations.modern_shiny(
                personality, pkmn.pokemon.DEFAULT_TRAINER_ID
            )
        )
        self.assertEquals(
            pkmn.gender.MALE,
            pkmn.calculations.modern_pokemon_gender(
                pkmn.species.CHARMANDER,
                personality
            )
        )

    def test_gen2_shiny(self):
        # Make sure expected errors are raised.
        with self.assertRaises(IndexError):
            shiny = pkmn.calculations.gen2_shiny(-1, 0, 0, 0)
        with self.assertRaises(IndexError):
            shiny = pkmn.calculations.gen2_shiny(16, 0, 0, 0)
        with self.assertRaises(IndexError):
            shiny = pkmn.calculations.gen2_shiny(0, -1, 0, 0)
        with self.assertRaises(IndexError):
            shiny = pkmn.calculations.gen2_shiny(0, 16, 0, 0)
        with self.assertRaises(IndexError):
            shiny = pkmn.calculations.gen2_shiny(0, 0, -1, 0)
        with self.assertRaises(IndexError):
            shiny = pkmn.calculations.gen2_shiny(0, 0, 16, 0)
        with self.assertRaises(IndexError):
            shiny = pkmn.calculations.gen2_shiny(0, 0, 0, -1)
        with self.assertRaises(IndexError):
            shiny = pkmn.calculations.gen2_shiny(0, 0, 0, 16)

        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Individual_values#Shininess
        #
        self.assertTrue(pkmn.calculations.gen2_shiny(7, 10, 10, 10))
        self.assertFalse(pkmn.calculations.gen2_shiny(6, 15, 7, 15))

    def test_modern_shiny(self):
        #
        # Make sure known good inputs result in expected results.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Personality_value#Shininess
        #         http://www.smogon.com/ingame/rng/pid_iv_creation#how_shiny
        #
        self.assertTrue(pkmn.calculations.modern_shiny(2814471828, 2545049318))
        self.assertTrue(pkmn.calculations.modern_shiny(0xB58F0B2A, 398174488))

    def test_pokemon_size(self):
        # There are no known good calculations, so just check for reasonable values
        # for each relevant Pokemon.
        pokemon_with_size_checks = [
            pkmn.database.pokemon_entry(
                pkmn.species.BARBOACH,
                pkmn.game.RUBY,
                ""
            ),
            pkmn.database.pokemon_entry(
                pkmn.species.SHROOMISH,
                pkmn.game.RUBY,
                ""
            ),
            pkmn.database.pokemon_entry(
                pkmn.species.SEEDOT,
                pkmn.game.EMERALD,
                ""
            ),
            pkmn.database.pokemon_entry(
                pkmn.species.LOTAD,
                pkmn.game.EMERALD,
                ""
            ),
            pkmn.database.pokemon_entry(
                pkmn.species.MAGIKARP,
                pkmn.game.FIRERED,
                ""
            ),
            pkmn.database.pokemon_entry(
                pkmn.species.HERACROSS,
                pkmn.game.FIRERED,
                ""
            )
        ]

        # Make sure the SWIG wrapper keeps personality within the proper bounds.
        # Which error applies depends on the SWIG version.
        try:
            with self.assertRaises(OverflowError):
                pkmn.calculations.pokemon_size(
                    pkmn.species.MAGIKARP, -1, 0, 0, 0, 0, 0, 0
                )
        except:
            with self.assertRaises(TypeError):
                pkmn.calculations.pokemon_size(
                    pkmn.species.MAGIKARP, -1, 0, 0, 0, 0, 0, 0
                )
        try:
            with self.assertRaises(OverflowError):
                pkmn.calculations.pokemon_size(
                    pkmn.species.MAGIKARP, 0xFFFFFFFF+1, 0, 0, 0, 0, 0, 0
                )
        except:
            with self.assertRaises(TypeError):
                pkmn.calculations.pokemon_size(
                    pkmn.species.MAGIKARP, 0xFFFFFFFF+1, 0, 0, 0, 0, 0, 0
                )

        # Test input validation.
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                pkmn.species.MAGIKARP, 0, -1, 0, 0, 0, 0, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                pkmn.species.MAGIKARP, 0, 32, 0, 0, 0, 0, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                pkmn.species.MAGIKARP, 0, 0, -1, 0, 0, 0, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                pkmn.species.MAGIKARP, 0, 0, 32, 0, 0, 0, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                pkmn.species.MAGIKARP, 0, 0, 0, -1, 0, 0, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                pkmn.species.MAGIKARP, 0, 0, 0, 32, 0, 0, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                pkmn.species.MAGIKARP, 0, 0, 0, 0, -1, 0, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                pkmn.species.MAGIKARP, 0, 0, 0, 0, 32, 0, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                pkmn.species.MAGIKARP, 0, 0, 0, 0, 0, -1, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                pkmn.species.MAGIKARP, 0, 0, 0, 0, 0, 32, 0
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                pkmn.species.MAGIKARP, 0, 0, 0, 0, 0, 0, -1
            )
        with self.assertRaises(IndexError):
            pkmn.calculations.pokemon_size(
                pkmn.species.MAGIKARP, 0, 0, 0, 0, 0, 0, 32
            )

        for entry in pokemon_with_size_checks:
            height = entry.height

            for i in range(10):
                size = pkmn.calculations.pokemon_size(
                           entry.species,
                           random.randint(0, 0xFFFFFF),
                           random.randint(0, 31),
                           random.randint(0, 31),
                           random.randint(0, 31),
                           random.randint(0, 31),
                           random.randint(0, 31),
                           random.randint(0, 31)
                       )
                self.assertTrue(abs(size-height) <= height)

    def test_spinda_coords(self):
        # Check (in)equality operators.
        spinda_coords1 = pkmn.calculations.spinda_coords(123, 456)
        spinda_coords2 = pkmn.calculations.spinda_coords(123, 456)
        spinda_coords3 = pkmn.calculations.spinda_coords(456, 123)

        self.assertTrue(spinda_coords1 == spinda_coords2)
        self.assertFalse(spinda_coords1 != spinda_coords2)
        self.assertFalse(spinda_coords1 == spinda_coords3)
        self.assertTrue(spinda_coords1 != spinda_coords3)

        # Check addition operators.
        added_coords = spinda_coords1 + spinda_coords2
        self.assertEquals(246, added_coords.x)
        self.assertEquals(912, added_coords.y)

        added_coords += spinda_coords3
        self.assertEquals(702, added_coords.x)
        self.assertEquals(1035, added_coords.y)

    def test_spinda_spots(self):
        # Check (in)equality operators.
        spots1 = pkmn.calculations.spinda_spots(
                     pkmn.calculations.spinda_coords(7, 5),
                     pkmn.calculations.spinda_coords(10, 0),
                     pkmn.calculations.spinda_coords(1, 4),
                     pkmn.calculations.spinda_coords(2, 15)
                 )
        spots2 = pkmn.calculations.spinda_spots(
                     pkmn.calculations.spinda_coords(7, 5),
                     pkmn.calculations.spinda_coords(10, 0),
                     pkmn.calculations.spinda_coords(1, 4),
                     pkmn.calculations.spinda_coords(2, 15)
                 )
        spots3 = pkmn.calculations.spinda_spots(
                     pkmn.calculations.spinda_coords(7, 5),
                     pkmn.calculations.spinda_coords(10, 0),
                     pkmn.calculations.spinda_coords(2, 15),
                     pkmn.calculations.spinda_coords(1, 4)
                 )
        self.assertTrue(spots1 == spots2)
        self.assertFalse(spots1 != spots2)
        self.assertFalse(spots1 == spots3)
        self.assertTrue(spots1 != spots3)

        #
        # Make sure known good inputs result in expected results, and test (in)equality operators.
        #
        # Source: https://github.com/magical/spinda
        #
        spots = pkmn.calculations.spinda_spot_offset(4064348759)
        self.assertTrue(spots == spots1)
        self.assertFalse(spots != spots1)

        # Check addition operators.
        added_spots = spots1 + spots2
        self.assertEquals(14, added_spots.left_ear.x)
        self.assertEquals(10, added_spots.left_ear.y)
        self.assertEquals(20, added_spots.right_ear.x)
        self.assertEquals(0, added_spots.right_ear.y)
        self.assertEquals(2, added_spots.left_face.x)
        self.assertEquals(8, added_spots.left_face.y)
        self.assertEquals(4, added_spots.right_face.x)
        self.assertEquals(30, added_spots.right_face.y)

        added_spots += spots3
        self.assertEquals(21, added_spots.left_ear.x)
        self.assertEquals(15, added_spots.left_ear.y)
        self.assertEquals(30, added_spots.right_ear.x)
        self.assertEquals(0, added_spots.right_ear.y)
        self.assertEquals(4, added_spots.left_face.x)
        self.assertEquals(23, added_spots.left_face.y)
        self.assertEquals(5, added_spots.right_face.x)
        self.assertEquals(34, added_spots.right_face.y)

        added_spots += pkmn.calculations.spinda_coords(20, 50)
        self.assertEquals(41, added_spots.left_ear.x)
        self.assertEquals(65, added_spots.left_ear.y)
        self.assertEquals(50, added_spots.right_ear.x)
        self.assertEquals(50, added_spots.right_ear.y)
        self.assertEquals(24, added_spots.left_face.x)
        self.assertEquals(73, added_spots.left_face.y)
        self.assertEquals(25, added_spots.right_face.x)
        self.assertEquals(84, added_spots.right_face.y)

        added_spots = added_spots + pkmn.calculations.spinda_coords(1, 5)
        self.assertEquals(42, added_spots.left_ear.x)
        self.assertEquals(70, added_spots.left_ear.y)
        self.assertEquals(51, added_spots.right_ear.x)
        self.assertEquals(55, added_spots.right_ear.y)
        self.assertEquals(25, added_spots.left_face.x)
        self.assertEquals(78, added_spots.left_face.y)
        self.assertEquals(26, added_spots.right_face.x)
        self.assertEquals(89, added_spots.right_face.y)

    def assertIntsAlmostEqual(self, a, b):
        self.assertTrue(a == b or a-1 == b or a+1 == b)

    def test_gb_stats(self):
        # Make sure expected errors are raised.
        with self.assertRaises(ValueError):
            stat = pkmn.calculations.get_gb_stat(pkmn.pokemon_stat.NONE, 1, 1, 1, 1)
        with self.assertRaises(IndexError):
            stat = pkmn.calculations.get_gb_stat(pkmn.pokemon_stat.ATTACK, 1, 1, 123456, 1)
        with self.assertRaises(IndexError):
            stat = pkmn.calculations.get_gb_stat(pkmn.pokemon_stat.ATTACK, 1, 1, 1, 12345)

        #
        # Test with known good inputs.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generations_I_and_II
        #
        stat = pkmn.calculations.get_gb_stat(pkmn.pokemon_stat.HP, 81, 35, 22850, 7)
        self.assertIntsAlmostEqual(stat, 189)
        stat = pkmn.calculations.get_gb_stat(pkmn.pokemon_stat.ATTACK, 81, 55, 23140, 8)
        self.assertIntsAlmostEqual(stat, 137)

    def test_modern_stats(self):
        # Make sure expected errors are raised.
        with self.assertRaises(ValueError):
            stat = pkmn.calculations.get_modern_stat(pkmn.pokemon_stat.NONE, 1, 1.0, 1, 1, 1)
        with self.assertRaises(ValueError):
            stat = pkmn.calculations.get_modern_stat(pkmn.pokemon_stat.SPECIAL, 1, 1.0, 1, 1, 1)
        with self.assertRaises(ValueError):
            stat = pkmn.calculations.get_modern_stat(pkmn.pokemon_stat.ATTACK, 1, 0.666, 1, 1, 1)
        with self.assertRaises(IndexError):
            stat = pkmn.calculations.get_modern_stat(pkmn.pokemon_stat.ATTACK, 1, 1.0, 1, 12345, 1)
        with self.assertRaises(IndexError):
            stat = pkmn.calculations.get_modern_stat(pkmn.pokemon_stat.ATTACK, 1, 1.0, 1, 1, 12345)

        #
        # Test with known good inputs.
        #
        # Source: http://bulbapedia.bulbagarden.net/wiki/Statistic#In_Generation_III_onward
        #
        stat = pkmn.calculations.get_modern_stat(pkmn.pokemon_stat.HP, 78, 1.0, 108, 74, 24)
        self.assertIntsAlmostEqual(stat, 289)
        stat = pkmn.calculations.get_modern_stat(pkmn.pokemon_stat.ATTACK, 78, 1.1, 130, 195, 12)
        self.assertIntsAlmostEqual(stat, 280)

if __name__ == "__main__":
    suite = unittest.TestLoader().loadTestsFromTestCase(calculations_test)
    test_program = unittest.TextTestRunner(verbosity=3).run(suite)
    sys.exit(0 if test_program.wasSuccessful() else 1)
