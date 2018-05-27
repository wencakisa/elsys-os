#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>

#include "helpers.h"
#include "ls.h"

static int cmpstringp(const void *p1, const void *p2) {
    return strcoll(* (char * const *) p1, * (char * const *) p2);
}

int main(int argc, char const *argv[]) {
    setlocale(LC_ALL, "");

    const char* filenames[argc - 1];

    for (size_t i = 0; i < argc - 1; i++) {
        filenames[i] = argv[i + 1];
    }

    // qsort(filenames, argc - 1, sizeof(*filenames), cmpstringp);

    ls(argc - 1, filenames);

    return 0;
}
