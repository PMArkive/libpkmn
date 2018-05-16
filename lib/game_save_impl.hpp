/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_GAME_SAVE_IMPL_HPP
#define PKMN_GAME_SAVE_IMPL_HPP

#include <pkmn/game_save.hpp>

#include <boost/noncopyable.hpp>
#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/recursive_mutex.hpp>

BOOST_STATIC_CONSTEXPR int MONEY_MAX_VALUE = 999999;

namespace pkmn {

    class game_save_impl: public game_save,
                          private boost::noncopyable,
                          public boost::basic_lockable_adapter<boost::recursive_mutex>
    {
        public:
            game_save_impl() {};
            explicit game_save_impl(
                const std::string& filepath
            );

            virtual ~game_save_impl() {};

            std::string get_filepath() override final;

            void save() override final;

            std::string get_game() override final;

            virtual const pkmn::pokedex::sptr& get_pokedex() override;

            const pkmn::pokemon_party::sptr& get_pokemon_party() override final;

            const pkmn::pokemon_pc::sptr& get_pokemon_pc() override final;

            const pkmn::item_bag::sptr& get_item_bag() override final;

            const pkmn::item_list::sptr& get_item_pc() override final;

        protected:
            std::string _filepath;

            pkmn::pokedex::sptr _pokedex;
            pkmn::pokemon_party::sptr _pokemon_party;
            pkmn::pokemon_pc::sptr _pokemon_pc;
            pkmn::item_bag::sptr _item_bag;
            pkmn::item_list::sptr _item_pc;

            int _game_id;
    };

}

#endif /* PKMN_GAME_SAVE_IMPL_HPP */
