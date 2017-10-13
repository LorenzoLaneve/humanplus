//
//  io.cpp
//  HumanPlus
//
//  Created by Lore on 30/03/16.
//  Copyright © 2016 Lore. All rights reserved.
//

#include <stdio.h>

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
