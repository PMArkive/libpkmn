/*
 * Copyright (c) 2015-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "exception_internal.hpp"
#include "item_list_gen2_tmhmimpl.hpp"

#include <pksav/gen2/items.h>

#include <pkmn/database/item_entry.hpp>
#include <pkmn/exception.hpp>

#include <boost/thread/lock_guard.hpp>

#include <cstdio>
#include <cstring>
#include <stdexcept>

BOOST_STATIC_CONSTEXPR int TM01_ID = 305;
BOOST_STATIC_CONSTEXPR int TM50_ID = 354;
BOOST_STATIC_CONSTEXPR int HM01_ID = 397;
BOOST_STATIC_CONSTEXPR int HM07_ID = 403;

static PKMN_CONSTEXPR_OR_INLINE bool ITEM_ID_IS_TM(int num)
{
    return (num >= TM01_ID) && (num <= TM50_ID);
}

static PKMN_CONSTEXPR_OR_INLINE bool ITEM_ID_IS_HM(int num)
{
    return (num >= HM01_ID) && (num <= HM07_ID);
}

namespace pkmn {

    item_list_gen2_tmhmimpl::item_list_gen2_tmhmimpl(
        int item_list_id,
        int game_id,
        const struct pksav_gen2_tmhm_pocket* p_pksav_list
    ): item_list_impl(item_list_id, game_id)
    {
        _item_slots.resize(PKSAV_GEN2_TM_COUNT + PKSAV_GEN2_HM_COUNT);

        for(size_t tm_index = 0;
            tm_index < PKSAV_GEN2_TM_COUNT;
            ++tm_index)
        {
            _item_slots[tm_index].item =
                pkmn::e_item(size_t(pkmn::e_item::TM01) + tm_index);
        }
        for(size_t hm_index = 0;
            hm_index < PKSAV_GEN2_HM_COUNT;
            ++hm_index)
        {
            _item_slots[50+hm_index].item =
                pkmn::e_item(size_t(pkmn::e_item::HM01) + hm_index);
        }

        if(p_pksav_list != nullptr)
        {
            _pksav_list = *p_pksav_list;
        }
        else
        {
            std::memset(&_pksav_list, 0, sizeof(_pksav_list));
        }

        _from_native();

        _p_native = &_pksav_list;
    }

    int item_list_gen2_tmhmimpl::get_num_items()
    {
        boost::lock_guard<item_list_gen2_tmhmimpl> lock(*this);

        int ret = 0;
        ret += int(std::count_if(
                       _pksav_list.tm_count,
                       _pksav_list.tm_count + PKSAV_GEN2_TM_COUNT,
                       [](uint8_t tm_count)
                       {
                           return (tm_count > 0);
                       }));
        ret += int(std::count_if(
                       _pksav_list.hm_count,
                       _pksav_list.hm_count + PKSAV_GEN2_HM_COUNT,
                       [](uint8_t hm_count)
                       {
                           return (hm_count > 0);
                       }));

        return ret;
    }

    void item_list_gen2_tmhmimpl::add(
        pkmn::e_item item,
        int amount
    )
    {
        pkmn::enforce_bounds("Amount", amount, 1, 99);

        boost::lock_guard<item_list_gen2_tmhmimpl> lock(*this);

        pkmn::database::item_entry entry(item, get_game());
        if(entry.get_pocket() != get_name())
        {
            throw std::invalid_argument("This item is not valid for this list.");
        }

        int item_id = entry.get_item_id();
        int position = -1;
        if(ITEM_ID_IS_TM(item_id))
        {
            position = item_id - TM01_ID;
        }
        else if(ITEM_ID_IS_HM(item_id))
        {
            position = item_id - 347;
        }
        else
        {
            throw std::invalid_argument("Invalid item.");
        }

        int new_amount = _item_slots[position].amount + amount;
        _item_slots[position].amount = std::min<int>(99, new_amount);
        _to_native(position);
    }

    void item_list_gen2_tmhmimpl::remove(
        pkmn::e_item item,
        int amount
    )
    {
        pkmn::enforce_bounds("Amount", amount, 1, 99);

        boost::lock_guard<item_list_gen2_tmhmimpl> lock(*this);

        pkmn::database::item_entry entry(item, get_game());
        if(entry.get_pocket() != get_name())
        {
            throw std::invalid_argument("This item is not valid for this list.");
        }

        int item_id = entry.get_item_id();
        int position = -1;
        if(ITEM_ID_IS_TM(item_id))
        {
            position = item_id - TM01_ID;
        }
        else if(ITEM_ID_IS_HM(item_id))
        {
            position = item_id - 347;
        }
        else
        {
            throw std::runtime_error("Invalid item.");
        }

        int new_amount = _item_slots[position].amount - amount;
        _item_slots[position].amount = std::max<int>(0, new_amount);
        _to_native(position);
    }

    void item_list_gen2_tmhmimpl::move(int, int)
    {
        throw pkmn::feature_not_in_game_error("Cannot move items in this pocket.");
    }

    void item_list_gen2_tmhmimpl::set_item(
        int position,
        pkmn::e_item item,
        int amount
    )
    {
        // Input validation.
        int end_boundary = std::min<int>(_num_items, _capacity-1);
        pkmn::enforce_bounds("Position", position, 0, end_boundary);

        boost::lock_guard<item_list_gen2_tmhmimpl> lock(*this);

        pkmn::database::item_entry entry(item, get_game());
        if((item != pkmn::e_item::NONE) && (entry.get_pocket() != get_name()))
        {
            throw std::invalid_argument("This item does not belong in this pocket.");
        }

        pkmn::enforce_bounds("Amount", amount, 1, 99);
        pkmn::enforce_value_in_vector(
            "Item name",
            item,
            {_item_slots[position].item}
        );

        // No need to copy everything
        _item_slots[position].amount = amount;
    }

    void item_list_gen2_tmhmimpl::_from_native(int)
    {
        for(size_t tm_index = 0; tm_index < PKSAV_GEN2_TM_COUNT; ++tm_index)
        {
            _item_slots[tm_index].amount = _pksav_list.tm_count[tm_index];
        }
        for(size_t hm_index = 0; hm_index < PKSAV_GEN2_HM_COUNT; ++hm_index)
        {
            _item_slots[PKSAV_GEN2_TM_COUNT+hm_index].amount = _pksav_list.hm_count[hm_index];
        }
    }

    void item_list_gen2_tmhmimpl::_to_native(int)
    {
        for(size_t tm_index = 0; tm_index < PKSAV_GEN2_TM_COUNT; ++tm_index)
        {
            _pksav_list.tm_count[tm_index] = uint8_t(_item_slots[tm_index].amount);
        }
        for(size_t hm_index = 0; hm_index < PKSAV_GEN2_HM_COUNT; ++hm_index)
        {
            _pksav_list.hm_count[hm_index] = uint8_t(_item_slots[PKSAV_GEN2_TM_COUNT+hm_index].amount);
        }
    }
}
