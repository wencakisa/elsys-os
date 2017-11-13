#include <string.h>
#include <unistd.h>

int main() {
    const char *s = "Hello, World!\n";
    size_t len = strlen(s);
    size_t written = 0;
    ssize_t wresult = 0;

    while (written != len) {
        wresult = write(STDOUT_FILENO, s + written, len - written);

        if (wresult < 0) {
            return -1;
        }

        written += wresult;
    }

    return 0;
}
