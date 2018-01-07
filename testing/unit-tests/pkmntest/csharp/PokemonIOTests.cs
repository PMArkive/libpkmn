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

public class PokemonIOTests
{
    private static System.Random rng = new System.Random();

    private static string LibPKMNTestFiles = Environment.GetEnvironmentVariable("LIBPKMN_TEST_FILES");
    private static string TmpDir = PKMN.Paths.GetTmpDir();

    public static void TestLoadingAndSavingPK1(
        string game
    )
    {
        string pk1Path = System.IO.Path.Combine(TmpDir, String.Format("{0}_{1}.pk1", game, rng.Next()));

        PKMN.Pokemon2 randomPokemon = Util.GetRandomPokemon(game);
        randomPokemon.ExportToFile(pk1Path);

        PKMN.Pokemon2 importedPokemon = new PKMN.Pokemon2(pk1Path);
        Util.ComparePokemon(randomPokemon, importedPokemon);

        System.IO.File.Delete(pk1Path);
    }

    public static void TestLoadingAndSavingPK2(
        string game
    )
    {
        string pk2Path = System.IO.Path.Combine(TmpDir, String.Format("{0}_{1}.pk2", game, rng.Next()));

        PKMN.Pokemon2 randomPokemon = Util.GetRandomPokemon(game);
        randomPokemon.ExportToFile(pk2Path);

        PKMN.Pokemon2 importedPokemon = new PKMN.Pokemon2(pk2Path);
        Util.ComparePokemon(randomPokemon, importedPokemon);

        System.IO.File.Delete(pk2Path);
    }

    public static void TestLoadingAndSaving3GPKM(
        string game
    )
    {
        string _3gpkmPath = System.IO.Path.Combine(TmpDir, String.Format("{0}_{1}.3gpkm", game, rng.Next()));

        PKMN.Pokemon2 randomPokemon = Util.GetRandomPokemon(game);
        randomPokemon.ExportToFile(_3gpkmPath);

        PKMN.Pokemon2 importedPokemon = new PKMN.Pokemon2(_3gpkmPath);
        Util.ComparePokemon(randomPokemon, importedPokemon);

        System.IO.File.Delete(_3gpkmPath);
    }

    public static void TestOutside3GPKM()
    {
        /*
         * Test files in repo and compare to known values.
         */
        string _3GPKMDir = System.IO.Path.Combine(LibPKMNTestFiles, "3gpkm");

        PKMN.Pokemon2 mightyena = new PKMN.Pokemon2(
                                          System.IO.Path.Combine(_3GPKMDir, "MIGHTYENA.3gpkm")
                                      );
        Assert.AreEqual(mightyena.Species, "Mightyena");
        Assert.AreEqual(mightyena.Game, "Emerald");
        Assert.AreEqual(mightyena.Form, "Standard");
        Assert.AreEqual(mightyena.Nickname, "MIGHTYENA");
        Assert.IsFalse(mightyena.IsShiny);
        Assert.AreEqual(mightyena.HeldItem, "Heart Scale");
        Assert.AreEqual(mightyena.OriginalTrainerName, "A");
        Assert.AreEqual(mightyena.OriginalTrainerPublicID, 61415);
        Assert.AreEqual(mightyena.OriginalTrainerSecretID, 3417);
        Assert.AreEqual(mightyena.OriginalTrainerID, 223997927);
        Assert.AreEqual(mightyena.OriginalTrainerGender, "Female");
        Assert.AreEqual(mightyena.CurrentTrainerFriendship, 254);
        Assert.AreEqual(mightyena.Ability, "Intimidate");
        Assert.AreEqual(mightyena.Ball, "Great Ball");
        Assert.AreEqual(mightyena.LevelMet, 25);
        Assert.AreEqual(mightyena.LocationMet, "Route 120");
        Assert.AreEqual(mightyena.OriginalGame, "Emerald");
        Assert.AreEqual(mightyena.Personality, 3557601241);
        Assert.AreEqual(mightyena.Experience, 128734);
        Assert.AreEqual(mightyena.Level, 50);

        Assert.AreEqual(mightyena.Markings.Count, 4);
        foreach(string marking in mightyena.Markings.Keys)
        {
            Assert.IsFalse(mightyena.Markings[marking]);
        }

        Assert.AreEqual(mightyena.Ribbons.Count, 32);
        foreach(string ribbon in mightyena.Ribbons.Keys)
        {
            if(ribbon.Equals("Champion")) {
                Assert.IsTrue(mightyena.Ribbons[ribbon]);
            } else {
                Assert.IsFalse(mightyena.Ribbons[ribbon]);
            }
        }

        Assert.AreEqual(mightyena.ContestStats.Count, 6);
        foreach(string contestStat in mightyena.ContestStats.Keys)
        {
            Assert.AreEqual(mightyena.ContestStats[contestStat], 0);
        }

        string[] expectedMightyenaMoves =
        {
            "Crunch", "Strength", "Shadow Ball", "Double-Edge"
        };
        Assert.AreEqual(mightyena.Moves.Count, 4);
        for(int i = 0; i < 4; ++i)
        {
            Assert.AreEqual(mightyena.Moves[i].Move, expectedMightyenaMoves[i]);
        }

        Assert.AreEqual(mightyena.EVs.Count, 6);
        Assert.AreEqual(mightyena.EVs["HP"], 30);
        Assert.AreEqual(mightyena.EVs["Attack"], 110);
        Assert.AreEqual(mightyena.EVs["Defense"], 32);
        Assert.AreEqual(mightyena.EVs["Speed"], 48);
        Assert.AreEqual(mightyena.EVs["Special Attack"], 17);
        Assert.AreEqual(mightyena.EVs["Special Defense"], 83);

        Assert.AreEqual(mightyena.IVs.Count, 6);
        Assert.AreEqual(mightyena.IVs["HP"], 26);
        Assert.AreEqual(mightyena.IVs["Attack"], 28);
        Assert.AreEqual(mightyena.IVs["Defense"], 4);
        Assert.AreEqual(mightyena.IVs["Speed"], 13);
        Assert.AreEqual(mightyena.IVs["Special Attack"], 25);
        Assert.AreEqual(mightyena.IVs["Special Defense"], 26);

        Assert.AreEqual(mightyena.Stats.Count, 6);
        Assert.AreEqual(mightyena.Stats["HP"], 146);
        Assert.AreEqual(mightyena.Stats["Attack"], 122);
        Assert.AreEqual(mightyena.Stats["Defense"], 81);
        Assert.AreEqual(mightyena.Stats["Speed"], 87);
        Assert.AreEqual(mightyena.Stats["Special Attack"], 79);
        Assert.AreEqual(mightyena.Stats["Special Defense"], 88);
    }
}

}
