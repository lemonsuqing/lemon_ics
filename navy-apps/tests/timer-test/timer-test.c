#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int main() {
    struct timeval tv;
    struct timezone tz;
    time_t last_time = 0;
    time_t last_half_second_time = 0;

    while (1) {
        gettimeofday(&tv, &tz);
        double time_interval = (double)(tv.tv_sec - last_time);
        double half_second_interval = (double)(tv.tv_sec - last_half_second_time);
        if (time_interval >= 1.0) {
            printf("Now Time: %lds, %ldus\n", tv.tv_sec, tv.tv_usec);
            last_time = tv.tv_sec;
        }
        if (half_second_interval >= 0.5) {
            printf("0.5 seconds later\n");
            last_half_second_time = tv.tv_sec;
        }
    }

    return 0;
}
