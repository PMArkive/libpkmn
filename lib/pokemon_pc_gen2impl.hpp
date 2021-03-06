/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PC_GEN2IMPL_HPP
#define PKMN_POKEMON_PC_GEN2IMPL_HPP

#include "pokemon_pc_impl.hpp"

#include <pksav/gen2/pokemon.h>
#include <pksav/gen2/save.h>

#include <vector>

namespace pkmn {

    class pokemon_pc_gen2impl: public pokemon_pc_impl
    {
        public:
            pokemon_pc_gen2impl(
                int game_id,
                const struct pksav_gen2_pokemon_storage* p_native = nullptr
            );

            ~pokemon_pc_gen2impl() = default;

            int get_num_boxes() final;

        private:
            struct pksav_gen2_pokemon_storage _pksav_storage;

            // The PKSav storage struct contains pointers, so they need
            // to point to something.
            struct pksav_gen2_pokemon_box_names _native_box_names;
            uint8_t _current_box_num;
            struct pksav_gen2_pokemon_box _current_box;
            std::vector<struct pksav_gen2_pokemon_box> _native_boxes;

            void _from_native() final;
            void _to_native() final;

            void _update_box_names() final;
            void _update_native_box_wallpapers() final;
    };
}

#endif /* PKMN_POKEMON_PC_GEN2IMPL_HPP */
