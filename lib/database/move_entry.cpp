/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "database_common.hpp"
#include "id_to_string.hpp"

#include <pkmn/database/move_entry.hpp>

#include <boost/config.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>

#include <sstream>
#include <stdexcept>

namespace pkmn { namespace database {

    static pkmn::database::sptr _db;

    move_entry::move_entry(
        int move_id,
        int game_id
    ):
        _move_id(move_id),
        _game_id(game_id),
        _generation(0),
        _none(move_id == 0),
        _invalid(false) // TODO: proper check
    {
        // Connect to database
        pkmn::database::get_connection(_db);

        // TODO: specific error if move not in game

        _generation = pkmn::database::game_id_to_generation(
                          _game_id
                      );
    }

    move_entry::move_entry(
        const std::string &move_name,
        const std::string &game_name
    ):
        _none(move_name == "None"),
        _invalid(false)
    {
        // Connect to database
        pkmn::database::get_connection(_db);

        // Input validation
        // TODO: specific error if move not in game
        _move_id = pkmn::database::move_name_to_id(
                       move_name
                   );
        _game_id = pkmn::database::game_name_to_id(
                       game_name
                   );
        _generation = pkmn::database::game_id_to_generation(
                          _game_id
                      );
    }

    std::string move_entry::get_name() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return str(boost::format("Invalid (0x%x)") % _move_id);
        }

