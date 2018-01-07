/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest
{

public class Gen3PokemonTests
{
    private static string[] ContestTypes = {"Cool", "Beauty", "Cute", "Smart", "Tough"};
    private static string[] ContestLevels = {"Super", "Hyper", "Master"};
    private static string[] Ribbons = {"Champion", "Winning", "Victory", "Artist",
                                       "Effort", "Marine", "Land", "Sky",
                                       "Country", "National", "Earth", "World"};

    static private void CheckInitialRibbonMap(
        PKMN.Pokemon2 pokemon
    )
    {
        foreach(string contestType in ContestTypes)
        {
            Assert.IsTrue(pokemon.Ribbons.ContainsKey(contestType));
            Assert.IsFalse(pokemon.Ribbons[contestType]);

            foreach(string contestLevel in ContestLevels)
            {
                string ribbonName = contestType + " " + contestLevel;
                Assert.IsTrue(pokemon.Ribbons.ContainsKey(ribbonName));
                Assert.IsFalse(pokemon.Ribbons[ribbonName]);
            }
        }

        foreach(string ribbon in Ribbons)
        {
            Assert.IsTrue(pokemon.Ribbons.ContainsKey(ribbon));
            Assert.IsFalse(pokemon.Ribbons[ribbon]);
        }
    }

    static private void TestContestRibbons(
        PKMN.Pokemon2 pokemon
    )
    {
        foreach(string contestType in ContestTypes)
        {
            string ribbonName = contestType;
            string superRibbonName = contestType + " Super";
            string hyperRibbonName = contestType + " Hyper";
            string masterRibbonName = contestType + " Master";

            pokemon.Ribbons[hyperRibbonName] = true;
            Assert.IsTrue(pokemon.Ribbons[ribbonName]);
            Assert.IsTrue(pokemon.Ribbons[superRibbonName]);
            Assert.IsTrue(pokemon.Ribbons[hyperRibbonName]);
            Assert.IsFalse(pokemon.Ribbons[masterRibbonName]);

            pokemon.Ribbons[superRibbonName] = false;
            Assert.IsTrue(pokemon.Ribbons[ribbonName]);
            Assert.IsFalse(pokemon.Ribbons[superRibbonName]);
            Assert.IsFalse(pokemon.Ribbons[hyperRibbonName]);
            Assert.IsFalse(pokemon.Ribbons[masterRibbonName]);
        }
    }

    static private void TestRibbons(
        PKMN.Pokemon2 pokemon
    )
    {
        foreach(string ribbon in Ribbons)
        {
            pokemon.Ribbons[ribbon] = true;
            Assert.IsTrue(pokemon.Ribbons[ribbon]);
        }
    }

    static public void PokemonTest(
        string game,
        string species
    )
    {
        PKMN.Pokemon2 pokemon = new PKMN.Pokemon2(species, game, "", 30);

        PokemonTestParams testParams;
        bool isGamecube = (game.Equals("Colosseum") || game.Equals("XD"));

        if(isGamecube)
        {
            testParams = new PokemonTestParams(
                                 "Great Ball",
                                 new string[]{"Friend Ball", "Heal Ball"},
                                 "Razz Berry",
                                 new string[]{"Berry", "Mach Bike"},
                                 "Distant land",
                                 new string[]{"Phenac City", "Orre Colosseum"},
                                 new string[]{"New Bark Town", "Twinleaf Town"},
                                 new string[]{"Swallow", "Flamethrower", "Return", "Fire Blast"},
                                 new string[]{"Roost", "Flame Burst"},
                                 new string[]{"Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen", "Colosseum/XD", "Colosseum", "XD"},
                                 new string[]{"Gold", "HeartGold"}
                             );
        }
        else
        {
            testParams = new PokemonTestParams(
                                 "Great Ball",
                                 new string[]{"Friend Ball", "Heal Ball"},
                                 "Razz Berry",
                                 new string[]{"Berry", "Mach Bike"},
                                 "Fateful encounter",
                                 new string[]{"Petalburg Woods", "Viridian Forest"},
                                 new string[]{"New Bark Town", "Twinleaf Town"},
                                 new string[]{"Swallow", "Flamethrower", "Return", "Fire Blast"},
                                 new string[]{"Shadow Sky", "Roost"},
                                 new string[]{"Ruby", "Sapphire", "Emerald", "FireRed", "LeafGreen", "Colosseum/XD", "Colosseum", "XD"},
                                 new string[]{"Gold", "HeartGold"}
                             );
        }

        PokemonTestCommon.TestCommon(pokemon, testParams);

        CheckInitialRibbonMap(pokemon);
        TestContestRibbons(pokemon);
        TestRibbons(pokemon);

        // Gender and personality are tied, so make sure they affect each other.

        pokemon.Gender = "Female";
        Assert.Less((pokemon.Personality & 0xFF), 0xFF);
        pokemon.Gender = "Male";
        Assert.AreEqual((pokemon.Personality & 0xFF), 0xFF);

        pokemon.Personality = 0x1234AB00;
        Assert.AreEqual(pokemon.Gender, "Female");
        pokemon.Personality = 0xCD5678FF;
        Assert.AreEqual(pokemon.Gender, "Male");

        // Setting shininess should affect personality.

        pokemon.IsShiny = false;
        uint nonShinyPersonality = pokemon.Personality;

        pokemon.IsShiny = true;
        Assert.AreNotEqual(pokemon.Personality, nonShinyPersonality);

        // Shadow Pokémon should only work in Gamecube games.

        if(isGamecube)
        {
            string shadowSpecies = game.Equals("Colosseum") ? "Ledian" : "Ledyba";

            PKMN.Pokemon2 shadowPokemon = new PKMN.Pokemon2(shadowSpecies, game, "", 50);
            Assert.AreEqual(shadowPokemon.Form, "Standard");
            shadowPokemon.Form = "Shadow";
            Assert.AreEqual(shadowPokemon.Form, "Shadow");
        }
        else
        {
            Assert.Throws<ArgumentOutOfRangeException>(
                delegate
                {
                    new PKMN.Pokemon2("Ledyba", game, "Shadow", 50);
                }
            );
        }
    }
}

}
