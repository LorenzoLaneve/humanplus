//
//  runtime.hpp
//  HumanPlus
//
//  Created by Lore on 15/05/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_runtime
#define __human_plus_compiler_runtime

#include <sys/types.h>
#include <string>

namespace hpc {
    namespace runtime {
       
        typedef void void_ty;               ///< <tt>void</tt> type in the Human Plus Runtime.
        typedef bool boolean_ty;            ///< <tt>boolean</tt> type in the Human Plus Runtime.
        
        typedef char utf7_char_ty;          ///< <tt>char</tt> type in the Human Plus Runtime.
        typedef unsigned char utf8_char_ty; ///< <tt>unsigned char</tt> type in the Human Plus Runtime.
        typedef std::string string_ty;      ///< <tt>string</tt> type in the Human Plus Runtime.
        
        typedef unsigned char int8_ty;      ///< <tt>byte</tt> type in the Human Plus Runtime.
        typedef __int16_t int16_ty;         ///< <tt>short</tt> type in the Human Plus Runtime.
        typedef __uint16_t uint16_ty;       ///< <tt>unsigned short</tt> type in the Human Plus Runtime.
        typedef __int32_t int32_ty;         ///< <tt>integer</tt> type in the Human Plus Runtime.
        typedef __uint32_t uint32_ty;       ///< <tt>unsigned integer</tt> type in the Human Plus Runtime.
        typedef __int64_t int64_ty;         ///< <tt>long</tt> type in the Human Plus Runtime.
        typedef __uint64_t uint64_ty;       ///< <tt>unsigned long</tt> type in the Human Plus Runtime.
        
        typedef float fp_single_ty;         ///< <tt>float</tt> type in the Human Plus Runtime.
        typedef double fp_double_ty;        ///< <tt>double</tt> type in the Human Plus Runtime.
        
        //typedef __int128_t int128_ty;
        //typedef __uint128_t uint128_ty;
        
    }
}

#endif