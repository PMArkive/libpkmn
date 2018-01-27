/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_enum_maps.hpp"
#include "cpp_to_c.hpp"
#include "error_internal.hpp"
#include "exception_internal.hpp"

#include <boost/assert.hpp>
#include <boost/thread/mutex.hpp>

#include <pkmn-c/game_save2.h>

pkmn_error_t pkmn_game_save2_init_from_file(
    const char* filepath,
    pkmn_game_save2_t* game_save_out
)
{
    PKMN_CHECK_NULL_PARAM(filepath);
    PKMN_CHECK_NULL_PARAM(game_save_out);

    PKMN_CPP_TO_C(
        pkmn::game_save::sptr cpp = pkmn::game_save::from_file(filepath);

        pkmn::c::init_game_save(
            cpp,
            game_save_out
        );
    )
}

pkmn_error_t pkmn_game_save2_free(
    pkmn_game_save2_t* game_save_ptr
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);

    pkmn::c::free_pointer_and_set_to_null(&game_save_ptr->game);

    PKMN_CPP_TO_C(
        pkmn::c::delete_pointer_and_set_to_null(
            reinterpret_cast<pkmn_game_save_internal_t**>(&game_save_ptr->_internal)
        );
    )
}

const char* pkmn_game_save2_strerror(
    pkmn_game_save2_t* game_save_ptr
)
{
    if(!game_save_ptr)
    {
        return nullptr;
    }

    try
    {
        pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);
        if(!internal_ptr)
        {
            return nullptr;
        }

        boost::mutex::scoped_lock lock(internal_ptr->error_mutex);
        return internal_ptr->last_error.c_str();
    }
    catch(...)
    {
        return nullptr;
    }
}

// Save file actions

PKMN_C_API pkmn_error_t pkmn_game_save2_get_filepath(
    pkmn_game_save2_t* game_save_ptr,
    char* filepath_buffer,
    size_t filepath_buffer_length,
    size_t* actual_filepath_length_out
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(filepath_buffer, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_cpp_to_c(
            internal_ptr->cpp->get_filepath(),
            filepath_buffer,
            filepath_buffer_length,
            actual_filepath_length_out
        );
    )
}

pkmn_error_t pkmn_game_save2_save(
    pkmn_game_save2_t* game_save_ptr
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->save();
    )
}

pkmn_error_t pkmn_game_save2_save_as(
    pkmn_game_save2_t* game_save_ptr,
    const char* filepath
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(filepath, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->save_as(filepath);
    )
}

// Trainer info

pkmn_error_t pkmn_game_save2_get_trainer_info(
    pkmn_game_save2_t* game_save_ptr,
    pkmn_trainer_info2_t* trainer_info_out
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(trainer_info_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        // Put the values in a separate struct first so there are
        // no side effects if there's an error.
        pkmn_trainer_info2_t trainer_info;

        pkmn::c::string_cpp_to_c_alloc(
            internal_ptr->cpp->get_trainer_name(),
            &trainer_info.name
        );
        trainer_info.id.id = internal_ptr->cpp->get_trainer_id();

        if(internal_ptr->generation >= 2)
        {
            std::string cpp_gender = internal_ptr->cpp->get_trainer_gender();
            BOOST_ASSERT(pkmn::c::GENDER_BIMAP.left.count(cpp_gender) > 0);
            trainer_info.gender = pkmn::c::GENDER_BIMAP.left.at(cpp_gender);
        }
        else
        {
            trainer_info.gender = PKMN_GENDER_MALE;
        }

        *trainer_info_out = std::move(trainer_info);
    )
}

pkmn_error_t pkmn_game_save2_set_trainer_name(
    pkmn_game_save2_t* game_save_ptr,
    const char* trainer_name
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(trainer_name, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_trainer_name(trainer_name);
    )
}

pkmn_error_t pkmn_game_save2_set_trainer_id(
    pkmn_game_save2_t* game_save_ptr,
    uint32_t trainer_id
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_trainer_id(trainer_id);
    )
}