        return pkmn::database::move_id_to_name(
                   _move_id, _generation
               );
    }

    std::string move_entry::get_game() const {
        return pkmn::database::game_id_to_name(
                   _game_id
               );
    }

    std::string move_entry::get_type() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return "Invalid";
        } else if(_generation == 1) {
            /*
             * In Generation I, before the Dark type was introduced,
             * four moves were Normal type.
             *
             * There aren't enough edge cases to warrant adding them
             * to the database.
             */
            BOOST_STATIC_CONSTEXPR int NORMAL_IDS[] = {2,16,28,44};
            for(int i = 0; i < 4; i++) {
                if(_move_id == NORMAL_IDS[i]) {
                    return "Normal";
                }
            }
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM type_names WHERE local_language_id=9 "
            "AND type_id=(SELECT type_id FROM moves WHERE id=?)";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, _move_id
               );
    }

    std::string move_entry::get_description() const {
        if(_none or _invalid) {
            return get_name();
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT flavor_text FROM move_flavor_text WHERE move_id=? "
            "AND language_id=9";

        std::string from_db = pkmn::database::query_db_bind1<std::string, int>(
                                  _db, query, _move_id
                              );
        return fix_veekun_whitespace(from_db);
    }

    std::string move_entry::get_damage_class() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* old_games_query = \
            "SELECT description FROM move_damage_class_prose "
            "WHERE local_language_id=9 AND move_damage_class_id="
            "(SELECT damage_class_id FROM types WHERE id="
            "(SELECT type_id FROM moves where id=?))";

        static BOOST_CONSTEXPR const char* new_games_query = \
            "SELECT description FROM move_damage_class_prose "
            "WHERE local_language_id=9 AND move_damage_class_id="
            "(SELECT damage_class_id FROM moves WHERE id=?)";

        bool old_game = (_generation < 4 and not game_is_gamecube(_game_id));
        return pkmn::database::query_db_bind1<std::string, int>(
                   _db,
                   (old_game ? old_games_query : new_games_query),
                   _move_id
               );
    }

    int move_entry::get_base_power() const {
        if(_none or _invalid) {
            return -1;
        }

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
                   _db, old_queries[_generation], old_ret,
                   _move_id
                ))
            {
                return old_ret;
            }
        }

        return pkmn::database::query_db_bind1<int, int>(
                   _db, main_query, _move_id
               );
    }

    int move_entry::get_pp(
        int num_pp_ups
    ) const {
        if(_none or _invalid) {
            return -1;
        } else if(num_pp_ups < 0 or num_pp_ups > 3) {
            throw std::out_of_range("num_pp_ups: valid range 0-3");
        }

        return 0;
    }

    float move_entry::get_accuracy() const {
        if(_none or _invalid) {
            return -1.0f;
        }

        static BOOST_CONSTEXPR const char* main_query = \
            "SELECT accuracy FROM moves WHERE move_id=?";

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
         * accuracy. If not, fall back to the default query.
         */
        if(_generation < 6) {
            double old_ret;
            if(pkmn::database::maybe_query_db_bind1<double, int>(
                   _db, old_queries[_generation], old_ret,
                   _move_id
                ))
            {
                // Veekun's database stores this as an int 0-100.
                return (float(old_ret) / 100.0f);
            }
        }

        // SQLite uses doubles, so avoid implicit casting ambiguity
        float ret = (float)pkmn::database::query_db_bind1<double, int>(
                               _db, main_query, _move_id
                           );

        // Veekun's database stores this as an int 0-100.
        return (ret / 100.0f);
    }

    int move_entry::get_priority() const {
        if(_none or _invalid) {
            return -9;
        }

        static BOOST_CONSTEXPR const char* main_query = \
            "SELECT priority FROM moves WHERE move_id=?";

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
                   _db, old_queries[_generation], old_ret,
                   _move_id
                ))
            {
                return old_ret;
            }
        }

        return pkmn::database::query_db_bind1<int, int>(
                   _db, main_query, _move_id
               );
    }

    // TODO: Would boost::regex make this nicer?
    static std::string _cleanup_effect(
        const std::string &input,
        float effect_chance
    ) {
        std::string ret = input;

        ret.erase(std::remove(ret.begin(), ret.end(), '['), ret.end());
        ret.erase(std::remove(ret.begin(), ret.end(), ']'), ret.end());

        size_t open = ret.find("{");
        if(open != std::string::npos) {
            size_t close = ret.find("}");
            ret.replace(open, (close-open+1), "");
        }

        int effect_chance_num = int(effect_chance * 100.0);
        if(effect_chance_num > 0) {
            std::stringstream stream;
            stream << effect_chance_num << "%";

            size_t effect_chance_pos = ret.find("$effect_chance%");
            if(effect_chance_pos != std::string::npos) {
                ret.replace(effect_chance_pos, 15, stream.str());
            }
        }

        return ret;
    }

    std::string move_entry::get_effect() const {
        if(_none) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT short_effect FROM move_effect_prose WHERE move_effect_id="
            "(SELECT effect_id FROM moves WHERE id=?)";

        std::string from_db = pkmn::database::query_db_bind1<std::string, int>(
                                  _db, query, _move_id
                              );
        return _cleanup_effect(from_db, this->get_effect_chance());
    }

    float move_entry::get_effect_chance() const {
        if(_none or _invalid) {
            return -1.0f;
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT effect_chance FROM moves WHERE id=?";

        // SQLite uses doubles, so avoid implicit casting ambiguity
        float effect_chance_from_db = (float)pkmn::database::query_db_bind1<double, int>(
                                          _db, query, _move_id
                                      );

        // Veekun's database stores this as an int 0-100.
        return (effect_chance_from_db / 100.0f);
    }

    std::string move_entry::get_contest_type() const {
        // Contests started in Generation III
        if(_none or _generation < 3 or game_is_gamecube(_game_id)) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT name FROM contest_type_names WHERE contest_type_id="
            "(SELECT contest_type_id FROM moves WHERE id=?) "
            "AND local_language_id=9";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, _move_id
               );
    }

    std::string move_entry::get_contest_effect() const {
        // Contests started in Generation III
        if(_none or _generation < 3 or game_is_gamecube(_game_id)) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT flavor_text FROM contest_effect_prose WHERE contest_effect_id="
            "(SELECT contest_effect_id FROM moves WHERE id=?)";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, _move_id
               );
    }

    std::string move_entry::get_super_contest_effect() const {
        // Super Contests started in Generation III
        if(_none or _generation < 4) {
            return "None";
        } else if(_invalid) {
            return "Unknown";
        }

        static BOOST_CONSTEXPR const char* query = \
            "SELECT flavor_text FROM super_contest_effect_prose WHERE super_contest_effect_id="
            "(SELECT super_contest_effect_id FROM moves WHERE id=?)";

        return pkmn::database::query_db_bind1<std::string, int>(
                   _db, query, _move_id
               );
    }

}}
