/*
 * Copyright (c) 2017 Nicholas Corgan (n.corgan@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#ifndef PKMNTEST_CONFIG_HPP
#define PKMNTEST_CONFIG_HPP

/***************************************************************************
 * Allow use of and/or/not
 ***************************************************************************/
#include <ciso646>

/***************************************************************************
 * Define cross-platform macros
 ***************************************************************************/
#if defined(_MSC_VER)
#    define PKMNTEST_EXPORT         __declspec(dllexport)
#    define PKMNTEST_IMPORT         __declspec(dllimport)
#    define PKMNTEST_INLINE         __forceinline
#    define PKMNTEST_UNUSED(x)      x
#    pragma warning(disable: 4251) // class 'A<T>' needs to have dll-interface to be used by clients of class 'B'
#    pragma warning(disable: 4800) // forcing value to bool 'true' or 'false'
#elif defined(__GNUG__) && __GNUG__ >= 4
#    define PKMNTEST_EXPORT         __attribute__((visibility("default")))
#    define PKMNTEST_IMPORT         __attribute__((visibility("default")))
#    define PKMNTEST_INLINE         inline __attribute__((always_inline))
#    define PKMNTEST_UNUSED(x)      x __attribute__((unused))
#else
#    define PKMNTEST_EXPORT
#    define PKMNTEST_IMPORT
#    define PKMNTEST_INLINE         inline
#    define PKMNTEST_UNUSED(x)      x
#endif

#ifdef PKMNTEST_DLL_EXPORTS
#    define PKMNTEST_API PKMNTEST_EXPORT
#else
#    define PKMNTEST_API PKMNTEST_IMPORT
#endif

#if defined(linux) || defined(__linux) || defined(__linux__)
#    define PKMNTEST_PLATFORM_LINUX
#elif defined(__MINGW32__) || defined(MINGW32)
#    define PKMNTEST_PLATFORM_MINGW
#elif defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#    define PKMNTEST_PLATFORM_WIN32
#elif defined(macintosh) || defined(__APPLE__) || defined(__APPLE_CC__)
#    define PKMNTEST_PLATFORM_MACOS
#endif

#endif /* PKMNTEST_CONFIG_HPP */
