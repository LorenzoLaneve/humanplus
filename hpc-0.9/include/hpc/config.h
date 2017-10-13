//
//  config.h
//  HumanPlus
//
//  Created by Lore on 12/06/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#ifndef __human_plus_compiler_configurations
#define __human_plus_compiler_configurations

#define __human_plus_compiler_supports_bool_context_conversion ///< Whether hpc should convert primitive types to boolean implicitly, in a C-like manner.

#define __human_plus_compiler_when_integer_is_null !val ///< Condition which indicates whether an integer literal can be used as null pointer.

#define __human_plus_compiler_signature "Human Plus Compiler version 0.9 (hpc-0.9)" ///< String that will be put in \c !llvm.ident .

#define __hpc_debugging

#define __hpc_fe_ast_debug
#define __hpc_fe_ir_debug

#if defined(__APPLE__) && defined(__MACH__)
#define __human_plus_compiler_host_apple

#elif defined(__linux__)
#define __human_plus_compiler_host_linux

#elif defined(__sun) && defined(__SVR4)
#define __human_plus_compiler_host_solaris

#elif defined(__CYGWIN__)
#define __human_plus_compiler_host_cygwin

#elif defined(_WIN64)
#define __human_plus_compiler_host_win64

#elif defined(_WIN32)
#define __human_plus_compiler_host_win32

#endif


#endif
