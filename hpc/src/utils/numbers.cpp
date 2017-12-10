// => src/utils/numbers.cpp
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <hpc/utils/numbers.h>

#include <cerrno>
#include <climits>
#include <cstdlib>

using namespace hpc;

static int strtoi(const char *str, char **endptr, int base) {
#if INT_MAX == LONG_MAX && INT_MIN == LONG_MIN
    return (int) strtol(str, nullptr, base);
#else
    long x = strtol(str, nullptr, base);
    
    if (!errno) {
        if (x > INT_MAX) {
            errno = ERANGE;
            return INT_MAX;
        }
        
        if (x < INT_MIN) {
            errno = ERANGE;
            return INT_MIN;
        }
    }
    
    return (int) x;
#endif
}

static unsigned int strtoui(const char *str, char **endptr, int base) {
#if INT_MAX == LONG_MAX && INT_MIN == LONG_MIN
    return (int) strtoul(str, nullptr, base);
#else
    unsigned long x = strtoul(str, nullptr, base);
    
    if (!errno) {
        if (x > UINT_MAX) {
            errno = ERANGE;
            return UINT_MAX;
        }
    }
    
    return (unsigned int) x;
#endif
}

bool util::dec_toi(std::string numbstr, rt::int32_ty &result) {
    errno = 0;
    result = strtoi(numbstr.c_str(), nullptr, 10);
    return !errno;
}

bool util::dec_toui(std::string numbstr, rt::uint32_ty &result) {
    errno = 0;
    result = strtoui(numbstr.c_str(), nullptr, 10);
    return !errno;
}

bool util::dec_tol(std::string numbstr, rt::int64_ty &result) {
    errno = 0;
    result = strtol(numbstr.c_str(), nullptr, 10);
    return !errno;
}

bool util::dec_toul(std::string numbstr, rt::uint64_ty &result) {
    errno = 0;
    result = strtoul(numbstr.c_str(), nullptr, 10);
    return !errno;
}

bool util::dec_tof(std::string numbstr, rt::fp_single_ty &result) {
    errno = 0;
    result = strtof(numbstr.c_str(), nullptr);
    return !errno;
}

bool util::dec_tod(std::string numbstr, rt::fp_double_ty &result) {
    errno = 0;
    result = strtod(numbstr.c_str(), nullptr);
    return !errno;
}



bool util::hex_toi(std::string numbstr, rt::int32_ty &result) {
    errno = 0;
    result = strtoi(numbstr.c_str(), nullptr, 16);
    return !errno;
}

bool util::hex_toui(std::string numbstr, rt::uint32_ty &result) {
    errno = 0;
    result = strtoui(numbstr.c_str(), nullptr, 16);
    return !errno;
}

bool util::hex_tol(std::string numbstr, rt::int64_ty &result) {
    errno = 0;
    result = strtol(numbstr.c_str(), nullptr, 16);
    return !errno;
}

bool util::hex_toul(std::string numbstr, rt::uint64_ty &result) {
    errno = 0;
    result = strtoul(numbstr.c_str(), nullptr, 16);
    return !errno;
}



bool util::bin_toi(std::string numbstr, rt::int32_ty &result) {
    errno = 0;
    result = strtoi(numbstr.c_str(), nullptr, 2);
    return !errno;
}

bool util::bin_toui(std::string numbstr, rt::uint32_ty &result) {
    errno = 0;
    result = strtoui(numbstr.c_str(), nullptr, 2);
    return !errno;
}

bool util::bin_tol(std::string numbstr, rt::int64_ty &result) {
    errno = 0;
    result = strtol(numbstr.c_str(), nullptr, 2);
    return !errno;
}

bool util::bin_toul(std::string numbstr, rt::uint64_ty &result) {
    errno = 0;
    result = strtoul(numbstr.c_str(), nullptr, 2);
    return !errno;
}



bool util::oct_toi(std::string numbstr, rt::int32_ty &result) {
    errno = 0;
    result = strtoi(numbstr.c_str(), nullptr, 8);
    return !errno;
}

bool util::oct_toui(std::string numbstr, rt::uint32_ty &result) {
    errno = 0;
    result = strtoui(numbstr.c_str(), nullptr, 8);
    return !errno;
}

bool util::oct_tol(std::string numbstr, rt::int64_ty &result) {
    errno = 0;
    result = strtol(numbstr.c_str(), nullptr, 8);
    return !errno;
}

bool util::oct_toul(std::string numbstr, rt::uint64_ty &result) {
    errno = 0;
    result = strtoul(numbstr.c_str(), nullptr, 8);
    return !errno;
}

