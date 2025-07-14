/////////////////////////////////////////////////////////////////////////////////
// Repository of the library: https://github.com/owos02/c-visual-assert
/////////////////////////////////////////////////////////////////////////////////
//
// MIT License
//
// Copyright (c) 2025 Oliver Woschko
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
/////////////////////////////////////////////////////////////////////////////////

#pragma once
#include <cassert>
#include <iostream>
#include <utility>
#include <iomanip>
#include <vector>
#include <format>

namespace c_visual_assert_ {
    inline static std::vector< std::pair< bool, const char * > > calledAsserts_;

    inline static int assertionCounter = 0;

    namespace Settings {
        /// Disables the visual display of asserts for the future asserts.
        inline volatile static bool quiet = false;
        /// Collects all statements into a vector of pairs
        /// which can be listed in a concise print.
        /// See macro `c_visual_assert_list_all` for more information.
        inline volatile static bool collectionMode = false;
        /// Enables borders for the conditions when displaying the list of asserts.
        inline volatile static bool listAssertMessageBorder = false;

        inline volatile static int  leftOffset   = 80;
        inline volatile static int  rightOffset  = 40;
        inline volatile static char fillerSymbol = '.';
    }

    /// Define Colors based on the ANSI escape codes
    namespace ANSI_Colors {
        inline static auto FAILURE   = "91";
        inline static auto SUCCESS   = "92";
        inline static auto ASSERTING = "93";
        inline static auto INFO      = "94";
    }

    /// Define messages for the output
    namespace Message {
        inline auto FAILURE   = "FAILURE";
        inline auto SUCCESS   = "OK";
        inline auto ASSERTING = "ASSERTING";
    }

#define c_visual_assert_ansi(condition, message)                                                                                \
    assertionCounter++;                                                                                                         \
    if(!Settings::quiet) {                                                                                                      \
        auto displayMessage = std::format("\033[1;37m[{1}] \033[1;{0}m{2}:\033[0m\033[3m {3}\033[0m",                           \
                                ANSI_Colors::ASSERTING, assertionCounter, Message::ASSERTING, message);                         \
        std::cout << std::left << std::setw(Settings::leftOffset) << std::setfill(Settings::fillerSymbol) << displayMessage ;   \
    }                                                                                                                           \
    if(!condition && !Settings::quiet) {                                                                                        \
        auto displayFailure = std::format("\033[1;52;{0}m {1} \033[0m\n", ANSI_Colors::FAILURE, Message::FAILURE );             \
        std::cout << std::right << std::setw(Settings::rightOffset) << std::setfill(Settings::fillerSymbol) << displayFailure;  \
    }                                                                                                                           \
    else if(!Settings::quiet)  {                                                                                                \
        auto displaySuccess = std::format("\033[1;52;{0}m {1} \033[0m\n", ANSI_Colors::SUCCESS, Message::SUCCESS );             \
        std::cout << std::right << std::setfill(Settings::fillerSymbol) << std::setw(Settings::rightOffset) << displaySuccess;  \
    }                                                                                                                           \
    assert( message && condition);

#define c_assert_windows(condition, message) c_visual_assert_ansi( condition, message  )

    inline void c_visual_assert_selector( const bool condition, const char *message ) {
#if defined(c_visual_assert_enable_debug) || defined(DEBUG)
        if ( Settings::collectionMode )
            calledAsserts_.emplace_back( condition, message );
#ifdef _WIN32
        c_assert_windows(condition, message);
#elif __unix__
        c_visual_assert_ansi( condition, message );
#endif
#endif
    }

    inline bool check_collection_mode_is_disabled( ) {
        if ( !Settings::collectionMode ) {
            auto msg = std::format( "\33[1;{0}mCollection Mode\33[0m: \33[1;4;{1}mDisabled\33[0m\n", ANSI_Colors::INFO, ANSI_Colors::FAILURE );
            std::cout << msg;
        }
        return ( !Settings::collectionMode );
    }
#define c_visual_headline_print(message) \
std::cout << std::format("\n\33[1;4m{0}\33[0m\n", message);
    /// Shows stats about the successful and failed asserts
    /// For proper use define 'NDEBUG'
    inline void c_visual_assert_list_stats( ) {
#if defined(c_visual_assert_enable_debug) || defined(DEBUG)
        if ( check_collection_mode_is_disabled( ) )
            return;

        int    successes           = 0, failures           = 0;
        double successesPercentage = 0, failuresPercentage = 0;
        for ( auto called_assert : calledAsserts_ ) {
            if ( called_assert.first == true )
                successes++;
            else
                failures++;
        }
        successesPercentage = successes == 0
                                  ? 0
                                  : ( successes * 100 ) / static_cast< double >( calledAsserts_.size( ) );
        failuresPercentage = failures == 0
                                 ? 0
                                 : ( ( failures * 100 ) / static_cast< double >( calledAsserts_.size( ) ) );
        c_visual_headline_print("Statistics about the assertions");
        std::cout  << std::setfill(' ')<< std::left << std::setw(25) << std::format("Amount of assertions:")
        << std::right << std::setw(5) << std::format("\033[1;{0}m{1}\033[0m\n", ANSI_Colors::ASSERTING ,  calledAsserts_.size( ) );
        std::cout << std::left <<std::setw(25) << std::format("Successful assertions:")  <<
            std::format("\033[{0}m{1}\033[0m\n", ANSI_Colors::SUCCESS, successes);
        std::cout << std::left <<std::setw(25) << std::format("Failed assertions:")  <<
            std::format("\033[{0}m{1}\033[0m\n",ANSI_Colors::FAILURE, failures);
        std::cout << std::left << std::setw(16) << std::format("\nPercentages")  <<
            std::format("( \033[1;{}mSuccess\033[0m | \033[1;{}mFailure\033[0m )\n",ANSI_Colors::SUCCESS, ANSI_Colors::FAILURE );
        std::cout << std::fixed << std::setprecision( 2 ) << std::right <<std::setw(35) <<
            std::format("\033[4;{0}m{1}\033[0m%", ANSI_Colors::SUCCESS, successesPercentage) << " | " <<
            std::format("\033[4;{0}m{1}\033[0m%\n", ANSI_Colors::FAILURE, failuresPercentage);
#endif
    }

    inline void c_visual_assert_list_all( ) {
#if defined(c_visual_assert_enable_debug) || defined(DEBUG)
        if ( check_collection_mode_is_disabled( ) )
            return;
        c_visual_headline_print("List of Assertions");
        for ( auto [ condition, message ] : calledAsserts_ ) {
            std::cout << std::left << std::setw(60) <<std::setfill(Settings::fillerSymbol) <<
                std::format("\033[{};3m {} \033[0m", ( Settings::listAssertMessageBorder ? "52" : "0" ),message );
            if ( condition )
                std::cout << std::format("was \033[1;{}mTrue\033[0m\n", ANSI_Colors::SUCCESS);
            else
                std::cout << std::format("was \033[1;{}mFalse\033[0m\n", ANSI_Colors::FAILURE);
        }
#endif
    }
}

#define c_visual_assert(condition, message) \
    c_visual_assert_::c_visual_assert_selector( condition, message );

#define c_visual_assert_quiet(condition, message) \
    c_visual_assert_::Settings::quiet = true; \
    c_visual_assert(condition, message); \
    c_visual_assert_::Settings::quiet = false;

#define c_visual_assert_list_all c_visual_assert_::c_visual_assert_list_all();

#define c_visual_assert_list_stats c_visual_assert_::c_visual_assert_list_stats();

