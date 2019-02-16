#include <errno.h>

int _read(int file, char *ptr, int len) {
    errno = ENOSYS;
    return -1;
}

int _write(int file, char *ptr, int len) {
    errno = ENOSYS;
    return -1;
}
