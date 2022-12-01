#include <sys/time.h>

#define MiBinB 1048576
#define TESTCOUNT 5
double now() {
    struct timeval tv;
    gettimeofday(&tv, 0);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}