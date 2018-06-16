/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_LIST_IMPL_HPP
#define PKMN_ITEM_LIST_IMPL_HPP

#include <pkmn/item_list.hpp>

#include <boost/thread/lockable_adapter.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace pkmn {

    class item_list_impl: public item_list,
                          public boost::basic_lockable_adapter<boost::recursive_mutex>
    {
        public:
            item_list_impl() {};
            item_list_impl(
                int item_list_id,
                int game_id
            );

            virtual ~item_list_impl() {};

            std::string get_name() final;

            std::string get_game() final;

            int get_capacity() final;

            int get_num_items() override;

            virtual const pkmn::item_slot& at(
                int position
            ) override;

            virtual void add(
                const std::string& item_name,
                int amount
            ) override;

            virtual void remove(
                const std::string& item_name,
                int amount
            ) override;

            virtual void move(
                int old_position,
                int new_position
            ) override;

            virtual void set_item(
                int position,
                const std::string& item_name,
                int amount
            ) override;

            const pkmn::item_slots_t& as_vector() final;

            const std::vector<std::string>& get_valid_items() final;

            void* get_native() final;

        protected:
            int _item_list_id, _game_id, _version_group_id;
            int _capacity, _num_items;
            bool _pc;

            pkmn::item_slots_t _item_slots;
            std::vector<std::string> _valid_items;

            void* _p_native;

            virtual void _from_native(
                int index = -1
            ) = 0;
            virtual void _to_native(
                int index = -1
            ) = 0;
    };
}

#endif /* PKMN_ITEM_LIST_IMPL_HPP */
