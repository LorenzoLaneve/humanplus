// => hpc/inc/syntax/escapes.inc
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __escape_seq
#error "__escape_seq not defined before including escapes.inc"
#endif

//__escape_seq(toRead, value)

__escape_seq('\'', 0x27)
__escape_seq('\"', 0x22)
__escape_seq('\?', 0x3f)
__escape_seq('\\', 0x5c)
__escape_seq('a', 0x07)
__escape_seq('b', 0x08)
__escape_seq('f', 0x0c)
__escape_seq('n', 0x0a)
__escape_seq('r', 0x0d)
__escape_seq('t', 0x09)
__escape_seq('v', 0x0b)
