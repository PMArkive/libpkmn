/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

[TestFixture]
public class CSharpBuildInfoTest {
    [Test]
    public void BoostVersionTest() {
        Assert.Greater(PKMN.BuildInfo.BoostVersion.Length, 0);
    }

    [Test]
    public void PKSavVersionTest() {
        Assert.Greater(PKMN.BuildInfo.PKSavVersion.Length, 0);
    }

    [Test]
    public void SQLite3VersionTest() {
        Assert.Greater(PKMN.BuildInfo.SQLite3Version.Length, 0);
    }

    [Test]
    public void SQLiteCppVersionTest() {
        Assert.Greater(PKMN.BuildInfo.SQLiteCppVersion.Length, 0);
    }

    [Test]
    public void SWIGVersionTest() {
        Assert.Greater(PKMN.BuildInfo.SWIGVersion.Length, 0);
    }
}