/*
 * Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

using System;
using NUnit.Framework;

namespace PKMNTest {

public class GBAItemsTest {

    private static string[] AllPocketItemNames = {
        "Potion", "Mach Bike", "Great Ball", "TM01",
        "Aspear Berry", "Wailmer Pail", "Master Ball", "HM04"
    };
    private static string[] WrongGameAllPocketItemNames = {
        "Pink Bow", "Black Sludge",
        "Ein File S", "Gonzap's Key",
        "GS Ball", "Poffin Items",
        "TM51",
        "Berry", "Occa Berry"
    };

    public static void ItemPocketTest(
        PKMN.ItemList itemPocket,
        string game
    ) {
        int expectedCapacity = 0;
        if(game.Equals("Ruby") || game.Equals("Sapphire")) {
            expectedCapacity = 20;
        } else if(game.Equals("Emerald")) {
            expectedCapacity = 30;
        } else {
            expectedCapacity = 42;
        }

        // Check unchanging and initial values.
        Assert.AreEqual(itemPocket.GetName(), "Items");
        Assert.AreEqual(itemPocket.GetGame(), game);
        Assert.AreEqual(itemPocket.GetCapacity(), expectedCapacity);
        Assert.AreEqual(itemPocket.GetNumItems(), 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(itemPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            itemPocket,
            "Potion"
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemPocket,
            new string[]{"Bicycle", "Master Ball", "HM01", "Razz Berry"}
        );

        // Confirm items from other generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemPocket,
            new string[]{"Pink Bow", "Black Sludge", "Binding Band", "Beedrillite"}
        );

        // Make sure we can't add items from Gamecube games.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemPocket,
            new string[]{"Time Flute", "Poké Snack"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            itemPocket,
            new string[]{"Potion", "Orange Mail", "Lava Cookie", "Stardust",
                         "Shadow Mail", "Pink Scarf", "Antidote", "Green Shard"}
        );

        PKMN.StringList validItems = itemPocket.GetValidItems();
        Assert.Greater(validItems.Count, 0);
    }

    public static void KeyItemPocketTest(
        PKMN.ItemList keyItemPocket,
        string game
    ) {
        int expectedCapacity = 0;
        if(game.Equals("Ruby") || game.Equals("Sapphire")) {
            expectedCapacity = 20;
        } else {
            expectedCapacity = 30;
        }

        // Check unchanging and initial values.
        Assert.AreEqual(keyItemPocket.GetName(), "Key Items");
        Assert.AreEqual(keyItemPocket.GetGame(), game);
        Assert.AreEqual(keyItemPocket.GetCapacity(), expectedCapacity);
        Assert.AreEqual(keyItemPocket.GetNumItems(), 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(keyItemPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            keyItemPocket,
            "Basement Key"
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new string[]{"Potion", "Master Ball", "HM01", "Razz Berry"}
        );

        // Confirm items from other generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new string[]{"GS Ball", "Poffin Items", "DNA Splicers", "Aqua Suit"}
        );

        // Confirm items from incompatible Generation III games can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            keyItemPocket,
            new string[]{"Ein File S", "Powerup Part", "Gonzap's Key", "Krane Memo 1"}
        );
        if(game.Equals("Ruby") || game.Equals("Sapphire")) {
            ItemsTestsCommon.TestItemListInvalidItems(
                keyItemPocket,
                new string[]{"Helix Fossil", "Tea", "Ruby"}
            );
        }
        if(!game.Equals("Emerald")) {
            ItemsTestsCommon.TestItemListInvalidItems(
                keyItemPocket,
                new string[]{"Magma Emblem", "Old Sea Map"}
            );
        }

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            keyItemPocket,
            new string[]{"Wailmer Pail", "Basement Key", "Meteorite", "Old Rod",
                         "Red Orb", "Root Fossil", "Contest Pass", "Eon Ticket"}
        );

        PKMN.StringList validItems = keyItemPocket.GetValidItems();
        Assert.Greater(validItems.Count, 0);
    }

    public static void BallPocketTest(
        PKMN.ItemList ballPocket,
        string game
    ) {
        int expectedCapacity = 0;
        if(game.Equals("FireRed") || game.Equals("LeafGreen")) {
            expectedCapacity = 13;
        } else {
            expectedCapacity = 16;
        }

        // Check unchanging and initial values.
        Assert.AreEqual(ballPocket.GetName(), "Poké Balls");
        Assert.AreEqual(ballPocket.GetGame(), game);
        Assert.AreEqual(ballPocket.GetCapacity(), expectedCapacity);
        Assert.AreEqual(ballPocket.GetNumItems(), 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(ballPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            ballPocket,
            "Master Ball"
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            ballPocket,
            new string[]{"Potion", "Bicycle", "HM01", "Razz Berry"}
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            ballPocket,
            new string[]{"Moon Ball", "Heal Ball", "Dream Ball"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            ballPocket,
            new string[]{"Master Ball", "Ultra Ball", "Great Ball", "Poké Ball",
                         "Safari Ball", "Net Ball", "Dive Ball", "Nest Ball"}
        );

        PKMN.StringList validItems = ballPocket.GetValidItems();
        Assert.Greater(validItems.Count, 0);
    }

    public static void TMHMPocketTest(
        PKMN.ItemList tmhmPocket,
        string game
    ) {
        int expectedCapacity = 0;
        string expectedName = "";

        if(game.Equals("FireRed") || game.Equals("LeafGreen")) {
            expectedCapacity = 58;
            expectedName = "TM Case";
        } else {
            expectedCapacity = 64;
            expectedName = "TMs & HMs";
        }

        // Check unchanging and initial values.
        Assert.AreEqual(tmhmPocket.GetName(), expectedName);
        Assert.AreEqual(tmhmPocket.GetGame(), game);
        Assert.AreEqual(tmhmPocket.GetCapacity(), expectedCapacity);
        Assert.AreEqual(tmhmPocket.GetNumItems(), 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(tmhmPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            tmhmPocket,
            "TM01"
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            tmhmPocket,
            new string[]{"Potion", "Bicycle", "Great Ball", "Razz Berry"}
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            tmhmPocket,
            new string[]{"TM51"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            tmhmPocket,
            new string[]{"TM01", "HM01", "TM02", "HM02",
                         "TM03", "HM03", "TM04", "HM04"}
        );

        PKMN.StringList validItems = tmhmPocket.GetValidItems();
        Assert.Greater(validItems.Count, 0);
    }

    public static void BerryPocketTest(
        PKMN.ItemList berryPocket,
        string game
    ) {
        int expectedCapacity = 0;
        string expectedName = "";

        if(game.Equals("FireRed") || game.Equals("LeafGreen")) {
            expectedCapacity = 43;
            expectedName = "Berry Pouch";
        } else {
            expectedCapacity = 46;
            expectedName = "Berries";
        }

        // Check unchanging and initial values.
        Assert.AreEqual(berryPocket.GetName(), expectedName);
        Assert.AreEqual(berryPocket.GetGame(), game);
        Assert.AreEqual(berryPocket.GetCapacity(), expectedCapacity);
        Assert.AreEqual(berryPocket.GetNumItems(), 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(berryPocket);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            berryPocket,
            "Razz Berry"
        );

        // Confirm items from other pockets can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            berryPocket,
            new string[]{"Potion", "Bicycle", "Great Ball", "HM02"}
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            berryPocket,
            new string[]{"Berry", "Occa Berry", "Roseli Berry"}
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            berryPocket,
            new string[]{"Cheri Berry", "Razz Berry", "Lum Berry", "Pinap Berry",
                         "Aspear Berry", "Iapapa Berry", "Wiki Berry", "Apicot Berry"}
        );

        PKMN.StringList validItems = berryPocket.GetValidItems();
        Assert.Greater(validItems.Count, 0);
    }

    public static void ItemPCTest(
        PKMN.ItemList itemPC,
        string game
    ) {
        // Check unchanging and initial values.
        Assert.AreEqual(itemPC.GetName(), "PC");
        Assert.AreEqual(itemPC.GetGame(), game);
        Assert.AreEqual(itemPC.GetCapacity(), 50);
        Assert.AreEqual(itemPC.GetNumItems(), 0);

        // Make sure item slots start as correctly empty.
        ItemsTestsCommon.TestItemListEmptySlots(itemPC);

        // Confirm exceptions are thrown when expected.
        ItemsTestsCommon.TestItemListIndexOutOfRangeException(
            itemPC,
            "Potion"
        );

        // Confirm items from later generations can't be added.
        ItemsTestsCommon.TestItemListInvalidItems(
            itemPC,
            WrongGameAllPocketItemNames
        );

        // Start adding and removing stuff, and make sure the numbers are accurate.
        ItemsTestsCommon.TestItemListAddRemove(
            itemPC,
            AllPocketItemNames
        );

        PKMN.StringList validItems = itemPC.GetValidItems();
        PKMN.StringList fullItemList = PKMN.Database.GetItemList(game);
        Assert.AreEqual(validItems.Count, fullItemList.Count);
    }

    public static void ItemBagTest(
        PKMN.ItemBag itemBag,
        string game
    ) {
        string tmhmPocketName = "";
        string berryPocketName = "";
        if(game.Equals("FireRed") || game.Equals("LeafGreen")) {
            tmhmPocketName = "TM Case";
            berryPocketName = "Berry Pouch";
        } else {
            tmhmPocketName = "TMs & HMs";
            berryPocketName = "Berries";
        }

        // Check unchanging and initial values.
        Assert.AreEqual(itemBag.GetGame(), game);

        PKMN.ItemPockets itemPockets = itemBag.GetPockets();
        Assert.AreEqual(itemPockets.Count, 5);

        ItemPocketTest(itemPockets["Items"], game);
        KeyItemPocketTest(itemPockets["Key Items"], game);
        BallPocketTest(itemPockets["Poké Balls"], game);
        TMHMPocketTest(itemPockets[tmhmPocketName], game);
        BerryPocketTest(itemPockets[berryPocketName], game);

        // Make sure adding items through the bag adds to the proper pockets.
        Assert.AreEqual(itemPockets["Items"].GetNumItems(), 0);
        Assert.AreEqual(itemPockets["Key Items"].GetNumItems(), 0);
        Assert.AreEqual(itemPockets["Poké Balls"].GetNumItems(), 0);
        Assert.AreEqual(itemPockets[tmhmPocketName].GetNumItems(), 0);
        Assert.AreEqual(itemPockets[berryPocketName].GetNumItems(), 0);
        foreach(string itemName in AllPocketItemNames) {
            itemBag.Add(itemName, 5);
        }

        Assert.AreEqual(itemPockets["Items"][0].Item.GetName(), "Potion");
        Assert.AreEqual(itemPockets["Items"][0].Amount, 5);
        Assert.AreEqual(itemPockets["Items"][1].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Items"][1].Amount, 0);

        Assert.AreEqual(itemPockets["Key Items"][0].Item.GetName(), "Mach Bike");
        Assert.AreEqual(itemPockets["Key Items"][0].Amount, 5);
        Assert.AreEqual(itemPockets["Key Items"][1].Item.GetName(), "Wailmer Pail");
        Assert.AreEqual(itemPockets["Key Items"][1].Amount, 5);
        Assert.AreEqual(itemPockets["Key Items"][2].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Key Items"][2].Amount, 0);

        Assert.AreEqual(itemPockets["Poké Balls"][0].Item.GetName(), "Great Ball");
        Assert.AreEqual(itemPockets["Poké Balls"][0].Amount, 5);
        Assert.AreEqual(itemPockets["Poké Balls"][1].Item.GetName(), "Master Ball");
        Assert.AreEqual(itemPockets["Poké Balls"][1].Amount, 5);
        Assert.AreEqual(itemPockets["Poké Balls"][2].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Poké Balls"][2].Amount, 0);

        Assert.AreEqual(itemPockets[tmhmPocketName][0].Item.GetName(), "TM01");
        Assert.AreEqual(itemPockets[tmhmPocketName][0].Amount, 5);
        Assert.AreEqual(itemPockets[tmhmPocketName][1].Item.GetName(), "HM04");
        Assert.AreEqual(itemPockets[tmhmPocketName][1].Amount, 5);
        Assert.AreEqual(itemPockets[tmhmPocketName][2].Item.GetName(), "None");
        Assert.AreEqual(itemPockets[tmhmPocketName][2].Amount, 0);

        Assert.AreEqual(itemPockets[berryPocketName][0].Item.GetName(), "Aspear Berry");
        Assert.AreEqual(itemPockets[berryPocketName][0].Amount, 5);
        Assert.AreEqual(itemPockets[berryPocketName][1].Item.GetName(), "None");
        Assert.AreEqual(itemPockets[berryPocketName][1].Amount, 0);

        // Make sure removing items through the bag removes from the proper pockets.
        foreach(string itemName in AllPocketItemNames) {
            itemBag.Remove(itemName, 5);
        }

        Assert.AreEqual(itemPockets["Items"][0].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Items"][0].Amount, 0);
        Assert.AreEqual(itemPockets["Items"][1].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Items"][1].Amount, 0);

        Assert.AreEqual(itemPockets["Key Items"][0].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Key Items"][0].Amount, 0);
        Assert.AreEqual(itemPockets["Key Items"][1].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Key Items"][1].Amount, 0);
        Assert.AreEqual(itemPockets["Key Items"][2].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Key Items"][2].Amount, 0);

        Assert.AreEqual(itemPockets["Poké Balls"][0].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Poké Balls"][0].Amount, 0);
        Assert.AreEqual(itemPockets["Poké Balls"][1].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Poké Balls"][1].Amount, 0);
        Assert.AreEqual(itemPockets["Poké Balls"][2].Item.GetName(), "None");
        Assert.AreEqual(itemPockets["Poké Balls"][2].Amount, 0);

        Assert.AreEqual(itemPockets[tmhmPocketName][0].Item.GetName(), "None");
        Assert.AreEqual(itemPockets[tmhmPocketName][0].Amount, 0);
        Assert.AreEqual(itemPockets[tmhmPocketName][1].Item.GetName(), "None");
        Assert.AreEqual(itemPockets[tmhmPocketName][1].Amount, 0);
        Assert.AreEqual(itemPockets[tmhmPocketName][2].Item.GetName(), "None");
        Assert.AreEqual(itemPockets[tmhmPocketName][2].Amount, 0);

        Assert.AreEqual(itemPockets[berryPocketName][0].Item.GetName(), "None");
        Assert.AreEqual(itemPockets[berryPocketName][0].Amount, 0);
        Assert.AreEqual(itemPockets[berryPocketName][1].Item.GetName(), "None");
        Assert.AreEqual(itemPockets[berryPocketName][1].Amount, 0);

        /*
         * Make sure we can't add items from later generations or incompatible
         * Generation III games.
         */
        ItemsTestsCommon.TestItemBagInvalidItems(
            itemBag,
            WrongGameAllPocketItemNames
        );
    }
}

}