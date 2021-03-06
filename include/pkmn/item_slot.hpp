/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_ITEM_SLOT_HPP
#define PKMN_ITEM_SLOT_HPP

#include <pkmn/config.hpp>
#include <pkmn/enums/item.hpp>

#include <vector>

namespace pkmn {

    /*!
     * @brief A slot in an item list.
     *
     * An item list is made up of a list of item slots, each of which contains
     * a unique item type and the amount of that item in the given pocket. This
     * struct represents a single one of those slots.
     */
    struct item_slot
    {
        /*!
         * @brief Default constructor.
         *
         * The item entry represents an invalid item.
         */
        item_slot():
            item(pkmn::e_item::NONE),
            amount(0) {}

        /*!
         * @brief Constructor that takes in each member.
         *
         * \param slot_item item entry
         * \param slot_amount how many of the item
         */
        item_slot(
            pkmn::e_item slot_item,
            int slot_amount
        ): item(slot_item),
           amount(slot_amount) {}

        item_slot(const item_slot&) = default;
        item_slot& operator=(const item_slot&) = default;

#ifndef SWIG
        item_slot(item_slot&&) = default;
        item_slot& operator=(item_slot&&) = default;
#endif

        /*!
         * @brief Equality check.
         */
        inline bool operator==(const item_slot& rhs) const
        {
            return (this->item == rhs.item) &&
                   (this->amount == rhs.amount);
        }

        /*!
         * @brief Inequality check.
         */
        inline bool operator!=(const item_slot& rhs) const
        {
            return !operator==(rhs);
        }

        /*!
         * @brief The item in the given slot.
         */
        pkmn::e_item item;
        /*!
         * @brief How many of the item is in the given slot.
         */
        int amount;
    };

    /*!
     * @brief List of item slots.
     *
     * This is effectively a vector representation of an item list.
     */
    typedef std::vector<item_slot> item_slots_t;
}

#endif /* PKMN_ITEM_SLOT_HPP */
