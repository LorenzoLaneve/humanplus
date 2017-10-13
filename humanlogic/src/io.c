// => src/io.c
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

#include <stdio.h>

//
// Note: this file just provides the standard C functions to Human Plus programs for testing.
// The real runtime library will be developed soon.
//

int _rdc() {
    return fgetc(stdin);
}

int _rdi() {
    int x;
    fscanf(stdin, "%d", &x);
    return x;
}

int _wrtc(char x) {
    return putc(x, stdout);
}

int _wrti(int x) {
    return printf("%d", x);
}

int _wrts(const char *s) {
    return fputs(s, stdout);
}
