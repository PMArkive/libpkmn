/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_GAME_SAVE_IMPL_HPP
#define PKMN_GAME_SAVE_IMPL_HPP

#include <pkmn/game_save.hpp>

#include <boost/noncopyable.hpp>

BOOST_STATIC_CONSTEXPR int MONEY_MAX_VALUE = 999999;

namespace pkmn {

    class game_save_impl: public game_save, boost::noncopyable {
        public:
            game_save_impl() {};
            game_save_impl(
                const std::string &filepath
            );

            virtual ~game_save_impl() {};

            std::string get_filepath();

            std::string get_game();

            pkmn::item_bag::sptr get_item_bag();

            pkmn::item_list::sptr get_item_pc();

        protected:
            std::string _filepath;

            pkmn::item_bag::sptr _item_bag;
            pkmn::item_list::sptr _item_pc;

            int _game_id;
    };

}

#endif /* PKMN_GAME_SAVE_IMPL_HPP */
