#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int main() {
    struct timeval tv;
    struct timezone tz;
    time_t last_half_second_time = 0;

    while (1) {
        gettimeofday(&tv, &tz);
        double half_second_interval = (double)(tv.tv_sec - last_half_second_time);
        if (half_second_interval <= 0.5) {
            printf("Now Time: %lds, %ldus\n", tv.tv_sec, tv.tv_usec);
            printf("0.5 seconds later\n");
            last_half_second_time = tv.tv_sec;
        }
    }

    return 0;
}
