//
//  main.c
//  HumanPlus
//
//  Created by Lore on 23/01/2017.
//  Copyright © 2017 Lore. All rights reserved.
//


//
// TO DO list (hpc-1.0):
// 1  - all the options
// 2  - functional statements (currently BETA)
// 3  - runtime and standard namespace implementation
// 4  - using directive
// 5  - preprocessor bufferizer: #define, #include, #if, #ifdef, and #ifndef statements
//     (Note: #include is supposed to be used as file includer and should not be used in Human Plus as it is usually used in C/C++/Objective-C)
// 6  - byte type and division of boolean type format
// 7  - cascade assignments (!)
// 8  - switch-case, select-case (!)
// 9  - safe and unsafe pointers
// 10 - default parameter values
// 11 - object oriented programming paradigms
// 12 - classes, extensions, protocols
// 13 - class methods and ctors/dtors
// 14 - subclassing, overriding and late binding
// 15 - member access levels (public, private, protected, internal) for classes, protocols and namespaces
// 16 - interfile compilation: compilation units and kits
// 17 - canonicalization passes
// 18 - reflect methods
// 19 - attribs in type aliases + type printing
// 20 - new type system
//
//
// 21 - Various warnings
// 22 - Various notes following some errors
// 23 - Type parsing bugs and code design fixes
// 24 - Number literals code design fixes + exponential literals
// 25 - Copyright and license above each source file
//

int hpc_main(const char **argBegin, const char **argEnd, const char *argv0, void *main);

int main(int argc, const char *argv[]) {
    return hpc_main(argv+1, argv+argc, *argv, (void *)main);
}