PKMN_C_API pkmn_error_t pkmn_game_save2_set_trainer_public_id(
    pkmn_game_save2_t* game_save_ptr,
    uint16_t trainer_public_id
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_trainer_public_id(trainer_public_id);
    )
}

PKMN_C_API pkmn_error_t pkmn_game_save2_set_trainer_secret_id(
    pkmn_game_save2_t* game_save_ptr,
    uint16_t trainer_secret_id
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_trainer_secret_id(trainer_secret_id);
    )
}

pkmn_error_t pkmn_game_save2_set_gender(
    pkmn_game_save2_t* game_save_ptr,
    pkmn_gender_t gender
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::enforce_value_in_vector(
            "Gender",
            gender,
            {PKMN_GENDER_MALE, PKMN_GENDER_FEMALE}
        );

        internal_ptr->cpp->set_trainer_gender(
            pkmn::c::GENDER_BIMAP.right.at(gender)
        );
    )
}

// Other fields

pkmn_error_t pkmn_game_save2_get_rival_name(
    pkmn_game_save2_t* game_save_ptr,
    char* rival_name_buffer,
    size_t rival_name_buffer_length,
    size_t* actual_rival_name_length_out
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(rival_name_buffer, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::string_cpp_to_c(
            internal_ptr->cpp->get_rival_name(),
            rival_name_buffer,
            rival_name_buffer_length,
            actual_rival_name_length_out
        );
    )
}

pkmn_error_t pkmn_game_save2_set_rival_name(
    pkmn_game_save2_t* game_save_ptr,
    const char* rival_name
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(rival_name, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_rival_name(rival_name);
    )
}

pkmn_error_t pkmn_game_save2_get_money(
    pkmn_game_save2_t* game_save_ptr,
    int* money_out
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(money_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        *money_out = internal_ptr->cpp->get_money();
    )
}

pkmn_error_t pkmn_game_save2_set_money(
    pkmn_game_save2_t* game_save_ptr,
    int money
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        internal_ptr->cpp->set_money(money);
    )
}

// Pokémon and items

pkmn_error_t pkmn_game_save2_get_pokedex(
    pkmn_game_save2_t* game_save_ptr,
    pkmn_pokedex_t* pokedex_out
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokedex_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::init_pokedex(
            internal_ptr->cpp->get_pokedex(),
            pokedex_out
        );
    )
}

pkmn_error_t pkmn_game_save2_get_pokemon_party(
    pkmn_game_save2_t* game_save_ptr,
    pkmn_pokemon_party2_t* pokemon_party_out
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_party_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::init_pokemon_party(
            internal_ptr->cpp->get_pokemon_party(),
            pokemon_party_out
        );
    )
}

pkmn_error_t pkmn_game_save2_get_pokemon_pc(
    pkmn_game_save2_t* game_save_ptr,
    pkmn_pokemon_pc2_t* pokemon_pc_out
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(pokemon_pc_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::init_pokemon_pc(
            internal_ptr->cpp->get_pokemon_pc(),
            pokemon_pc_out
        );
    )
}

pkmn_error_t pkmn_game_save2_get_item_bag(
    pkmn_game_save2_t* game_save_ptr,
    pkmn_item_bag_t* item_bag_out
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item_bag_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::init_item_bag(
            internal_ptr->cpp->get_item_bag(),
            item_bag_out
        );
    )
}

pkmn_error_t pkmn_game_save2_get_item_pc(
    pkmn_game_save2_t* game_save_ptr,
    pkmn_item_list_t* item_pc_out
)
{
    PKMN_CHECK_NULL_PARAM(game_save_ptr);
    pkmn_game_save_internal_t* internal_ptr = GAME_SAVE_INTERNAL_RCAST(game_save_ptr->_internal);
    PKMN_CHECK_NULL_PARAM_WITH_HANDLE(item_pc_out, internal_ptr);

    PKMN_CPP_TO_C_WITH_HANDLE(internal_ptr,
        pkmn::c::init_item_list(
            internal_ptr->cpp->get_item_pc(),
            item_pc_out
        );
    )
}
