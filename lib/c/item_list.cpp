/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "cpp_to_c.hpp"
#include "error_internal.hpp"

#include <pkmn-c/item_list.h>

pkmn_error_t pkmn_item_list_make(
    pkmn_item_list_handle_t* handle_ptr,
    const char* item_list_name,
    const char* game_name
) {
    PKMN_CPP_TO_C(
        pkmn::item_list::sptr cpp = pkmn::item_list::make(
                                        item_list_name,
                                        game_name
                                    );

        (*handle_ptr) = new pkmn_item_list_t;
        (*handle_ptr)->cpp = cpp;
        (*handle_ptr)->last_error = "None";
    )
}

pkmn_error_t pkmn_item_list_free(
    pkmn_item_list_handle_t* handle_ptr
) {
    PKMN_CPP_TO_C(
        delete (*handle_ptr);
        *handle_ptr = NULL;
    )
}

const char* pkmn_item_list_strerror(
    pkmn_item_list_handle_t handle
) {
    return handle->last_error.c_str();
}

pkmn_error_t pkmn_item_list_get_name(
    pkmn_item_list_handle_t handle,
    char* name_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_string_to_c_str(
            handle->cpp->get_name(),
            name_out,
            buffer_len,
            actual_strlen_out
        )
    )
}

pkmn_error_t pkmn_item_list_get_game(
    pkmn_item_list_handle_t handle,
    char* game_out,
    size_t buffer_len,
    size_t* actual_strlen_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_string_to_c_str(
            handle->cpp->get_game(),
            game_out,
            buffer_len,
            actual_strlen_out
        )
    )
}

pkmn_error_t pkmn_item_list_get_capacity(
    pkmn_item_list_handle_t handle,
    int* capacity_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *capacity_out = handle->cpp->get_capacity();
    )
}

pkmn_error_t pkmn_item_list_get_num_items(
    pkmn_item_list_handle_t handle,
    int* num_items_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        *num_items_out = handle->cpp->get_num_items();
    )
}

pkmn_error_t pkmn_item_list_at(
    pkmn_item_list_handle_t handle,
    int position,
    pkmn_item_slot_t* item_slot_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::pkmn_item_slot_cpp_to_c(
            handle->cpp->at(position),
            item_slot_out
        );
    )
}

pkmn_error_t pkmn_item_list_add(
    pkmn_item_list_handle_t handle,
    const char* name,
    int amount
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->add(
            name, amount
        );
    )
}

pkmn_error_t pkmn_item_list_remove(
    pkmn_item_list_handle_t handle,
    const char* name,
    int amount
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->remove(
            name, amount
        );
    )
}

pkmn_error_t pkmn_item_list_move(
    pkmn_item_list_handle_t handle,
    int old_position,
    int new_position
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        handle->cpp->move(
            old_position,
            new_position
        );
    )
}

pkmn_error_t pkmn_item_list_get_valid_items(
    pkmn_item_list_handle_t handle,
    pkmn_string_list_t* string_list_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::std_vector_std_string_to_string_list(
            handle->cpp->get_valid_items(),
            string_list_out,
            list_length_out
        );
    )
}

pkmn_error_t pkmn_item_list_as_array(
    pkmn_item_list_handle_t handle,
    pkmn_item_slots_t* array_out,
    size_t* list_length_out
) {
    PKMN_CPP_TO_C_WITH_HANDLE(handle,
        pkmn::pkmn_item_slots_cpp_to_c(
            handle->cpp->as_vector(),
            array_out,
            list_length_out
        );
    )
}