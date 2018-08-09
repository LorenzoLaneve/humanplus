// => hpc/inc/opts.inc
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __opt
#error "__opt not defined before including opts.inc"
#endif

//__opt(PREFIX, ID, TYPE, GROUP, ALIAS, FLAGS, ALIASARGS, HELPTEXT, METAVAR)

__opt("<L group>", L_group, Group, Nothing, Nothing, 0, 0, 0, 0)
__opt("<m group>", m_group, Group, Nothing, Nothing, 0, 0, 0, 0)
__opt("<x86 features group>", target_features, Group, Nothing, Nothing, 0, 0, 0, 0)

__opt("<input>", InputFiles, Input, Nothing, Nothing, 0, 0, 0, 0)
__opt("<unknown>", Unknown, Unknown, Nothing, Nothing, 0, 0, 0, 0)
__opt("--help-hidden", __help_hidden, Flag, Nothing, Nothing, 0, 0, 0, 0)
__opt("--help", __help, Flag, Nothing, Nothing, 0, 0, "Show available options", 0)
__opt("--version", __version, Flag, Nothing, Nothing, 0, 0, 0, 0)
__opt("-c", c, Flag, Nothing, Nothing, 0, 0, "Only compile and assemble", 0)
__opt("-emit-llvm", emit_llvm, Flag, Nothing, Nothing, 0, 0, 0, 0)
__opt("-L", L, JoinedOrSeparate, L_group, Nothing, 0, 0, 0, 0)
__opt("-maes", maes, Flag, target_features, Nothing, 0, 0, 0, 0)
__opt("-mcpu=", target_cpu, Joined, Nothing, Nothing, 0, 0, 0, 0)
__opt("-m", m, JoinedOrSeparate, m_group, Nothing, 0, 0, 0, 0)
__opt("-O", O, Joined, Nothing, Nothing, 0, 0, 0, 0)
__opt("-o", o, JoinedOrSeparate, Nothing, Nothing, 0, 0, 0, 0)
__opt("-S", S, Flag, Nothing, Nothing, 0, 0, 0, 0)
__opt("-target", triple, Separate, Nothing, Nothing, 0, 0, "Generate code for the given target", 0)
__opt("-v", v, Flag, Nothing, Nothing, 0, 0, 0, 0)
