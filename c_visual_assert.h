/* c-visual-assert
*   You can find the license of this library at at:
*       https://github.com/owos02/c-visual-assert/blob/main/LICENSE
*
*   A lightweight visualized assert library with comments.
*   For documentation see:
*       https://github.com/owos02/c-visual-assert/blob/main/README.md
*
**/

#pragma once
#include <cassert>
#include <iostream>
#include <utility>
#include <iomanip>
#include <vector>

namespace c_visual_assert_ {
    inline static std::vector< std::pair< bool, const char * > > calledAsserts_;

    namespace Settings {
        /// Disables the visual display of asserts for the future asserts.
        inline volatile static bool quiet = false;
        /// Collects all statements into a vector of pairs
        /// which can be listed in a concise print.
        /// See macro `c_visual_assert_list_all` for more information.
        inline volatile static bool collectionMode = false;
        /// Enables borders for the conditions when displaying the list of asserts.
        inline volatile static bool listAssertMessageBorder = false;
    }

    /// Define Colors based on the ANSI escape codes
    namespace ANSI_Colors {
        inline static auto FAILURE   = "91";
        inline static auto SUCCESS   = "92";
        inline static auto ASSERTING = "93";
    }

    /// Define messages for the output
    namespace Message {
        inline auto FAILURE   = "FAILURE";
        inline auto SUCCESS   = "OK";
        inline auto ASSERTING = "ASSERTING";
    }

#define c_visual_assert_ansi(condition, message) \
    if(!Settings::quiet)\
        std::cout << "\033[1;" << ANSI_Colors::ASSERTING << "m "<<Message::ASSERTING<<": \033[0m" << "\033[3m"<< message << "\033[0m";\
    if(!condition && !Settings::quiet)\
        std::cout << "\t-- \033[1;52;" << ANSI_Colors::FAILURE << "m "<<Message::FAILURE<<" \033[0m" << std::endl;\
    else if(!Settings::quiet)\
        std::cout << "\t--\t \033[1;52;" << ANSI_Colors::SUCCESS << "m "<<Message::SUCCESS<<" \033[0m" << std::endl; \
    assert( message && condition);

#define c_assert_windows(condition, message) c_visual_assert_ansi( condition, message  )

    inline void c_visual_assert_selector( const bool condition, const char *message ) {
#if defined(c_visual_assert_enable_debug) || defined(DEBUG)
        if ( Settings::collectionMode )
            calledAsserts_.emplace_back( condition, message );
#ifdef _WIN32
        //c_assert_windows(condition, message);
#elif __unix__
        c_visual_assert_ansi( condition, message );
#endif
#endif
    }

    inline bool check_collection_mode_is_disabled() {
        if (!Settings::collectionMode)
            std::cout << "Collection Mode: \33[1;4;" << ANSI_Colors::FAILURE << "mDisabled\33[0m" <<  std::endl;
        return (!Settings::collectionMode);
    }

    /// Shows stats about the successful and failed asserts
    /// For proper use define 'NDEBUG'
    inline void c_visual_assert_list_stats( ) {
#if defined(c_visual_assert_enable_debug) || defined(DEBUG)
        if (check_collection_mode_is_disabled())
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
        failuresPercentage  = failures == 0
                                  ? 0
                                  : ( ( failures * 100 ) / static_cast< double >( calledAsserts_.size( ) ) );
        std::cout << std::endl << "\033[1;4mStatistics about the assertions\033[0m" << std::endl;
        std::cout << std::endl << "Amount of assertions: \033[1;4;"<<ANSI_Colors::ASSERTING<< "m" << calledAsserts_.size( ) << "\033[0m"<< std::endl;
        std::cout << "Percentages ( \033[1;4;" << ANSI_Colors::SUCCESS << "mSuccess\033[0m | \033[1;4;" <<
                ANSI_Colors::FAILURE << "mFailure\033[0m )" << std::endl << std::setw(19)
                << "\033[4;" << ANSI_Colors::SUCCESS << "m" << std::fixed << std::setprecision(2) << successesPercentage << "\033[0m%" << " | "
                << "\033[4;" << ANSI_Colors::FAILURE << "m" << failuresPercentage << "\033[0m%" << std::endl;
        std::cout << "Successful assertions: \033[" << ANSI_Colors::SUCCESS << "m" << successes << "\033[0m" <<
                std::endl;
        std::cout << "Failed assertions: \033[" << ANSI_Colors::FAILURE << "m" << failures << "\033[0m" << std::endl;
#endif
    }

    inline void c_visual_assert_list_all( ) {
#if defined(c_visual_assert_enable_debug) || defined(DEBUG)
        if (check_collection_mode_is_disabled())
            return;
        std::cout << std::endl << "\033[1;4mList of Assertions\033[0m" << std::endl;
        for ( auto [ condition, message ] : calledAsserts_ ) {
            std::cout << "\033[" << ( Settings::listAssertMessageBorder ? "52" : "0" ) << ";3m " << message <<
                    " \033[0m";
            if ( condition )
                std::cout << "was \033[1;" << ANSI_Colors::SUCCESS << "mTrue\033[0m" << std::endl;
            else
                std::cout << "was \033[1;" << ANSI_Colors::FAILURE << "mFailure\033[0m" << std::endl;
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
