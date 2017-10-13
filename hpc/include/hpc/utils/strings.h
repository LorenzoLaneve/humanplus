// => hpc/utils/strings.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_util
#define __human_plus_compiler_util

#include <string>
#include <vector>

#define __PRETTY_CLASS__ util::extractClassName(__PRETTY_FUNCTION__)

namespace hpc {
    namespace util {
        
        /*!
         \brief Splits the string \c s into substrings that were divided in the string by \c delim.
         \param truncempty If \c true, empty substrings will not be added to the returning vector.
         \return A \c std::vector containing the substrings.
         */
        std::vector<std::string> split(const std::string &s, char delim, bool truncempty = true);
        
        /*!
         \brief Replaces all the occurrences of \c from in the string \c str into \c to.
         \return A copy of the string with the replacements.
         */
        std::string replace(std::string str, std::string from, std::string to);
        
        /*!
         \brief Returns a std::string containing the class name, extracted from the \c __PRETTY_FUNCTION__ passed as argument.
         */
        std::string extractClassName(std::string pf);
        
        /*!
         \brief \c true if \c str starts with \c begin, \c false otherwise.
         */
        bool startsWith(std::string str, std::string begin);
        
        /*!
         \brief \c true if the string is empty or has only whitespaces, \c false otherwise.
         */
        bool isEmptyOrBlank(std::string str);
        
        /*!
         \brief Returns a copy of \c str encoded for use in a bourne-again shell.
         */
        std::string bashEncode(std::string str);
        
        /*!
         \brief Returns a string containing \c reasons with the formats \c %n (with \c n a number in \c [0, 9]) with the string at index \c n in the \c params vector.
         \warning The parameters replaced cannot exceed 9 parameters, because the function just parses a single-digit number. E.g.: If there is a \c %10 format, only the \c %1 will be replaced.
         */
        std::string replaceParams(std::string reason, std::vector<std::string> params);

    }
}

#endif
