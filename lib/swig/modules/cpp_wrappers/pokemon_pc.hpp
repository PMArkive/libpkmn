/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef CPP_WRAPPERS_POKEMON_PC_HPP
#define CPP_WRAPPERS_POKEMON_PC_HPP

#include "exception_internal.hpp"

#include <pkmn/config.hpp>
#include <pkmn/exception.hpp>
#include <pkmn/pokemon_pc.hpp>

#include "swig/modules/cpp_wrappers/pokemon_box.hpp"

namespace pkmn { namespace swig {

    class pokemon_pc
    {
        public:
            pokemon_pc():
                _pokemon_pc(nullptr),
                _generation(0)
            {}

            pokemon_pc(
                const pkmn::pokemon_pc::sptr& cpp_pokemon_pc
            ): _pokemon_pc(cpp_pokemon_pc),
               _generation(pkmn::priv::game_name_to_generation(cpp_pokemon_pc->get_game()))
            {
            }

            pokemon_pc(
                const std::string& game
            ): _pokemon_pc(pkmn::pokemon_pc::make(game)),
               _generation(pkmn::priv::game_name_to_generation(game))
            {
            }

            inline std::string get_game()
            {
                return _pokemon_pc->get_game();
            }

            inline int get_num_boxes()
            {
                return _pokemon_pc->get_num_boxes();
            }

            inline pkmn::swig::pokemon_box get_box(
                int index
            )
            {
#ifdef SWIGLUA
                pkmn::enforce_bounds(
                    "Box index",
                    index,
                    1,
                    get_num_boxes()
                );

                return pkmn::swig::pokemon_box(_pokemon_pc->get_box(index-1));
#else
                return pkmn::swig::pokemon_box(_pokemon_pc->get_box(index));
#endif
            }

            // Copy the vector, since the const in the reference
            // is casted away.
            inline std::vector<std::string> get_box_names()
            {
                if(_generation >= 2)
                {
                    return _pokemon_pc->get_box_names();
                }
                else
                {
                    return std::vector<std::string>();
                }
            }

#ifdef SWIGCSHARP
            inline uintmax_t cptr()
            {
                return uintmax_t(_pokemon_pc.get());
            }
#else
            inline bool operator==(const pokemon_pc& rhs) const
            {
                return (_pokemon_pc == rhs._pokemon_pc);
            }

            inline bool operator!=(const pokemon_pc& rhs) const
            {
                return !operator==(rhs);
            }
#endif

        private:
            pkmn::pokemon_pc::sptr _pokemon_pc;
            int _generation;
    };

}}

#endif /* CPP_WRAPPERS_POKEMON_PC_HPP */
