--
-- Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local pkmntest_utils = require("pkmntest_utils")
local luaunit = require("luaunit")

local pokemon_pc_tests = {}
pokemon_pc_tests.pokemon_box = {}
pokemon_pc_tests.pokemon_pc = {}

-- Stupid hacky functions to be able to test indexing and attributes

function pokemon_pc_tests.pokemon_box.get_pokemon(box, index)
    local pokemon = box[index]
end

function pokemon_pc_tests.pokemon_box.set_pokemon(box, index, pokemon)
    party[index] = pokemon
end

function pokemon_pc_tests.pokemon_box.set_name(box, name)
    box.name = name
end

function pokemon_pc_tests.pokemon_pc.get_box(pc, index)
    local box = pc[index]
end

-- Actual test functions

function pokemon_pc_tests.test_empty_pokemon_box(box, game)
    local generation = pkmntest_utils.GAME_TO_GENERATION[game]

    luaunit.assertEquals(box.game, game)
    luaunit.assertEquals(box.name, "")

    for box_index = 1, #box
    do
        luaunit.assertEquals(box[box_index].species, pkmn.species.NONE)
        luaunit.assertEquals(box[box_index].game, game)

        for move_index = 1, #box[box_index].moves
        do
            luaunit.assertEquals(box[box_index].moves[move_index].move, pkmn.move.NONE)
            luaunit.assertEquals(box[box_index].moves[move_index].pp, 0)
        end
    end

    -- Make sure trying to get a Pokémon at an invalid index fails.
    luaunit.assertError(
        pokemon_pc_tests.pokemon_box.get_pokemon,
        box,
        0
    )
    luaunit.assertError(
        pokemon_pc_tests.pokemon_box.get_pokemon,
        box,
        #box+1
    )
end

function pokemon_pc_tests.test_box_name(box)
    local generation = pkmntest_utils.GAME_TO_GENERATION[box.game]

    if generation == 1
    then
        luaunit.assertEquals(box.name, "")
        luaunit.assertError(
            pokemon_pc_tests.pokemon_box.set_name,
            box,
            "ABCDEFGH"
        )
    else
        luaunit.assertError(
            pokemon_pc_tests.pokemon_box.set_name,
            box,
            "ABCDEFGHI"
        )

        box.name = "ABCDEFGH"
        luaunit.assertEquals(box.name, "ABCDEFGH")
    end
end

