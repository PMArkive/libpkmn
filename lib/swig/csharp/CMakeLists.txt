#
# Copyright (c) 2016-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

# Generate CamelCase conversions
FILE(GLOB_RECURSE header_files "${PKMN_SOURCE_DIR}/include/*.hpp")
ADD_CUSTOM_COMMAND(
    DEPENDS ${PKMN_SOURCE_DIR}/scripts/generate_CamelCase.py
    DEPENDS ${header_files}
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/csharp_CamelCase.i
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_CamelCase.py --language=csharp --include-dir=${PKMN_SOURCE_DIR}/include --output=${CMAKE_CURRENT_BINARY_DIR}/csharp_CamelCase.i
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_CamelCase.py --language=csharp --include-dir=${PKMN_SOURCE_DIR}/lib/swig/cpp_wrappers --output=${CMAKE_CURRENT_BINARY_DIR}/csharp_CamelCase.i
    COMMENT "Generating csharp_CamelCase.i"
)
ADD_CUSTOM_TARGET(
    csharp_CamelCase_i ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/csharp_CamelCase.i
)

# Generate C# docstrings
ADD_CUSTOM_COMMAND(
    DEPENDS ${PKMN_SOURCE_DIR}/scripts/generate_swig_docstrings.py
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/pkmn_csharp_docs.i
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_swig_docstrings.py --language=csharp --include-dir=${PKMN_SOURCE_DIR}/include --output-dir=${CMAKE_CURRENT_BINARY_DIR}
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_swig_docstrings.py --language=csharp --include-dir=${PKMN_SOURCE_DIR}/lib/swig/cpp_wrappers --output-dir=${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating C# XML docs"
)
ADD_CUSTOM_TARGET(
    pkmn_csharp_docs ALL
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/pkmn_csharp_docs.i
)

ADD_SUBDIRECTORY(CSharpSources)

#
# Set variables needed for CMake macros
#

SET(SWIG_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(SWIG_CSHARP_INCLUDE_DIRS
    ${SWIG_MODULE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${SWIG_MODULE_DIR}/..
    ${SWIG_MODULE_DIR}/../modules
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_BINARY_DIR}/include
)

SET(SWIG_CSHARP_LIBRARIES
    pkmn
)

INCLUDE(SWIGCSharp)
SWIG_CSHARP_INIT()

SWIG_BUILD_CSHARP_MODULE(breeding_csharp pkmn_csharp_breeding Breeding TRUE PKMN)
SWIG_BUILD_CSHARP_MODULE(calculations_csharp pkmn_csharp_calculations Calculations TRUE PKMN)
SWIG_BUILD_CSHARP_MODULE(pkmn_csharp pkmn_csharp_core PKMN TRUE PKMN)
SWIG_BUILD_CSHARP_MODULE(database_csharp pkmn_csharp_database Database TRUE PKMN.Database)
SWIG_BUILD_CSHARP_MODULE(gui_csharp pkmn_csharp_gui GUI TRUE PKMN)
SWIG_BUILD_CSHARP_MODULE(lists_csharp pkmn_csharp_lists Lists TRUE PKMN.Database)
SWIG_BUILD_CSHARP_MODULE(paths_csharp pkmn_csharp_paths Paths TRUE PKMN)
SWIG_BUILD_CSHARP_MODULE(stl_csharp pkmn_csharp_stl STL TRUE PKMN)

SET(swig_modules
    pkmn_csharp_breeding
    pkmn_csharp_calculations
    pkmn_csharp_core
    pkmn_csharp_database
    pkmn_csharp_gui
    pkmn_csharp_lists
    pkmn_csharp_paths
    pkmn_csharp_stl
)

# Build the DLL
SET(CSHARP_BINARY_DIRECTORY ${PKMN_BINARY_DIR}/testing/unit-tests/csharp)
FILE(MAKE_DIRECTORY ${CSHARP_BINARY_DIRECTORY})
CSHARP_BUILD_DLL("PKMN.CSharp" "${swig_modules}")
