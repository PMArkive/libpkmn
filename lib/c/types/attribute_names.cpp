/*
 * Copyright (c) 2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn-c/types/attribute_names.h>

#include "error_internal.hpp"

enum pkmn_error pkmn_attribute_names_free(
    struct pkmn_attribute_names* p_attribute_names
)
{
    PKMN_CHECK_NULL_PARAM(p_attribute_names);

    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn_string_list_free(&p_attribute_names->numeric_attribute_names);
    if(!error)
    {
        error = pkmn_string_list_free(&p_attribute_names->string_attribute_names);
        if(!error)
        {
            error = pkmn_string_list_free(&p_attribute_names->boolean_attribute_names);
        }
    }

    return error;
}
