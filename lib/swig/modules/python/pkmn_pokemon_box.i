/*
 * Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%{
    #include "cpp_wrappers/pokemon_box.hpp"
%}

%include <attribute.i>

%ignore pkmn::swig::pokemon_box::pokemon_box();
%ignore pkmn::swig::pokemon_box::pokemon_box(const pkmn::pokemon_box::sptr&);
%ignore pkmn::swig::pokemon_box::get_pokemon(int);
%ignore pkmn::swig::pokemon_box::get_pokemon(int, const pkmn::swig::pokemon&);
%ignore pkmn::swig::pokemon_box::get_capacity();
%ignore pkmn::swig::pokemon_box::cptr();

// Convert getter/setter functions into attributes for more idiomatic Python.

%attribute(pkmn::swig::pokemon_box, pkmn::e_game, game, get_game);
%attributestring(pkmn::swig::pokemon_box, std::string, name, get_name, set_name);
%attributestring(pkmn::swig::pokemon_box, int, num_pokemon, get_num_pokemon);

%extend pkmn::swig::pokemon_box
{
    pkmn::swig::pokemon __getitem__(
        size_t position
    )
    {
        return self->get_pokemon(int(position));
    }

    void __setitem__(
        size_t position,
        const pkmn::swig::pokemon& pokemon
    )
    {
        self->set_pokemon(int(position), pokemon);
    }

    size_t __len__()
    {
        return self->get_capacity();
    }
}

%include "cpp_wrappers/pokemon_box.hpp"
