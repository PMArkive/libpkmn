#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Source/binary dirs
SET(TESTS_SOURCE_DIR ${PKMN_SOURCE_DIR}/testing/unit-tests)
SET(TESTS_BINARY_DIR ${PKMN_BINARY_DIR}/testing/unit-tests)

INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_BINARY_DIR}/include
    ${PKMN_SOURCE_DIR}/lib
)

SET(pkmn_cpp_test_libs
    pkmn
    pkmntest
)

SET(pkmn_c_test_libs
    pkmn-c
    pkmntest-c
    unity
)

MACRO(PKMN_ADD_TEST test_name test_cmd)
    IF(CMAKE_CROSSCOMPILING)
        ADD_TEST(${test_name} ${test_cmd})
    ELSE()
        SET(TEST_CMD ${test_cmd})
        SET(DATABASE_PATH ${CMAKE_BINARY_DIR}/libpkmn-database/database/libpkmn.db)
        SET(PYTHONPATH
            "${CMAKE_BINARY_DIR}/lib/swig/python"
            "${TESTS_SOURCE_DIR}/pkmntest/python"
        )
        SET(CLASSPATH
            "${CMAKE_CURRENT_BINARY_DIR}"
            "${CMAKE_BINARY_DIR}/lib/swig/java/PKMN.jar"
        )
        IF(WIN32)
            SET(LIBRARY_PATHS
                "${Boost_LIBRARY_DIRS}"
                "${QT_LIBRARY_DIR}"
                "${PKMN_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE}"
                "${PKMN_BINARY_DIR}/lib/c/${CMAKE_BUILD_TYPE}"
                "${PKMN_BINARY_DIR}/lib/swig/csharp/${CMAKE_BUILD_TYPE}"
                "${PKMN_BINARY_DIR}/lib/swig/java/${CMAKE_BUILD_TYPE}"
                "${PKMN_BINARY_DIR}/lib/swig/python/pkmn"
                "${PKMN_BINARY_DIR}/lib/swig/python/pkmn/calculations"
                "${PKMN_BINARY_DIR}/lib/swig/python/pkmn/database"
                "${TESTS_BINARY_DIR}/pkmntest/cpp/${CMAKE_BUILD_TYPE}"
                "${TESTS_BINARY_DIR}/pkmntest/c/${CMAKE_BUILD_TYPE}"
            )
            SET(TEST_CMD ${test_cmd})
            SET(LIBRARY_DIR ${PKMN_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
            SET(DATABASE_PATH ${PKMN_BINARY_DIR}/libpkmn-database/database/libpkmn.db)
            STRING(REPLACE "/" "\\" TEST_CMD "${TEST_CMD}")
            STRING(REPLACE "/" "\\" LIBRARY_PATHS "${LIBRARY_PATHS}")
            STRING(REPLACE "/" "\\" PYTHONPATH "${PYTHONPATH}")
            STRING(REPLACE "/" "\\" CLASSPATH "${CLASSPATH}")
            STRING(REPLACE "/" "\\" DATABASE_PATH "${DATABASE_PATH}")
            CONFIGURE_FILE(
                ${TESTS_SOURCE_DIR}/unit_test_template.bat.in
                ${TESTS_BINARY_DIR}/${test_name}.bat
            @ONLY)
            ADD_TEST(${test_name} ${TESTS_BINARY_DIR}/${test_name}.bat)
        ELSE()
            IF(APPLE)
                SET(DY "DY")
            ENDIF(APPLE)
            SET(LIBRARY_PATHS
                "${Boost_LIBRARY_DIRS}"
                "${QT_LIBRARY_DIR}"
                "${PKMN_BINARY_DIR}/lib"
                "${PKMN_BINARY_DIR}/lib/c"
                "${PKMN_BINARY_DIR}/lib/swig/csharp"
                "${PKMN_BINARY_DIR}/lib/swig/java"
                "${PKMN_BINARY_DIR}/lib/swig/python/pkmn"
                "${PKMN_BINARY_DIR}/lib/swig/python/pkmn/calculations"
                "${PKMN_BINARY_DIR}/lib/swig/python/pkmn/database"
                "${TESTS_BINARY_DIR}/pkmntest/cpp"
                "${TESTS_BINARY_DIR}/pkmntest/c"
            )
            STRING(REPLACE ";" ":" LIBRARY_PATHS "${LIBRARY_PATHS}")
            STRING(REPLACE ";" ":" CLASSPATH "${CLASSPATH}")
            STRING(REPLACE ";" ":" PYTHONPATH "${PYTHONPATH}")
            CONFIGURE_FILE(
                ${TESTS_SOURCE_DIR}/unit_test_template.sh.in
                ${TESTS_BINARY_DIR}/${test_name}.sh
            @ONLY)
            ADD_TEST(${test_name} ${TESTS_BINARY_DIR}/${test_name}.sh)
        ENDIF(WIN32)
    ENDIF(CMAKE_CROSSCOMPILING)
ENDMACRO(PKMN_ADD_TEST)

MACRO(PKMN_ADD_CPP_TEST test_name test_srcs)
    ADD_EXECUTABLE(${test_name} ${test_srcs})
    SET_SOURCE_FILES_PROPERTIES(${test_srcs}
        PROPERTIES COMPILE_FLAGS "${PKMN_CXX_FLAGS}"
    )
    TARGET_LINK_LIBRARIES(${test_name} ${pkmn_cpp_test_libs})

    IF(WIN32)
        SET(cpp_test_cmd "${TESTS_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${test_name}.exe")
        STRING(REPLACE "/" "\\\\" cpp_test_cmd ${cpp_test_cmd})
    ELSE()
        SET(cpp_test_cmd "${TESTS_BINARY_DIR}/${test_name}")
    ENDIF(WIN32)
    PKMN_ADD_TEST(${test_name} ${cpp_test_cmd})
ENDMACRO(PKMN_ADD_CPP_TEST test_name test_src)

MACRO(PKMN_ADD_C_TEST test_name test_srcs)
    ADD_EXECUTABLE(${test_name} ${test_srcs})
    SET_SOURCE_FILES_PROPERTIES(${test_srcs}
        PROPERTIES COMPILE_FLAGS "${PKMN_C_FLAGS}"
    )
    TARGET_LINK_LIBRARIES(${test_name} ${pkmn_c_test_libs})

    IF(WIN32)
        SET(c_test_cmd "${TESTS_BINARY_DIR}/${CMAKE_BUILD_TYPE}/${test_name}.exe")
        STRING(REPLACE "/" "\\\\" c_test_cmd ${c_test_cmd})
    ELSE()
        SET(c_test_cmd "${TESTS_BINARY_DIR}/${test_name}")
    ENDIF(WIN32)
    PKMN_ADD_TEST(${test_name} ${c_test_cmd})
ENDMACRO(PKMN_ADD_C_TEST test_name test_src)

MACRO(PKMN_ADD_PYTHON_TEST test_name)
    SET(CMD "\"${PYTHON_EXECUTABLE}\" \"${CMAKE_CURRENT_SOURCE_DIR}/${test_name}.py\"")
    PKMN_ADD_TEST(${test_name} ${CMD})
ENDMACRO(PKMN_ADD_PYTHON_TEST)
