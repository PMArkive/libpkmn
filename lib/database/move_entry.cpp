/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "utils/misc.hpp"
#include "enum_conversions.hpp"
#include "database_common.hpp"
#include "id_to_string.hpp"

#include <pkmn/exception.hpp>
#include <pkmn/database/move_entry.hpp>

#include <boost/config.hpp>
#include <boost/format.hpp>

#include <sstream>
#include <stdexcept>

namespace pkmn { namespace database {

    /*
     * This array lists the upper bound of valid move IDs
     * for each generation. This is trivial enough not to
     * warrant a database query.
     */
    BOOST_STATIC_CONSTEXPR int MOVE_INDEX_BOUNDS[] =
    {
        -1, 165, 251, 354, 467, 559, 621
    };

    // Some Generation VI moves are only in OR/AS
    BOOST_STATIC_CONSTEXPR int XY_MAX = 617;

    // Gamecube games have an extra range.
    BOOST_STATIC_CONSTEXPR int SHADOW_RUSH = 10001;
    BOOST_STATIC_CONSTEXPR int SHADOW_SKY  = 10018;

    // Relevant version IDs
    BOOST_STATIC_CONSTEXPR int COLO = 19;
    BOOST_STATIC_CONSTEXPR int XD   = 20;
    BOOST_STATIC_CONSTEXPR int Y    = 24;

    move_entry::move_entry(
        int move_id,
        int game_id
    ):
        _move_id(move_id),
        _game_id(game_id),
        _none(move_id == 0),
        _invalid(false)
    {
        _generation = pkmn::database::game_id_to_generation(
                          _game_id
                      );

        /*
         * Check to see if the move is valid for the given game.
         *
         * With this constructor, it's fine if not.
         */
        if(_move_id > MOVE_INDEX_BOUNDS[_generation])
        {
            if(_game_id == COLO)
            {
                _invalid = (_move_id != SHADOW_RUSH);
            }
            else if(_game_id == XD)
            {
                _invalid = (_move_id < SHADOW_RUSH) ||
                           (_move_id > SHADOW_SKY);
            }
            else
            {
                _invalid = true;
            }
        }
        else if((_generation == 6) && (_game_id <= Y))
        {
            _invalid = (_move_id > XY_MAX);
        }
        else
        {
            _invalid = false;
        }
    }

    // TODO: disallow specifically passing in INVALID here
    move_entry::move_entry(
        pkmn::e_move move,
        pkmn::e_game game
    ):
        _none(move == pkmn::e_move::NONE),
        _invalid(move == pkmn::e_move::INVALID)
    {
        // Input validation
        _game_id = pkmn::database::game_enum_to_id(game);
        _generation = pkmn::database::game_id_to_generation(
                          _game_id
                      );
        _move_id = static_cast<int>(move);

        // Check to see if the move is valid for the given game.
        if(_move_id > MOVE_INDEX_BOUNDS[_generation])
        {
            if(_game_id == COLO)
            {
                _invalid = (_move_id != SHADOW_RUSH);
            }
            else if(_game_id == XD)
            {
                _invalid = (_move_id < SHADOW_RUSH) ||
                           (_move_id > SHADOW_SKY);
            }
            else
            {
                _invalid = true;
            }
        }
        else if(_generation == 6 and _game_id <= Y)
        {
            _invalid = (_move_id > XY_MAX);
        }
        else
        {
            _invalid = false;
        }
        if(_invalid)
        {
            throw std::invalid_argument("This move was not in this game.");
        }
    }

    std::string move_entry::get_name() const {
        std::string ret;

        if(_none) {
            ret = "None";
        } else if(_invalid) {
            ret = str(boost::format("Invalid (0x%x)") % _move_id);
        } else {
            ret = pkmn::database::move_id_to_name(
                       _move_id, _generation
                   );
        }

        return ret;
    }

    pkmn::e_game move_entry::get_game() const
    {
        return pkmn::database::game_id_to_enum(_game_id);
    }

