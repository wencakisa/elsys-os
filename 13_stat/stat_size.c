#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char const *argv[]) {
    struct stat fileStat;

    for (size_t i = 1; i < argc; i++) {
        stat(argv[i], &fileStat);

        printf("%s %ld\n", argv[i], fileStat.st_size);
    }

    return 0;
}
