--
-- Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
--
-- Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
-- or copy at http://opensource.org/licenses/MIT)
--

local pkmn = require("pkmn")
local luaunit = require("luaunit")

local pokemon_pc_tests = require("pokemon_pc_tests")

---
--- Red
---

function test_red_pokemon_box()
    box = pkmn.pokemon_box("Red")
    pokemon_pc_tests.test_pokemon_box(box, "Red")
end

function test_red_pokemon_pc()
    pc = pkmn.pokemon_pc("Red")
    pokemon_pc_tests.test_pokemon_pc(pc, "Red")
end

---
--- Blue
---

function test_blue_pokemon_box()
    box = pkmn.pokemon_box("Blue")
    pokemon_pc_tests.test_pokemon_box(box, "Blue")
end

function test_blue_pokemon_pc()
    pc = pkmn.pokemon_pc("Blue")
    pokemon_pc_tests.test_pokemon_pc(pc, "Blue")
end

---
--- Yellow
---

function test_yellow_pokemon_box()
    box = pkmn.pokemon_box("Yellow")
    pokemon_pc_tests.test_pokemon_box(box, "Yellow")
end

function test_yellow_pokemon_pc()
    pc = pkmn.pokemon_pc("Yellow")
    pokemon_pc_tests.test_pokemon_pc(pc, "Yellow")
end

---
--- Gold
---

function test_gold_pokemon_box()
    box = pkmn.pokemon_box("Gold")
    pokemon_pc_tests.test_pokemon_box(box, "Gold")
end

function test_gold_pokemon_pc()
    pc = pkmn.pokemon_pc("Gold")
    pokemon_pc_tests.test_pokemon_pc(pc, "Gold")
end

---
--- Silver
---

function test_silver_pokemon_box()
    box = pkmn.pokemon_box("Silver")
    pokemon_pc_tests.test_pokemon_box(box, "Silver")
end

function test_silver_pokemon_pc()
    pc = pkmn.pokemon_pc("Silver")
    pokemon_pc_tests.test_pokemon_pc(pc, "Silver")
end

---
--- Crystal
---

function test_crystal_pokemon_box()
    box = pkmn.pokemon_box("Crystal")
    pokemon_pc_tests.test_pokemon_box(box, "Crystal")
end

function test_crystal_pokemon_pc()
    pc = pkmn.pokemon_pc("Crystal")
    pokemon_pc_tests.test_pokemon_pc(pc, "Crystal")
end

---
--- Ruby
---

function test_ruby_pokemon_box()
    box = pkmn.pokemon_box("Ruby")
    pokemon_pc_tests.test_pokemon_box(box, "Ruby")
end

function test_ruby_pokemon_pc()
    pc = pkmn.pokemon_pc("Ruby")
    pokemon_pc_tests.test_pokemon_pc(pc, "Ruby")
end

---
--- Sapphire
---

function test_sapphire_pokemon_box()
    box = pkmn.pokemon_box("Sapphire")
    pokemon_pc_tests.test_pokemon_box(box, "Sapphire")
end

function test_sapphire_pokemon_pc()
    pc = pkmn.pokemon_pc("Sapphire")
    pokemon_pc_tests.test_pokemon_pc(pc, "Sapphire")
end

---
--- Emerald
---

function test_emerald_pokemon_box()
    box = pkmn.pokemon_box("Emerald")
    pokemon_pc_tests.test_pokemon_box(box, "Emerald")
end

function test_emerald_pokemon_pc()
    pc = pkmn.pokemon_pc("Emerald")
    pokemon_pc_tests.test_pokemon_pc(pc, "Emerald")
end

---
--- FireRed
---

function test_firered_pokemon_box()
    box = pkmn.pokemon_box("FireRed")
    pokemon_pc_tests.test_pokemon_box(box, "FireRed")
end

function test_firered_pokemon_pc()
    pc = pkmn.pokemon_pc("FireRed")
    pokemon_pc_tests.test_pokemon_pc(pc, "FireRed")
end

---
--- LeafGreen
---

function test_leafgreen_pokemon_box()
    box = pkmn.pokemon_box("LeafGreen")
    pokemon_pc_tests.test_pokemon_box(box, "LeafGreen")
end

function test_leafgreen_pokemon_pc()
    pc = pkmn.pokemon_pc("LeafGreen")
    pokemon_pc_tests.test_pokemon_pc(pc, "LeafGreen")
end

---
--- Colosseum
---

function test_colosseum_pokemon_box()
    box = pkmn.pokemon_box("Colosseum")
    pokemon_pc_tests.test_pokemon_box(box, "Colosseum")
end

function test_colosseum_pokemon_pc()
    pc = pkmn.pokemon_pc("Colosseum")
    pokemon_pc_tests.test_pokemon_pc(pc, "Colosseum")
end

---
--- XD
---

function test_xd_pokemon_box()
    box = pkmn.pokemon_box("XD")
    pokemon_pc_tests.test_pokemon_box(box, "XD")
end

function test_xd_pokemon_pc()
    pc = pkmn.pokemon_pc("XD")
    pokemon_pc_tests.test_pokemon_pc(pc, "XD")
end

local runner = luaunit.LuaUnit.new()
os.exit(runner:runSuite())
