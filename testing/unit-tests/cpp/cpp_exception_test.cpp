/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <pkmn/exception.hpp>

#include <pksav/error.h>

#include <boost/assign.hpp>

#include <gtest/gtest.h>

TEST(cpp_exception_test, test_feature_not_in_game_error)
{
    const std::string expected_msg = "Ribbons not in Generation II";

    try {
        throw pkmn::feature_not_in_game_error("Ribbons", "Generation II");
        FAIL() << "Did not throw";
    } catch(const pkmn::feature_not_in_game_error& e) {
        EXPECT_EQ(expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw pkmn::feature_not_in_game_error";
    }
}

TEST(cpp_exception_test, test_pksav_error)
{
    // Source: https://github.com/ncorgan/pksav/blob/master/lib/error.c
    const std::string expected_msg = "PKSav returned the following error: \"File I/O error\"";

    try {
        throw pkmn::pksav_error(PKSAV_ERROR_FILE_IO);
        FAIL() << "Did not throw";
    } catch(const pkmn::pksav_error& e) {
        EXPECT_EQ(expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw pkmn::pksav_error";
    }
}

TEST(cpp_exception_test, test_unimplemented_error)
{
    const std::string expected_msg = "Currently unimplemented.";

    try {
        throw pkmn::unimplemented_error();
        FAIL() << "Did not throw.";
    } catch(const pkmn::unimplemented_error& e) {
        EXPECT_EQ(expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw pkmn::unimplemented_error";
    }
}

TEST(cpp_exception_test, test_invalid_argument)
{
    static const std::vector<int> int_values = boost::assign::list_of
        (0)(5)(10)(15)
    ;
    const std::string int_expected_msg = "int_param: valid values 0, 5, 10, 15.";

    try {
        pkmn::throw_invalid_argument("int_param", int_values);
        FAIL() << "Did not throw.";
    } catch(const std::invalid_argument& e) {
        EXPECT_EQ(int_expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw std::invalid_argument";
    }

    static const std::vector<std::string> string_values = boost::assign::list_of
        ("A")("B")("C")("D")
    ;
    const std::string string_expected_msg = "string_param: valid values A, B, C, D.";

    try {
        pkmn::throw_invalid_argument("string_param", string_values);
        FAIL() << "Did not throw.";
    } catch(const std::invalid_argument& e) {
        EXPECT_EQ(string_expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw std::invalid_argument";
    }

    // This can happen when converting a number passed in from a SWIG wrapper.
    static const std::vector<float> float_values = boost::assign::list_of
        (0.1900000001f)(1.370000002f)(50.000003f)(12.34000004f)
    ;
    const std::string float_expected_msg = "float_param: valid values 0.19, 1.37, 50.00, 12.34.";

    try {
        pkmn::throw_invalid_argument("float_param", float_values);
        FAIL() << "Did not throw.";
    } catch(const std::invalid_argument& e) {
        EXPECT_EQ(float_expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw std::invalid_argument";
    }

    // This can happen when converting a number passed in from a SWIG wrapper.
    static const std::vector<double> double_values = boost::assign::list_of
        (0.1900000001)(1.370000002)(50.000003)(12.34000004)
    ;
    const std::string double_expected_msg = "double_param: valid values 0.19, 1.37, 50.00, 12.34.";

    try {
        pkmn::throw_invalid_argument("double_param", double_values);
        FAIL() << "Did not throw.";
    } catch(const std::invalid_argument& e) {
        EXPECT_EQ(double_expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw std::invalid_argument";
    }
}

TEST(cpp_exception_test, test_out_of_range)
{
    const int int_min = -100;
    const int int_max = 2000;
    const std::string int_expected_msg = "int_param: valid values -100-2000.";

    try {
        pkmn::throw_out_of_range("int_param", int_min, int_max);
        FAIL() << "Did not throw";
    } catch(const std::out_of_range& e) {
        EXPECT_EQ(int_expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw std::out_of_range";
    }

    const unsigned short ushort_min = 3;
    const unsigned short ushort_max = 1234;
    const std::string ushort_expected_msg = "ushort_param: valid values 3-1234.";

    try {
        pkmn::throw_out_of_range("ushort_param", ushort_min, ushort_max);
        FAIL() << "Did not throw";
    } catch(const std::out_of_range& e) {
        EXPECT_EQ(ushort_expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw std::out_of_range";
    }

    // This can happen when converting a number passed in from a SWIG wrapper.
    const float float_min = 12.230000005f;
    const float float_max = 45.670000009f;
    const std::string float_expected_msg = "float_param: valid values 12.23-45.67.";

    try {
        pkmn::throw_out_of_range("float_param", float_min, float_max);
        FAIL() << "Did not throw";
    } catch(const std::out_of_range& e) {
        EXPECT_EQ(float_expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw std::out_of_range";
    }

    // This can happen when converting a number passed in from a SWIG wrapper.
    const double double_min = 12.230000005;
    const double double_max = 45.670000009;
    const std::string double_expected_msg = "double_param: valid values 12.23-45.67.";

    try {
        pkmn::throw_out_of_range("double_param", double_min, double_max);
        FAIL() << "Did not throw";
    } catch(const std::out_of_range& e) {
        EXPECT_EQ(double_expected_msg, std::string(e.what()));
    } catch(...) {
        FAIL() << "Did not throw std::out_of_range";
    }
}