    pkmn::e_type move_entry::get_type() const
    {
        pkmn::e_type ret = pkmn::e_type::NONE;

        if(!_none && !_invalid)
        {
            if(_generation == 1)
            {
                /*
                 * In Generation I, before the Dark type was introduced,
                 * four moves were Normal type.
                 *
                 * There aren't enough edge cases to warrant adding them
                 * to the database.
                 */
                BOOST_STATIC_CONSTEXPR int NORMAL_IDS[] = {2,16,28,44};
                for(int normal_id: NORMAL_IDS)
                {
                    if(_move_id == normal_id)
                    {
                        ret = pkmn::e_type::NORMAL;
                    }
                }
            }

            if((ret == pkmn::e_type::NONE) && (_generation < 6))
            {
                /*
                 * In Generation VI, before the Fairy type was introduced,
                 * three moves were Normal type.
                 *
                 * There aren't enough edge cases to warrant adding them
                 * to the database.
                 */
                BOOST_STATIC_CONSTEXPR int NORMAL_IDS[] = {186,204,236};
                for(int normal_id: NORMAL_IDS)
                {
                    if(_move_id == normal_id)
                    {
                        ret = pkmn::e_type::NORMAL;
                    }
                }
            }

            if(ret == pkmn::e_type::NONE)
            {
                static const std::string query =
                    "SELECT type_id FROM moves WHERE id=?";

                ret = static_cast<pkmn::e_type>(
                          pkmn::database::query_db_bind1<int, int>(
                              query.c_str(),
                              _move_id
                          )
                      );
            }
        }

        return ret;
    }

    std::string move_entry::get_description() const {
        std::string ret;

        if(_none or _invalid) {
            ret = get_name();
        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT flavor_text FROM move_flavor_text WHERE move_id=? "
                "AND language_id=9";

            std::string from_db = pkmn::database::query_db_bind1<std::string, int>(
                                      query, _move_id
                                  );
            ret = fix_veekun_whitespace(from_db);
        }

        return ret;
    }

    pkmn::e_move_target move_entry::get_target() const
    {
        pkmn::e_move_target ret = pkmn::e_move_target::NONE;

        if(!_none && !_invalid)
        {
            static const std::string query =
                "SELECT id FROM move_targets WHERE id="
                "(SELECT target_id FROM moves WHERE id=?)";

            ret = static_cast<pkmn::e_move_target>(
                      pkmn::database::query_db_bind1<int, int>(
                          query.c_str(),
                          _move_id
                      )
                  );
        }

        return ret;
    }

    pkmn::e_move_damage_class move_entry::get_damage_class() const
    {
        pkmn::e_move_damage_class ret = pkmn::e_move_damage_class::NONE;

        if(!_none && !_invalid)
        {
            static const std::string main_query =
                "SELECT damage_class_id FROM moves WHERE id=?";

            ret = static_cast<pkmn::e_move_damage_class>(
                      pkmn::database::query_db_bind1<int, int>(
                          main_query.c_str(),
                          _move_id
                      )
                  );

            /*
             * In Generations I-III (minus the Gamecube games), a move's damage
             * class was associated with its type instead of the move itself,
             * unless it's a status move.
             */
            const bool is_old_game = ((_generation < 4) && !game_is_gamecube(_game_id));
            if(is_old_game && (ret != pkmn::e_move_damage_class::STATUS))
            {
                static const std::string old_game_query =
                    "SELECT damage_class_id FROM types WHERE id="
                    "(SELECT type_id FROM moves where id=?)";

                ret = static_cast<pkmn::e_move_damage_class>(
                          pkmn::database::query_db_bind1<int, int>(
                              old_game_query.c_str(),
                              _move_id
                          )
                      );
            }
        }

        return ret;
    }

    int move_entry::get_base_power() const {
        int ret = 0;

        if(_none or _invalid) {
            ret = -1;
        }
        else if(_move_id == 10001 and _game_id == 19) {
            // Edge case not worth putting in a database
            ret = 90;
        } else {
            static BOOST_CONSTEXPR const char* main_query = \
                "SELECT power FROM moves WHERE id=?";

            static BOOST_CONSTEXPR const char* old_queries[] = {
                "",
                "SELECT gen1_power FROM old_move_powers WHERE move_id=?",
                "SELECT gen2_power FROM old_move_powers WHERE move_id=?",
                "SELECT gen3_power FROM old_move_powers WHERE move_id=?",
                "SELECT gen4_power FROM old_move_powers WHERE move_id=?",
                "SELECT gen5_power FROM old_move_powers WHERE move_id=?",
            };

            /*
             * If this entry is for an older game, check if it had an older
             * power. If not, fall back to the default query.
             */
            if(_generation < 6) {
                int old_ret = 0;
                if(pkmn::database::maybe_query_db_bind1<int, int>(
                       old_queries[_generation], old_ret,
                       _move_id
                   ))
                {
                    ret = old_ret;
                }
                else
                {
                    ret = pkmn::database::query_db_bind1<int, int>(
                              main_query, _move_id
                          );
                }
            } else {
                ret = pkmn::database::query_db_bind1<int, int>(
                          main_query, _move_id
                      );
            }
        }

        return ret;
    }

