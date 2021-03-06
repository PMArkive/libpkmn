#
# Copyright (c) 2017-2018 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

INCLUDE(SWIGRuby)

#
# These files need to be in the working directory when SWIG is called.
#
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/ruby/rubycontainer.swg
    ${CMAKE_CURRENT_BINARY_DIR}/rubycontainer.swg
@ONLY)
CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/../../modules/ruby/rubyfragments.swg
    ${CMAKE_CURRENT_BINARY_DIR}/rubyfragments.swg
@ONLY)

#
# Set variables needed for CMake macros
#

SET(SWIG_MODULE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

SET(SWIG_RUBY_INCLUDE_DIRS
    ${SWIG_MODULE_DIR}/..
    ${CMAKE_CURRENT_BINARY_DIR}/..
    ${SWIG_MODULE_DIR}/../..
    ${SWIG_MODULE_DIR}/../../modules
    ${PKMN_SOURCE_DIR}/include
    ${PKMN_BINARY_DIR}/include
)

SET(SWIG_RUBY_LIBRARIES
    pkmn
)

SWIG_BUILD_RUBY_MODULE(PKMNRuby     TRUE)
SWIG_BUILD_RUBY_MODULE(Breeding     TRUE)
SWIG_BUILD_RUBY_MODULE(BuildInfo    TRUE)
SWIG_BUILD_RUBY_MODULE(Calculations TRUE)
SWIG_BUILD_RUBY_MODULE(Database     TRUE)
SWIG_BUILD_RUBY_MODULE(GUI          TRUE)
SWIG_BUILD_RUBY_MODULE(Paths        TRUE)
SWIG_BUILD_RUBY_MODULE(STL          TRUE)

# Oh, SWIG. We need to build this for other SWIG modules to treat C++ enum
# parameters properly, but we don't need to install it.
SWIG_BUILD_RUBY_MODULE_BUILD_ONLY(Enums TRUE)

# Generate .rb files with enums.
SET(ruby_enums_deps
    ${PKMN_SOURCE_DIR}/include/pkmn/enums/CMakeLists.txt
    ${PKMN_SOURCE_DIR}/scripts/generate_swig_enums.py
)
FOREACH(enum ${ENUM_LIST})
    LIST(APPEND ruby_enums
        "${CMAKE_CURRENT_BINARY_DIR}/${enum}.rb"
    )
ENDFOREACH()

ADD_CUSTOM_COMMAND(
    DEPENDS ${ruby_enums_deps}
    OUTPUT ${ruby_enums}
    COMMAND ${PYTHON_EXECUTABLE} ${PKMN_SOURCE_DIR}/scripts/generate_swig_enums.py --ruby --enum-dir=${PKMN_SOURCE_DIR}/include/pkmn/enums --output-dir=${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating Ruby enums"
)
ADD_CUSTOM_TARGET(
    pkmn_ruby_enums ALL
    DEPENDS ${ruby_enums}
)

INSTALL(
    FILES ${ruby_enums}
    DESTINATION ${RUBY_INSTALL_DIR}/PKMN
    COMPONENT Ruby
)

#
# The PKMN module's SWIG file needs to have a different filename
# because pkmn.i is already in use, and Windows uses a case-insensitive
# filesystem.
#
SET_TARGET_PROPERTIES(${SWIG_MODULE_PKMNRuby_REAL_NAME} PROPERTIES OUTPUT_NAME "PKMN")

IF(PKMN_ENABLE_RDOC)
    SET(RDOC_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/ruby")
    ADD_CUSTOM_COMMAND(
        OUTPUT ${RDOC_OUTPUT_DIR}/index.html
        COMMAND ${RDOC_EXECUTABLE} -O -o ${RDOC_OUTPUT_DIR} ../PKMN.rb *.cxx
        COMMENT "Generating Ruby RDoc documentation"
    )
    ADD_CUSTOM_TARGET(ruby_rdoc_docs ALL DEPENDS ${RDOC_OUTPUT_DIR}/index.html)
    INSTALL(DIRECTORY ${RDOC_OUTPUT_DIR} DESTINATION ${PKMN_DOCS_DIR} COMPONENT Documentation)

    ADD_DEPENDENCIES(ruby_rdoc_docs
        ${SWIG_MODULE_PKMNRuby_REAL_NAME}
        ${SWIG_MODULE_Breeding_REAL_NAME}
        ${SWIG_MODULE_BuildInfo_REAL_NAME}
        ${SWIG_MODULE_Calculations_REAL_NAME}
        ${SWIG_MODULE_Database_REAL_NAME}
        ${SWIG_MODULE_GUI_REAL_NAME}
        ${SWIG_MODULE_Paths_REAL_NAME}
        ${SWIG_MODULE_STL_REAL_NAME}
    )
ENDIF()
