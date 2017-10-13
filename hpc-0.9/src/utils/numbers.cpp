//
//  numbers.cpp
//  HumanPlus
//
//  Created by Lore on 27/05/16.
//  Copyright © 2016 Lore. All rights reserved.
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

bool util::dec_toi(std::string numbstr, runtime::int32_ty &result) {
    errno = 0;
    result = strtoi(numbstr.c_str(), nullptr, 10);
    return !errno;
}

bool util::dec_toui(std::string numbstr, runtime::uint32_ty &result) {
    errno = 0;
    result = strtoui(numbstr.c_str(), nullptr, 10);
    return !errno;
}

bool util::dec_tol(std::string numbstr, runtime::int64_ty &result) {
    errno = 0;
    result = strtol(numbstr.c_str(), nullptr, 10);
    return !errno;
}

bool util::dec_toul(std::string numbstr, runtime::uint64_ty &result) {
    errno = 0;
    result = strtoul(numbstr.c_str(), nullptr, 10);
    return !errno;
}

bool util::dec_tof(std::string numbstr, runtime::fp_single_ty &result) {
    errno = 0;
    result = strtof(numbstr.c_str(), nullptr);
    return !errno;
}

bool util::dec_tod(std::string numbstr, runtime::fp_double_ty &result) {
    errno = 0;
    result = strtod(numbstr.c_str(), nullptr);
    return !errno;
}



bool util::hex_toi(std::string numbstr, runtime::int32_ty &result) {
    errno = 0;
    result = strtoi(numbstr.c_str(), nullptr, 16);
    return !errno;
}

bool util::hex_toui(std::string numbstr, runtime::uint32_ty &result) {
    errno = 0;
    result = strtoui(numbstr.c_str(), nullptr, 16);
    return !errno;
}

bool util::hex_tol(std::string numbstr, runtime::int64_ty &result) {
    errno = 0;
    result = strtol(numbstr.c_str(), nullptr, 16);
    return !errno;
}

bool util::hex_toul(std::string numbstr, runtime::uint64_ty &result) {
    errno = 0;
    result = strtoul(numbstr.c_str(), nullptr, 16);
    return !errno;
}



bool util::bin_toi(std::string numbstr, runtime::int32_ty &result) {
    errno = 0;
    result = strtoi(numbstr.c_str(), nullptr, 2);
    return !errno;
}

bool util::bin_toui(std::string numbstr, runtime::uint32_ty &result) {
    errno = 0;
    result = strtoui(numbstr.c_str(), nullptr, 2);
    return !errno;
}

bool util::bin_tol(std::string numbstr, runtime::int64_ty &result) {
    errno = 0;
    result = strtol(numbstr.c_str(), nullptr, 2);
    return !errno;
}

bool util::bin_toul(std::string numbstr, runtime::uint64_ty &result) {
    errno = 0;
    result = strtoul(numbstr.c_str(), nullptr, 2);
    return !errno;
}



bool util::oct_toi(std::string numbstr, runtime::int32_ty &result) {
    errno = 0;
    result = strtoi(numbstr.c_str(), nullptr, 8);
    return !errno;
}

bool util::oct_toui(std::string numbstr, runtime::uint32_ty &result) {
    errno = 0;
    result = strtoui(numbstr.c_str(), nullptr, 8);
    return !errno;
}

bool util::oct_tol(std::string numbstr, runtime::int64_ty &result) {
    errno = 0;
    result = strtol(numbstr.c_str(), nullptr, 8);
    return !errno;
}

bool util::oct_toul(std::string numbstr, runtime::uint64_ty &result) {
    errno = 0;
    result = strtoul(numbstr.c_str(), nullptr, 8);
    return !errno;
}