function pokemon_pc_tests.test_setting_pokemon(box)
    local game = box.game
    local generation = pkmntest_utils.GAME_TO_GENERATION[game]

    local original_first = box[1]
    local original_second = box[2]

    -- Make sure we can't set Pokémon at invalid indices.
    luaunit.assertError(
        pokemon_pc_tests.pokemon_box.set_pokemon,
        box,
        0,
        original_first
    )
    luaunit.assertError(
        pokemon_pc_tests.pokemon_box.set_pokemon,
        box,
        #box+1,
        original_second
    )

    -- Create Pokémon and place in box. The original variables should
    -- still have the same underlying Pokémon.
    local bulbasaur = pkmn.pokemon(pkmn.species.BULBASAUR, game, "", 5)
    local charmander = pkmn.pokemon(pkmn.species.CHARMANDER, game, "", 5)
    local squirtle = pkmn.pokemon(pkmn.species.SQUIRTLE, game, "", 5)

    box[1] = bulbasaur
    luaunit.assertEquals(box.num_pokemon, 1)
    box[2] = charmander
    luaunit.assertEquals(box.num_pokemon, 2)

    -- Replace one of the new ones.
    box[1] = squirtle
    luaunit.assertEquals(box.num_pokemon, 2)

    -- Make sure we can't copy a Pokémon to itself.
    luaunit.assertError(
        pokemon_pc_tests.pokemon_box.set_pokemon,
        box,
        2,
        box[2]
    )
    luaunit.assertEquals(box.num_pokemon, 2)

    -- Copy a Pokémon whose memory is already part of the box.
    box[3] = box[2]
    luaunit.assertEquals(box.num_pokemon, 3)

    -- We should always be able to clear the last contiguous Pokémon.
    box[3] = original_first
    luaunit.assertEquals(box.num_pokemon, 2)
    luaunit.assertEquals(box[3].species, pkmn.species.NONE)

    -- Put it back.
    box[3] = box[2]
    luaunit.assertEquals(box.num_pokemon, 3)

    -- Check that Pokémon can be placed non-contiguously in the correct games.
    if generation <= 2
    then
        luaunit.assertError(
            pokemon_pc_tests.pokemon_box.set_pokemon,
            box,
            2,
            original_first
        )
        luaunit.assertEquals(box.num_pokemon, 3)
        luaunit.assertEquals(box[2].species, pkmn.species.CHARMANDER)

        luaunit.assertError(
            pokemon_pc_tests.pokemon_box.set_pokemon,
            box,
            5,
            bulbasaur
        )
        luaunit.assertEquals(box.num_pokemon, 3)
        luaunit.assertEquals(box[5].species, pkmn.species.NONE)
    else
        box[2] = original_first
        luaunit.assertEquals(box.num_pokemon, 2)
        luaunit.assertEquals(box[2].species, pkmn.species.NONE)

        box[5] = bulbasaur
        luaunit.assertEquals(box.num_pokemon, 3)
        luaunit.assertEquals(box[5].species, pkmn.species.BULBASAUR)

        -- Restore it to how it was.
        box[2] = charmander
        box[5] = original_first
        luaunit.assertEquals(box[2].species, pkmn.species.CHARMANDER)
        luaunit.assertEquals(box[5].species, pkmn.species.NONE)
    end

    -- Now check everything we've created. Each variable should have
    -- the same underlying Pokémon.
    luaunit.assertEquals(box[1].species, pkmn.species.SQUIRTLE)
    luaunit.assertEquals(box[2].species, pkmn.species.CHARMANDER)
    luaunit.assertEquals(box[3].species, pkmn.species.CHARMANDER)
    luaunit.assertEquals(original_first.species, pkmn.species.NONE)
    luaunit.assertEquals(original_second.species, pkmn.species.NONE)
    luaunit.assertEquals(bulbasaur.species, pkmn.species.BULBASAUR)
    luaunit.assertEquals(charmander.species, pkmn.species.CHARMANDER)
    luaunit.assertEquals(squirtle.species, pkmn.species.SQUIRTLE)
end

function pokemon_pc_tests.test_pokemon_box(box, game)
    pokemon_pc_tests.test_empty_pokemon_box(box, game)
    pokemon_pc_tests.test_box_name(box)
    pokemon_pc_tests.test_setting_pokemon(box)
end

function pokemon_pc_tests.test_empty_pokemon_pc(pc, game)
    luaunit.assertEquals(pc.game, game)

    for i = 1, #pc
    do
        pokemon_pc_tests.test_empty_pokemon_box(pc[i], game)
    end
end

function pokemon_pc_tests.test_box_names(pc)
    local generation = pkmntest_utils.GAME_TO_GENERATION[pc.game]

    if generation == 1
    then
        luaunit.assertError(
            pokemon_pc_tests.pokemon_box.set_name,
            pc[1],
            "ABCDEFGH"
        )
    else
        for i = 1, #pc
        do
            local box_name = string.format("BOX%d", i)
            pc[i].name = box_name
        end

        for i = 1, #pc.box_names
        do
            local expected_box_name = string.format("BOX%d", i)
            luaunit.assertEquals(pc.box_names[i], expected_box_name)
        end
    end
end

function pokemon_pc_tests.test_setting_pokemon_in_boxes(pc)
    for i = 1, #pc
    do
        pokemon_pc_tests.test_setting_pokemon(pc[i])
        luaunit.assertEquals(pc[i][1].species, pkmn.species.SQUIRTLE)
        luaunit.assertEquals(pc[i][2].species, pkmn.species.CHARMANDER)
    end
end

function pokemon_pc_tests.test_pokemon_pc(pc, game)
    pokemon_pc_tests.test_empty_pokemon_pc(pc, game)
    pokemon_pc_tests.test_box_names(pc)
    pokemon_pc_tests.test_setting_pokemon_in_boxes(pc)
end

return pokemon_pc_tests