    int move_entry::get_pp(
        int num_pp_ups
    ) const
    {
        int ret  = 0;

        if(_none or _invalid)
        {
            ret = 0;
        }
        else
        {
            pkmn::enforce_bounds("# PP Ups", num_pp_ups, 0, 3);

            static BOOST_CONSTEXPR const char* main_query = \
                "SELECT pp FROM moves WHERE id=?";

            static BOOST_CONSTEXPR const char* old_queries[] = {
                "",
                "SELECT gen1_pp FROM old_move_pps WHERE move_id=?",
                "SELECT gen2_pp FROM old_move_pps WHERE move_id=?",
                "SELECT gen3_pp FROM old_move_pps WHERE move_id=?",
                "SELECT gen4_pp FROM old_move_pps WHERE move_id=?",
                "SELECT gen5_pp FROM old_move_pps WHERE move_id=?",
            };

            /*
             * If this entry is for an older game, check if it had an older
             * base PP. If not, fall back to the default query.
             */
            int base_pp = -1;
            if(_generation < 6)
            {
                (void)pkmn::database::maybe_query_db_bind1<int, int>(
                          old_queries[_generation],
                          base_pp, _move_id
                      );
            }

            if(base_pp == -1)
            {
                base_pp = pkmn::database::query_db_bind1<int, int>(
                              main_query, _move_id
                          );
            }

            if(num_pp_ups == 0)
            {
                ret = base_pp;
            }
            else
            {
                int _20p = int(base_pp * 0.2);
                ret = (base_pp + (num_pp_ups * _20p));
            }
        }

        return ret;
    }

    float move_entry::get_accuracy() const {
        float ret = 0.0f;

        if(_none or _invalid) {
            ret = -1.0f;
        } else {
            static BOOST_CONSTEXPR const char* main_query = \
                "SELECT accuracy FROM moves WHERE id=?";

            static BOOST_CONSTEXPR const char* old_queries[] = {
                "",
                "SELECT gen1_accuracy FROM old_move_accuracies WHERE move_id=?",
                "SELECT gen2_accuracy FROM old_move_accuracies WHERE move_id=?",
                "SELECT gen3_accuracy FROM old_move_accuracies WHERE move_id=?",
                "SELECT gen4_accuracy FROM old_move_accuracies WHERE move_id=?",
                "SELECT gen5_accuracy FROM old_move_accuracies WHERE move_id=?",
            };

            /*
             * If this entry is for an older game, check if it had an older
             * accuracy.
             */
            if(_generation < 6) {
                double old_ret;
                if(pkmn::database::maybe_query_db_bind1<double, int>(
                       old_queries[_generation], old_ret,
                       _move_id
                   ))
                {
                    // Veekun's database stores this as an int 0-100.
                    ret = (float(old_ret) / 100.0f);
                }
                else
                {
                    // SQLite uses doubles, so avoid implicit casting ambiguity
                    ret = (static_cast<float>(pkmn::database::query_db_bind1<double, int>(
                                                  main_query, _move_id
                                             ))) / 100.0f;
                }
            } else {
                // SQLite uses doubles, so avoid implicit casting ambiguity
                ret = (static_cast<float>(pkmn::database::query_db_bind1<double, int>(
                                              main_query, _move_id
                                         ))) / 100.0f;
            }
        }

        return ret;
    }

