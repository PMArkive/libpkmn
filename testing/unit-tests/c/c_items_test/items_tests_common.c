/*
 * Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "c_items_test.h"

#include <pkmn-c/database/lists.h>

#include <unity.h>

#include <string.h>

// Helper functions

void check_num_items(
    struct pkmn_item_list* p_item_list,
    size_t expected_num_items
)
{
    TEST_ASSERT_NOT_NULL(p_item_list);
    TEST_ASSERT_NOT_NULL(p_item_list->p_internal);

    enum pkmn_error error = PKMN_ERROR_NONE;
    size_t num_items = 0;

    error = pkmn_item_list_get_num_items(
                p_item_list,
                &num_items
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(expected_num_items, num_items);
}

void check_item_at_index(
    struct pkmn_item_list* p_item_list,
    size_t index,
    const char* expected_item_name,
    size_t expected_item_amount
)
{
    TEST_ASSERT_NOT_NULL(p_item_list);
    TEST_ASSERT_NOT_NULL(p_item_list->p_internal);
    TEST_ASSERT_NOT_NULL(expected_item_name);

    enum pkmn_error error = PKMN_ERROR_NONE;
    struct pkmn_item_slot item_slot =
    {
        .p_item = NULL,
        .amount = 0
    };

    error = pkmn_item_list_at(
                p_item_list,
                index,
                &item_slot
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    TEST_ASSERT_EQUAL_STRING(expected_item_name, item_slot.p_item);
    TEST_ASSERT_EQUAL(expected_item_amount, item_slot.amount);

    error = pkmn_item_slot_free(&item_slot);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

// Test functions

void test_item_list_initial_values(
    struct pkmn_item_list* p_item_list
)
{
    TEST_ASSERT_NOT_NULL(p_item_list);
    TEST_ASSERT_NOT_NULL(p_item_list->p_internal);

    TEST_ASSERT_EQUAL_STRING(
        "None",
        pkmn_item_list_strerror(p_item_list)
    );

    enum pkmn_error error = PKMN_ERROR_NONE;

    size_t num_items = 0;
    error = pkmn_item_list_get_num_items(
                p_item_list,
                &num_items
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_EQUAL(0, num_items);

    struct pkmn_item_slots item_slots =
    {
        .p_item_slots = NULL,
        .length = 0
    };
    error = pkmn_item_list_as_list(
                p_item_list,
                &item_slots
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(item_slots.p_item_slots);
    TEST_ASSERT_EQUAL(p_item_list->capacity, item_slots.length);

    for(size_t item_index = 0; item_index < item_slots.length; ++item_index)
    {
        TEST_ASSERT_EQUAL_STRING(
            "None",
            item_slots.p_item_slots[item_index].p_item
        );
        TEST_ASSERT_EQUAL(0, item_slots.p_item_slots[item_index].amount);
    }

    error = pkmn_item_slots_free(&item_slots);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

void test_item_list_out_of_range_error(
    struct pkmn_item_list* p_item_list,
    const char* item_name
)
{
    TEST_ASSERT_NOT_NULL(p_item_list);
    TEST_ASSERT_NOT_NULL(p_item_list->p_internal);
    TEST_ASSERT_NOT_NULL(item_name);

    enum pkmn_error error = PKMN_ERROR_NONE;

    size_t old_num_items = 0;
    error = pkmn_item_list_get_num_items(
                p_item_list,
                &old_num_items
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    error = pkmn_item_list_add(
                p_item_list,
                item_name,
                0
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_item_list_add(
                p_item_list,
                item_name,
                100
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_item_list_remove(
                p_item_list,
                item_name,
                0
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_item_list_remove(
                p_item_list,
                item_name,
                100
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    error = pkmn_item_list_set_item(
                p_item_list,
                0,
                item_name,
                0
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);
    error = pkmn_item_list_set_item(
                p_item_list,
                0,
                item_name,
                100
            );
    TEST_ASSERT_EQUAL(PKMN_ERROR_OUT_OF_RANGE, error);

    // This should not have changed.
    check_num_items(p_item_list, old_num_items);
}

void test_item_list_invalid_items(
    struct pkmn_item_list* p_item_list,
    const char** item_names,
    size_t num_items
)
{
    TEST_ASSERT_NOT_NULL(p_item_list);
    TEST_ASSERT_NOT_NULL(p_item_list->p_internal);
    TEST_ASSERT_NOT_NULL(item_names);
    TEST_ASSERT_TRUE(num_items > 0);

    enum pkmn_error error = PKMN_ERROR_NONE;

    size_t num_items_before = 0;
    error = pkmn_item_list_get_num_items(
                p_item_list,
                &num_items_before
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    for(size_t item_num = 0; item_num < num_items; ++item_num)
    {
        error = pkmn_item_list_add(
                    p_item_list,
                    item_names[item_num],
                    1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }

    // This should not have changed.
    check_num_items(p_item_list, num_items_before);
}

void test_item_bag_invalid_items(
    struct pkmn_item_bag* p_item_bag,
    const char** item_names,
    size_t num_items
)
{
    TEST_ASSERT_NOT_NULL(p_item_bag);
    TEST_ASSERT_NOT_NULL(p_item_bag->p_internal);
    TEST_ASSERT_NOT_NULL(item_names);
    TEST_ASSERT_TRUE(num_items > 0);

    enum pkmn_error error = PKMN_ERROR_NONE;

    for(size_t item_num = 0; item_num < num_items; ++item_num)
    {
        error = pkmn_item_bag_add(
                    p_item_bag,
                    item_names[item_num],
                    1
                );
        TEST_ASSERT_EQUAL(PKMN_ERROR_INVALID_ARGUMENT, error);
    }
}

void test_item_list_set_item(
    struct pkmn_item_list* p_item_list,
    const char** item_names,
    size_t num_items
)
{
    TEST_ASSERT_NOT_NULL(p_item_list);
    TEST_ASSERT_NOT_NULL(p_item_list->p_internal);
    TEST_ASSERT_NOT_NULL(item_names);
    TEST_ASSERT_EQUAL(3, num_items);

    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn_item_list_set_item(
        p_item_list,
        0,
        item_names[0],
        50
    );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_item_list_set_item(
        p_item_list,
        1,
        item_names[1],
        40
    );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_item_list_set_item(
        p_item_list,
        2,
        item_names[2],
        30
    );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_item_at_index(p_item_list, 0, item_names[0], 50);
    check_item_at_index(p_item_list, 1, item_names[1], 40);
    check_item_at_index(p_item_list, 2, item_names[2], 30);
    check_num_items(p_item_list, 3);

    for(size_t i = 0; i < 3; ++i)
    {
        error = pkmn_item_list_set_item(
                    p_item_list,
                    0,
                    "None",
                    0
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
    }

    check_item_at_index(p_item_list, 0, "None", 0);
    check_item_at_index(p_item_list, 1, "None", 0);
    check_item_at_index(p_item_list, 2, "None", 0);
    check_num_items(p_item_list, 0);
}

void test_item_list_add_remove(
    struct pkmn_item_list* p_item_list,
    const char** item_names,
    size_t num_items
)
{
    TEST_ASSERT_NOT_NULL(p_item_list);
    TEST_ASSERT_NOT_NULL(p_item_list->p_internal);
    TEST_ASSERT_NOT_NULL(item_names);
    TEST_ASSERT_EQUAL(8, num_items);

    enum pkmn_error error = PKMN_ERROR_NONE;

    error = pkmn_item_list_add(
                p_item_list,
                item_names[0],
                30
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_item_list_add(
                p_item_list,
                item_names[1],
                99
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    error = pkmn_item_list_add(
                p_item_list,
                item_names[2],
                1
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_item_at_index(p_item_list, 0, item_names[0], 30);
    check_item_at_index(p_item_list, 1, item_names[1], 99);
    check_item_at_index(p_item_list, 2, item_names[2], 1);
    check_num_items(p_item_list, 3);

    error = pkmn_item_list_add(
                p_item_list,
                item_names[2],
                15
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_item_at_index(p_item_list, 0, item_names[0], 30);
    check_item_at_index(p_item_list, 1, item_names[1], 99);
    check_item_at_index(p_item_list, 2, item_names[2], 16);
    check_num_items(p_item_list, 3);

    error = pkmn_item_list_remove(
                p_item_list,
                item_names[1],
                20
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_item_at_index(p_item_list, 0, item_names[0], 30);
    check_item_at_index(p_item_list, 1, item_names[1], 79);
    check_item_at_index(p_item_list, 2, item_names[2], 16);
    check_num_items(p_item_list, 3);

    error = pkmn_item_list_move(
                p_item_list,
                0,
                1
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_item_at_index(p_item_list, 0, item_names[1], 79);
    check_item_at_index(p_item_list, 1, item_names[0], 30);
    check_item_at_index(p_item_list, 2, item_names[2], 16);
    check_num_items(p_item_list, 3);

    error = pkmn_item_list_remove(
                p_item_list,
                item_names[0],
                30
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_item_at_index(p_item_list, 0, item_names[1], 79);
    check_item_at_index(p_item_list, 1, item_names[2], 16);
    check_item_at_index(p_item_list, 2, "None", 0);
    check_num_items(p_item_list, 2);

    error = pkmn_item_list_add(
                p_item_list,
                item_names[3],
                90
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_item_at_index(p_item_list, 0, item_names[1], 79);
    check_item_at_index(p_item_list, 1, item_names[2], 16);
    check_item_at_index(p_item_list, 2, item_names[3], 90);
    check_num_items(p_item_list, 3);

    error = pkmn_item_list_add(
                p_item_list,
                item_names[4],
                2
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_item_at_index(p_item_list, 0, item_names[1], 79);
    check_item_at_index(p_item_list, 1, item_names[2], 16);
    check_item_at_index(p_item_list, 2, item_names[3], 90);
    check_item_at_index(p_item_list, 3, item_names[4], 2);
    check_num_items(p_item_list, 4);

    error = pkmn_item_list_remove(
                p_item_list,
                item_names[1],
                30
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_item_at_index(p_item_list, 0, item_names[1], 49);
    check_item_at_index(p_item_list, 1, item_names[2], 16);
    check_item_at_index(p_item_list, 2, item_names[3], 90);
    check_item_at_index(p_item_list, 3, item_names[4], 2);
    check_num_items(p_item_list, 4);

    error = pkmn_item_list_add(
                p_item_list,
                item_names[5],
                12
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_item_at_index(p_item_list, 0, item_names[1], 49);
    check_item_at_index(p_item_list, 1, item_names[2], 16);
    check_item_at_index(p_item_list, 2, item_names[3], 90);
    check_item_at_index(p_item_list, 3, item_names[4], 2);
    check_item_at_index(p_item_list, 4, item_names[5], 12);
    check_num_items(p_item_list, 5);

    error = pkmn_item_list_remove(
                p_item_list,
                item_names[2],
                16
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_item_at_index(p_item_list, 0, item_names[1], 49);
    check_item_at_index(p_item_list, 1, item_names[3], 90);
    check_item_at_index(p_item_list, 2, item_names[4], 2);
    check_item_at_index(p_item_list, 3, item_names[5], 12);
    check_num_items(p_item_list, 4);

    error = pkmn_item_list_add(
                p_item_list,
                item_names[6],
                65
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_item_at_index(p_item_list, 0, item_names[1], 49);
    check_item_at_index(p_item_list, 1, item_names[3], 90);
    check_item_at_index(p_item_list, 2, item_names[4], 2);
    check_item_at_index(p_item_list, 3, item_names[5], 12);
    check_item_at_index(p_item_list, 4, item_names[6], 65);
    check_num_items(p_item_list, 5);

    error = pkmn_item_list_add(
                p_item_list,
                item_names[7],
                6
            );
    PKMN_TEST_ASSERT_SUCCESS(error);

    check_item_at_index(p_item_list, 0, item_names[1], 49);
    check_item_at_index(p_item_list, 1, item_names[3], 90);
    check_item_at_index(p_item_list, 2, item_names[4], 2);
    check_item_at_index(p_item_list, 3, item_names[5], 12);
    check_item_at_index(p_item_list, 4, item_names[6], 65);
    check_item_at_index(p_item_list, 5, item_names[7], 6);
    check_num_items(p_item_list, 6);

    // Since getting all the item slots as a whole is slow, so check
    // it once.

    struct pkmn_item_slots item_slots =
    {
        .p_item_slots = NULL,
        .length = 0
    };
    error = pkmn_item_list_as_list(
                p_item_list,
                &item_slots
            );
    PKMN_TEST_ASSERT_SUCCESS(error);
    TEST_ASSERT_NOT_NULL(item_slots.p_item_slots);
    TEST_ASSERT_EQUAL(p_item_list->capacity, item_slots.length);

    const char* expected_item_names[] =
    {
        item_names[1],
        item_names[3],
        item_names[4],
        item_names[5],
        item_names[6],
        item_names[7]
    };
    size_t expected_item_amounts[] = {49, 90, 2, 12, 65, 6};

    size_t num_expected_items = sizeof(expected_item_names)/sizeof(expected_item_names[0]);
    TEST_ASSERT_EQUAL(6, num_expected_items);

    for(size_t item_index = 0; item_index < num_expected_items; ++item_index)
    {
        TEST_ASSERT_EQUAL_STRING(
            expected_item_names[item_index],
            item_slots.p_item_slots[item_index].p_item
        );
        TEST_ASSERT_EQUAL(
            expected_item_amounts[item_index],
            item_slots.p_item_slots[item_index].amount
        );
    }

    for(size_t item_index = 0; item_index < num_expected_items; ++item_index)
    {
        error = pkmn_item_list_remove(
                    p_item_list,
                    item_slots.p_item_slots[item_index].p_item,
                    item_slots.p_item_slots[item_index].amount
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
    check_num_items(p_item_list, 0);

    error = pkmn_item_slots_free(&item_slots);
    PKMN_TEST_ASSERT_SUCCESS(error);
}

void test_item_bag_pocket_names(
    struct pkmn_item_bag* p_item_bag
)
{
    TEST_ASSERT_NOT_NULL(p_item_bag);

    enum pkmn_error error = PKMN_ERROR_NONE;

    for(size_t pocket_index = 0; pocket_index < p_item_bag->pocket_names.length; ++pocket_index)
    {
        struct pkmn_item_list pocket =
        {
            .p_name = NULL,
            .game = PKMN_GAME_NONE,
            .capacity = 0,
            .p_internal = NULL
        };

        error = pkmn_item_bag_get_pocket(
                    p_item_bag,
                    p_item_bag->pocket_names.pp_strings[pocket_index],
                    &pocket
                );
        PKMN_TEST_ASSERT_SUCCESS(error);
        TEST_ASSERT_NOT_NULL(pocket.p_internal);
        TEST_ASSERT_EQUAL_STRING(
            p_item_bag->pocket_names.pp_strings[pocket_index],
            pocket.p_name
        );
        TEST_ASSERT_EQUAL(p_item_bag->game, pocket.game);

        error = pkmn_item_list_free(&pocket);
        PKMN_TEST_ASSERT_SUCCESS(error);
    }
}
