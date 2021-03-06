/*
 * Copyright (c) 2016-2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

%include <lua/lua_init.i>
PKMN_LUA_INIT

%import <pkmn_enums_lua.i>
%import <pkmn_stl_lua.i>

%{
    #include <pkmn/calculations/form.hpp>
    #include <pkmn/calculations/gender.hpp>
    #include <pkmn/calculations/nature.hpp>
    #include <pkmn/calculations/personality.hpp>
    #include <pkmn/calculations/shininess.hpp>
    #include <pkmn/calculations/size.hpp>
    #include <pkmn/calculations/spinda_spots.hpp>
    #include <pkmn/calculations/stats.hpp>

    #include <pkmn/calculations/moves/critical_hit.hpp>
    #include <pkmn/calculations/moves/damage.hpp>
    #include <pkmn/calculations/moves/hidden_power.hpp>
    #include <pkmn/calculations/moves/modifiers.hpp>
    #include <pkmn/calculations/moves/power.hpp>
    #include <pkmn/calculations/moves/natural_gift.hpp>
%}

%include <pkmn/calculations/form.hpp>
%include <pkmn/calculations/gender.hpp>
%include <pkmn/calculations/nature.hpp>
%include <pkmn/calculations/personality.hpp>
%include <pkmn/calculations/shininess.hpp>
%include <pkmn/calculations/size.hpp>
%include <pkmn/calculations/spinda_spots.hpp>
%include <pkmn/calculations/stats.hpp>

%include <pkmn/calculations/moves/critical_hit.hpp>
%include <pkmn/calculations/moves/damage.hpp>
%include <pkmn/calculations/moves/hidden_power.hpp>
%include <pkmn/calculations/moves/modifiers.hpp>
%include <pkmn/calculations/moves/power.hpp>
%include <pkmn/calculations/moves/natural_gift.hpp>