    int move_entry::get_priority() const {
        int ret = 0;

        if(_none or _invalid) {
            ret = -9;
        } else {
            static BOOST_CONSTEXPR const char* main_query = \
                "SELECT priority FROM moves WHERE id=?";

            static BOOST_CONSTEXPR const char* old_queries[] = {
                "",
                "SELECT gen1_priority FROM old_move_priorities WHERE move_id=?",
                "SELECT gen2_priority FROM old_move_priorities WHERE move_id=?",
                "SELECT gen3_priority FROM old_move_priorities WHERE move_id=?",
                "SELECT gen4_priority FROM old_move_priorities WHERE move_id=?",
                "SELECT gen5_priority FROM old_move_priorities WHERE move_id=?",
            };

            /*
             * If this entry is for an older game, check if it had an older
             * priority. If not, fall back to the default query.
             */
            if(_generation < 6) {
                int old_ret = 0;
                if(pkmn::database::maybe_query_db_bind1<int, int>(
                       old_queries[_generation], old_ret,
                       _move_id
                    ))
                {
                    ret = old_ret;
                }
                else
                {
                    ret = pkmn::database::query_db_bind1<int, int>(
                              main_query, _move_id
                          );
                }
            } else {
                ret = pkmn::database::query_db_bind1<int, int>(
                          main_query, _move_id
                      );
            }
        }

        return ret;
    }

    static std::string _cleanup_effect(
        const std::string& input,
        int move_id
    ) {
        // Get effect chance
        static BOOST_CONSTEXPR const char* query = \
            "SELECT effect_chance FROM moves WHERE id=?";

        int effect_chance = pkmn::database::query_db_bind1<int, int>(
                                query, move_id
                            );

        std::string ret = input;

        ret.erase(std::remove(ret.begin(), ret.end(), '['), ret.end());
        ret.erase(std::remove(ret.begin(), ret.end(), ']'), ret.end());

        size_t open = ret.find("{");
        if(open != std::string::npos) {
            size_t close = ret.find("}");
            ret.replace(open, (close-open+1), "");
        }

        if(effect_chance > 0) {
            std::stringstream stream;
            stream << effect_chance << "%";

            size_t effect_chance_pos = ret.find("$effect_chance%");
            if(effect_chance_pos != std::string::npos) {
                ret.replace(effect_chance_pos, 15, stream.str());
            }
        }

        return ret;
    }

    std::string move_entry::get_effect() const {
        std::string ret;

        if(_none) {
            ret = "None";
        } else if(_invalid) {
            ret = "Unknown";
        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT short_effect FROM move_effect_prose WHERE move_effect_id="
                "(SELECT effect_id FROM moves WHERE id=?)";

            std::string from_db = pkmn::database::query_db_bind1<std::string, int>(
                                      query, _move_id
                                  );
            ret = _cleanup_effect(from_db, _move_id);
        }

        return ret;
    }

    pkmn::e_contest_stat move_entry::get_contest_type() const
    {
        pkmn::e_contest_stat ret = pkmn::e_contest_stat::NONE;

        if(!_none && !_invalid && (_generation >= 3) && !game_is_gamecube(_game_id))
        {
            int contest_stat_as_int = 0;

            static const std::string query = "SELECT contest_type_id FROM moves WHERE id=?";
            if(pkmn::database::maybe_query_db_bind1<int, int>(
                   query.c_str(), contest_stat_as_int, _move_id
               ))
            {
                ret = static_cast<pkmn::e_contest_stat>(contest_stat_as_int);
            }
        }

        return ret;
    }

    std::string move_entry::get_contest_effect() const {
        std::string ret;

        // Contests started in Generation III
        if(_none or _generation < 3 or game_is_gamecube(_game_id)) {
            ret = "None";
        } else if(_invalid) {
            ret = "Unknown";
        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT flavor_text FROM contest_effect_prose WHERE contest_effect_id="
                "(SELECT contest_effect_id FROM moves WHERE id=?)";

            if(not pkmn::database::maybe_query_db_bind1<std::string, int>(
                       query, ret, _move_id
               ))
            {
                ret = "None";
            }
        }

        return ret;
    }

    std::string move_entry::get_super_contest_effect() const {
        std::string ret;
        // Super Contests are only in Generation IV
        if(_none or _generation != 4) {
            ret = "None";
        } else if(_invalid) {
            ret = "Unknown";
        } else {
            static BOOST_CONSTEXPR const char* query = \
                "SELECT flavor_text FROM super_contest_effect_prose WHERE super_contest_effect_id="
                "(SELECT super_contest_effect_id FROM moves WHERE id=?)";

            if(not pkmn::database::maybe_query_db_bind1<std::string, int>(
                       query, ret, _move_id
               ))
            {
                ret = "None";
            }
        }

        return ret;
    }

}}
