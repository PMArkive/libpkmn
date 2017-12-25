/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMN_C_ITEM_BAG_H
#define PKMN_C_ITEM_BAG_H

#include <pkmn-c/config.h>
#include <pkmn-c/error.h>

#include <pkmn-c/item_list.h>
#include <pkmn-c/types/item_pockets.h>
#include <pkmn-c/types/string_types.h>

typedef struct
{
    char* game;
    pkmn_item_pockets_t pockets;

    void* _internal;
} pkmn_item_bag_t;

#ifdef __cplusplus
extern "C" {
#endif

PKMN_C_API pkmn_error_t pkmn_item_bag_init(
    const char* game,
    pkmn_item_bag_t* item_bag_out
);

PKMN_C_API pkmn_error_t pkmn_item_bag_free(
    pkmn_item_bag_t* item_bag
);

PKMN_C_API const char* pkmn_item_bag_strerror(
    pkmn_item_bag_t* item_bag
);

PKMN_C_API pkmn_error_t pkmn_item_bag_get_pocket(
    pkmn_item_bag_t* item_bag,
    const char* pocket_name,
    pkmn_item_list_t** item_list_out
);

PKMN_C_API pkmn_error_t pkmn_item_bag_add(
    pkmn_item_bag_t* item_bag,
    const char* item,
    int amount
);

PKMN_C_API pkmn_error_t pkmn_item_bag_remove(
    pkmn_item_bag_t* item_bag,
    const char* item,
    int amount
);

#ifdef __cplusplus
}
#endif

#endif /* PKMN_C_ITEM_BAG_H */
