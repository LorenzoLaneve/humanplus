// => hpc/inc/builtintypes.inc
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#ifndef __builtintype
#error "__builtintype not defined before including builtintypes.inc"
#endif

//__builtintype(ID, SRCID, FORMAT, MAGNITUDE)

__builtintype(Void, "void", Void, 0)
__builtintype(Boolean, "boolean", Boolean, 0)
__builtintype(String, "string", String, 0)

__builtintype(Character, "char", UnsignedInteger, 1)
//__builtintype(WideCharacter, "wchar", UnsignedInteger, 2)
__builtintype(UTF16Character, "char16", UnsignedInteger, 2)
__builtintype(UTF32Character, "char32", UnsignedInteger, 4)

// Integral types
__builtintype(SignedByte, "signed byte", SignedInteger, 1)
__builtintype(UnsignedByte, "byte", UnsignedInteger, 1)
__builtintype(SignedShort, "short", SignedInteger, 2)
__builtintype(UnsignedShort, "unsigned short", UnsignedInteger, 2)
__builtintype(SignedInteger, "int", SignedInteger, 4)
__builtintype(UnsignedInteger, "unsigned int", UnsignedInteger, 4)
__builtintype(SignedLong, "long", SignedInteger, 8)
__builtintype(UnsignedLong, "unsigned long", UnsignedInteger, 8)
__builtintype(SignedLongLong, "long long", SignedInteger, 16)
__builtintype(UnsignedLongLong, "unsigned long long", UnsignedInteger, 16)

// Floating point types
//__builtintype(Half, "half", FloatingPoint, 16)
__builtintype(Float, "float", FloatingPoint, 32)
__builtintype(Double, "double", FloatingPoint, 64)
