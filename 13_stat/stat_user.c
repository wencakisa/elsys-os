#include <stdio.h>
#include <sys/stat.h>
#include <pwd.h>

int main(int argc, char const *argv[]) {
    struct stat fileStat;

    for (size_t i = 1; i < argc; i++) {
        stat(argv[i], &fileStat);

        printf("%s %s\n", argv[i], getpwuid(fileStat.st_uid)->pw_name);
    }

    return 0;
}
