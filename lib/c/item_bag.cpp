/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <boost/thread/mutex.hpp>

#include <pkmn-c/item_bag.h>

#include <cstdio>

#define INTERNAL_RCAST(ptr) reinterpret_cast<pkmn_item_bag_internal_t*>(ptr)
#define LIST_INTERNAL_RCAST(ptr) reinterpret_cast<pkmn_item_list_internal_t*>(ptr)

// The caller is expected to be exception-safe.
void init_item_bag(
    pkmn_item_bag2_t* item_bag
)
{
    pkmn::item_bag::sptr cpp = INTERNAL_RCAST(item_bag->_internal)->cpp;

    std::strncpy(
        item_bag->game,
        cpp->get_game().c_str(),
        sizeof(item_bag->game)
    );
    item_bag->pockets.num_pockets = cpp->get_pockets().size();
    item_bag->pockets.pockets =
        (pkmn_item_list2_t*)std::malloc(sizeof(pkmn_item_list2_t)*item_bag->pockets.num_pockets);
    item_bag->pockets.pocket_names =
        (char**)std::malloc(sizeof(char*)*item_bag->pockets.num_pockets);

    const pkmn::item_pockets_t& pockets_cpp = cpp->get_pockets();
    size_t i = 0;
    for(auto pocket_iter = pockets_cpp.begin(); pocket_iter != pockets_cpp.end(), ++i; ++pocket_iter)
    {
        pkmn::std_string_to_c_str_alloc(
            pocket_iter->first,
            &item_bag->pockets.pocket_names[i]
        );
        item_bag->pockets.pockets->_internal = new pkmn_item_list_internal_t;
        LIST_INTERNAL_RCAST(item_bag->pockets.pockets[i]._internal)->cpp = pocket_iter->second;

        init_item_list(&item_bag->pockets.pockets[i]);
    }
}

// The caller is expected to be exception-safe.
void update_item_bag(
    pkmn_item_bag2_t* item_bag
)
{
    for(size_t i = 0; i < item_bag->pockets.num_pockets; ++i)
    {
        update_item_list(&item_bag->pockets.pockets[i]);
    }
}

pkmn_error_t pkmn_item_bag2_init(
    const char* game,
    pkmn_item_bag2_t* item_bag_out
)
{
    PKMN_CHECK_NULL_PARAM(game);
    PKMN_CHECK_NULL_PARAM(item_bag_out);

    PKMN_CPP_TO_C(
        pkmn::item_bag::sptr cpp = pkmn::item_bag::make(game);
        item_bag_out->_internal = new pkmn_item_bag_internal_t;
        INTERNAL_RCAST(item_bag_out->_internal)->cpp = cpp;

        init_item_bag(item_bag_out);
    )
}

pkmn_error_t pkmn_item_bag2_free(
    pkmn_item_bag2_t* item_bag
)
{
    PKMN_CHECK_NULL_PARAM(item_bag);

    item_bag->game[0] = '\0';

    pkmn_item_pockets_free(&item_bag->pockets);

    PKMN_CPP_TO_C(
        delete INTERNAL_RCAST(item_bag->_internal);
        item_bag->_internal = NULL;
    )
}

const char* pkmn_item_bag2_strerror(
    pkmn_item_bag2_t* item_bag
)
{
    if(!item_bag)
    {
        return NULL;
    }

    try
    {
        boost::mutex::scoped_lock lock(INTERNAL_RCAST(item_bag->_internal)->error_mutex);
        return INTERNAL_RCAST(item_bag->_internal)->last_error.c_str();
    }
    catch(...)
    {
        return NULL;
    }
}

pkmn_error_t pkmn_item_bag2_add(
    pkmn_item_bag2_t* item_bag,
    const char* item,
    int amount
)
{
    PKMN_CHECK_NULL_PARAM(item_bag);
    // TODO: cleaner macro
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item, INTERNAL_RCAST(item_bag->_internal));

    // TODO: cleaner macro
    PKMN_CPP_TO_C_WITH_HANDLE(INTERNAL_RCAST(item_bag->_internal),
        pkmn::item_bag::sptr cpp = INTERNAL_RCAST(item_bag->_internal)->cpp;

        cpp->add(
            item,
            amount
        );
    )
}

pkmn_error_t pkmn_item_bag2_remove(
    pkmn_item_bag2_t* item_bag,
    const char* item,
    int amount
)
{
    PKMN_CHECK_NULL_PARAM(item_bag);
    // TODO: cleaner macro
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item, INTERNAL_RCAST(item_bag->_internal));

    // TODO: cleaner macro
    PKMN_CPP_TO_C_WITH_HANDLE(INTERNAL_RCAST(item_bag->_internal),
        pkmn::item_bag::sptr cpp = INTERNAL_RCAST(item_bag->_internal)->cpp;

        cpp->remove(
            item,
            amount
        );
    )
}

// OLD BELOW

pkmn_error_t pkmn_item_bag_make(
    pkmn_item_bag_handle_t* handle_ptr,
    const char* game_name
) {
    PKMN_CHECK_NULL_PARAM(handle_ptr);
    PKMN_CHECK_NULL_PARAM(game_name);

    PKMN_CPP_TO_C(
        pkmn::item_bag::sptr cpp = pkmn::item_bag::make(
                                       game_name
                                   );

        (*handle_ptr) = new pkmn_item_bag_t;
        (*handle_ptr)->cpp = cpp;
        (*handle_ptr)->last_error = "None";
    )
}

pkmn_error_t pkmn_item_bag_free(
    pkmn_item_bag_handle_t* handle_ptr
) {
    PKMN_CHECK_NULL_PARAM(handle_ptr);
    PKMN_CHECK_NULL_PARAM((*handle_ptr));

    PKMN_CPP_TO_C(
        delete (*handle_ptr);
        *handle_ptr = NULL;
    )
}

const char* pkmn_item_bag_strerror(
    pkmn_item_bag_handle_t handle
) {
    if(!handle) {
        return NULL;
    }

    try {
        boost::mutex::scoped_lock lock(handle->error_mutex);
        return handle->last_error.c_str();
    } catch(...) {
        return NULL;
    }
}

pkmn_error_t pkmn_item_bag_get_game(
    pkmn_item_bag_handle_t handle,
    char* game_out,
    size_t buffer_len
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(game_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        return pkmn::std_string_to_c_str_with_handle<pkmn_item_bag_handle_t>(
                   handle,
                   handle->cpp->get_game(),
                   game_out,
                   buffer_len
               );
    )
}

pkmn_error_t pkmn_item_bag_get_pocket(
    pkmn_item_bag_handle_t handle,
    const char* name,
    pkmn_item_list_handle_t* item_list_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(name, handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item_list_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        (*item_list_out) = new pkmn_item_list_t;
        (*item_list_out)->cpp = handle->cpp->get_pocket(name);
        (*item_list_out)->last_error = "None";
    )
}

pkmn_error_t pkmn_item_bag_get_pocket_names(
    pkmn_item_bag_handle_t handle,
    pkmn_string_list_t* pocket_names_out
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pocket_names_out, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_vector_std_string_to_string_list(
            handle->cpp->get_pocket_names(),
            pocket_names_out
        );
    )
}

pkmn_error_t pkmn_item_bag_add(
    pkmn_item_bag_handle_t handle,
    const char* name,
    int amount
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(name, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->add(
            name, amount
        );
    )
}

pkmn_error_t pkmn_item_bag_remove(
    pkmn_item_bag_handle_t handle,
    const char* name,
    int amount
) {
    PKMN_CHECK_NULL_PARAM(handle);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(name, handle);

    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->remove(
            name, amount
        );
    )
}
