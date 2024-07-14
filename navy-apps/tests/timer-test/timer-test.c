#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int main() {
    struct timeval tv;
    struct timezone tz;
    time_t last_time = 0;

    while (1) {
        gettimeofday(&tv, &tz);
        double time_interval = (double)(tv.tv_sec - last_time);
        if (time_interval >= 0.5) {
            printf("Now Time: %.1lfs, %ldus\n", (double)tv.tv_sec, tv.tv_usec);
            printf("0.5 seconds later\n");
            last_time = tv.tv_sec;
        }
    }

    return 0;
}
