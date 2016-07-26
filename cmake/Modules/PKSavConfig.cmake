#
# Copyright (c) 2016 Nicholas Corgan (n.corgan@gmail.com)
#
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)
#

#
# The Boost.Predef check allows us to specify PKSav's endianness.
#
IF(PLATFORM_LITTLE_ENDIAN)
    SET(PKSAV_LITTLE_ENDIAN ON CACHE BOOL "PKSav: little endian")
    SET(PKSAV_BIG_ENDIAN OFF CACHE BOOL "PKSav: big endian")
ELSE()
    SET(PKSAV_LITTLE_ENDIAN OFF CACHE BOOL "PKSav: little endian")
    SET(PKSAV_BIG_ENDIAN ON CACHE BOOL "PKSav: big endian")
ENDIF(PLATFORM_LITTLE_ENDIAN)

#
# Set PKSav components
#
SET(PKSAV_ENABLE_GEN1 ON  CACHE BOOL "PKSav: build Generation I component")
SET(PKSAV_ENABLE_GEN2 ON  CACHE BOOL "PKSav: build Generation II component")
SET(PKSAV_ENABLE_GBA  OFF CACHE BOOL "PKSav: build Game Boy Advance component")
SET(PKSAV_ENABLE_GCN  OFF CACHE BOOL "PKSav: build Gamecube component")
SET(PKSAV_ENABLE_GEN4 OFF CACHE BOOL "PKSav: build Generation IV component")
SET(PKSAV_ENABLE_GEN5 OFF CACHE BOOL "PKSav: build Generation V component")
SET(PKSAV_ENABLE_GEN6 OFF CACHE BOOL "PKSav: build Generation VI component")