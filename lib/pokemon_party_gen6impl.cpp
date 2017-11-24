/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "pokemon_gen6impl.hpp"
#include "pokemon_party_gen6impl.hpp"
#include "misc_common.hpp"

#include <pkmn/exception.hpp>

#include <pksav/gen6/pokemon.h>
#include <pksav/math/endian.h>

#include <boost/thread/lock_guard.hpp>

#include <cstring>
#include <stdexcept>

#define NATIVE_LIST_RCAST (reinterpret_cast<pksav_gen6_pokemon_party_t*>(_native))

namespace pkmn {

    pokemon_party_gen6impl::pokemon_party_gen6impl(
        int game_id
    ): pokemon_party_impl(game_id),
       _num_pokemon(0)
    {
        _native = reinterpret_cast<void*>(new pksav_gen6_pokemon_party_t);
        std::memset(_native, 0, sizeof(pksav_gen6_pokemon_party_t));
        _our_mem = true;

        _from_native();
    }

    pokemon_party_gen6impl::pokemon_party_gen6impl(
        int game_id,
        pksav_gen6_pokemon_party_t* native
    ): pokemon_party_impl(game_id),
       _num_pokemon(0)
    {
        _native = reinterpret_cast<void*>(native);
        _our_mem = false;

        _from_native();
    }

    pokemon_party_gen6impl::~pokemon_party_gen6impl()
    {
        if(_our_mem) {
            delete NATIVE_LIST_RCAST;
        }
    }

    int pokemon_party_gen6impl::get_num_pokemon()
    {
        return _num_pokemon;
    }

    void pokemon_party_gen6impl::set_pokemon(
        int index,
        pkmn::pokemon::sptr new_pokemon
    )
    {
        int num_pokemon = get_num_pokemon();
        int max_index = std::min<int>(PARTY_SIZE-1, num_pokemon);

        pkmn::enforce_bounds("Party index", index, 0, max_index);

        if(_pokemon_list.at(index)->get_native_pc_data() == new_pokemon->get_native_pc_data())
        {
            throw std::invalid_argument("Cannot set a Pokémon to itself.");
        }
        else if(index < (num_pokemon-1) and new_pokemon->get_species() == "None")
        {
            throw std::invalid_argument("Parties store Pokémon contiguously.");
        }

        boost::mutex::scoped_lock(_mem_mutex);

        // If the given Pokémon isn't from this party's game, convert it if we can.
        pkmn::pokemon::sptr actual_new_pokemon;
        if(_game_id == new_pokemon->get_database_entry().get_game_id())
        {
            actual_new_pokemon = new_pokemon;
        }
        else
        {
            actual_new_pokemon = new_pokemon->to_game(get_game());
        }

        pokemon_impl* new_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(actual_new_pokemon.get());
        pokemon_impl* old_party_pokemon_impl_ptr = dynamic_cast<pokemon_impl*>(_pokemon_list[index].get());

        // Make sure no one else is using the Pokémon variables.
        boost::lock_guard<pokemon_impl> new_pokemon_lock(*new_pokemon_impl_ptr);
        old_party_pokemon_impl_ptr->lock();

        // Copy the underlying memory to the party. At the end of this process,
        // all existing variables will correspond to the same Pokémon, even if
        // their underlying memory has changed.

        // Make a copy of the current Pokémon in the given party slot so it can be preserved in an sptr
        // that owns its own memory.
        copy_party_pokemon<pksav_gen6_pc_pokemon_t, pksav_gen6_pokemon_party_data_t>(index);

        // Copy the new Pokémon's internals into the party's internals and create a new sptr.
        void* new_pokemon_native_pc_ptr = new_pokemon_impl_ptr->_native_pc;
        void* new_pokemon_native_party_ptr = new_pokemon_impl_ptr->_native_party;

        // Unlock the old Pokémon's mutex is unlocked before it's destructor is called.
        old_party_pokemon_impl_ptr->unlock();

        NATIVE_LIST_RCAST->party[index].pc = *reinterpret_cast<pksav_gen6_pc_pokemon_t*>(new_pokemon_native_pc_ptr);
        NATIVE_LIST_RCAST->party[index].party_data = *reinterpret_cast<pksav_gen6_pokemon_party_data_t*>(new_pokemon_native_party_ptr);
        _pokemon_list[index] = pkmn::make_shared<pokemon_gen6impl>(
                                   &NATIVE_LIST_RCAST->party[index],
                                   _game_id
                               );

        // Update the number of Pokémon in the party if needed.
        std::string new_species = new_pokemon->get_species();
        if(index == num_pokemon)
        {
            if(pksav_littleendian16(NATIVE_LIST_RCAST->party[index].pc.blocks.blockA.species) > 0 and new_species != "None")
            {
                ++_num_pokemon;
            }
        }
        else if(index == (num_pokemon-1))
        {
            if(pksav_littleendian16(NATIVE_LIST_RCAST->party[index].pc.blocks.blockA.species) == 0 and new_species == "None")
            {
                --_num_pokemon;
            }
        }
    }

    void pokemon_party_gen6impl::_from_native()
    {
        // This shouldn't resize if the vector is populated.
        _pokemon_list.resize(PARTY_SIZE);

        for(int i = 0; i < PARTY_SIZE; ++i)
        {
            _pokemon_list[i] = pkmn::make_shared<pokemon_gen6impl>(
                                   &NATIVE_LIST_RCAST->party[i],
                                   _game_id
                               );
        }
    }
}