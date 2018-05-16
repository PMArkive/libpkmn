/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_POKEMON_PARTY_GCNIMPL_HPP
#define PKMN_POKEMON_PARTY_GCNIMPL_HPP

#include "pokemon_party_impl.hpp"

#include "libpkmgc_includes.hpp"

namespace pkmn {

    typedef struct {
        LibPkmGC::GC::Pokemon* pokemon[PARTY_SIZE];
    } gcn_pokemon_party_t;

    class pokemon_party_gcnimpl: public pokemon_party_impl {
        public:
            pokemon_party_gcnimpl() {}
            explicit pokemon_party_gcnimpl(
                int game_id
            );
            pokemon_party_gcnimpl(
                int game_id,
                LibPkmGC::GC::Pokemon** native
            );

            ~pokemon_party_gcnimpl();

            int get_num_pokemon() override final;

            void set_pokemon(
                int index,
                const pkmn::pokemon::sptr& new_pokemon
            ) override final;

        private:

            int _num_pokemon;

            void _from_native() override final;
    };
}

#endif /* PKMN_POKEMON_PARTY_GCNIMPL_HPP */
