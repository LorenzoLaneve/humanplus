// => hpc/utils/numbers.h
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __human_plus_compiler_util_numbers
#define __human_plus_compiler_util_numbers

#include <hpc/runtime/runtime.h>

#include <cstdlib>
#include <string>

namespace hpc {
    namespace util {
        /*!
         \brief Parses the number represented as a Human Plus <tt>integer</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool dec_toi(std::string numbstr, rt::int32_ty &result);
        /*!
         \brief Parses the number represented as a Human Plus <tt>unsigned integer</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool dec_toui(std::string numbstr, rt::uint32_ty &result);
        /*!
         \brief Parses the number represented as a Human Plus <tt>long</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool dec_tol(std::string numbstr, rt::int64_ty &result);
        /*!
         \brief Parses the number represented as a Human Plus <tt>unsigned long</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool dec_toul(std::string numbstr, rt::uint64_ty &result);
        /*!
         \brief Parses the number represented as a Human Plus <tt>float</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool dec_tof(std::string numbstr, rt::fp_single_ty &result);
        /*!
         \brief Parses the number represented as a Human Plus <tt>double</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool dec_tod(std::string numbstr, rt::fp_double_ty &result);
        
        /*!
         \brief Parses the hexadecimal number represented as a Human Plus <tt>integer</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool hex_toi(std::string numbstr, rt::int32_ty &result);
        /*!
         \brief Parses the hexadecimal number represented as a Human Plus <tt>unsigned integer</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool hex_toui(std::string numbstr, rt::uint32_ty &result);
        /*!
         \brief Parses the hexadecimal number represented as a Human Plus <tt>long</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool hex_tol(std::string numbstr, rt::int64_ty &result);
        /*!
         \brief Parses the hexadecimal number represented as a Human Plus <tt>unsigned long</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool hex_toul(std::string numbstr, rt::uint64_ty &result);
        
        /*!
         \brief Parses the binary number represented as a Human Plus <tt>integer</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool bin_toi(std::string numbstr, rt::int32_ty &result);
        /*!
         \brief Parses the binary number represented as a Human Plus <tt>unsigned integer</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool bin_toui(std::string numbstr, rt::uint32_ty &result);
        /*!
         \brief Parses the binary number represented as a Human Plus <tt>long</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool bin_tol(std::string numbstr, rt::int64_ty &result);
        /*!
         \brief Parses the binary number represented as a Human Plus <tt>unsigned long</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool bin_toul(std::string numbstr, rt::uint64_ty &result);
        
        /*!
         \brief Parses the octal number represented as a Human Plus <tt>integer</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool oct_toi(std::string numbstr, rt::int32_ty &result);
        /*!
         \brief Parses the octal number represented as a Human Plus <tt>unsigned integer</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool oct_toui(std::string numbstr, rt::uint32_ty &result);
        /*!
         \brief Parses the octal number represented as a Human Plus <tt>long</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool oct_tol(std::string numbstr, rt::int64_ty &result);
        /*!
         \brief Parses the octal number represented as a Human Plus <tt>unsigned long</tt> in the given string and puts the result in \c result.
         \return \c true if the number was parsed successfully, \c false if an overflow occurred or if the number represented in the string is invalid.
         */
        bool oct_toul(std::string numbstr, rt::uint64_ty &result);
    }
}

#endif
