// => src/main.c
//
//                     The Human Plus Project
//
// This file is distributed under the University of Illinois/NCSA
// Open Source License. See LICENSE.TXT for details.
//
//

int hpc_main(const char **argBegin, const char **argEnd, const char *argv0, void *main);

int main(int argc, const char *argv[]) {
    return hpc_main(argv+1, argv+argc, *argv, (void *)main);
}


