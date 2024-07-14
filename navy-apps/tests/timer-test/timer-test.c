#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int main() {
    struct timeval tv;
    struct timezone tz;
    time_t last_time = 0;

    while (1) {
        gettimeofday(&tv, &tz);
        if (tv.tv_sec >= last_time + 5) {
            printf("当前时间: %ld秒, %ld微秒\n", tv.tv_sec, tv.tv_usec);
            printf("距离上一次获取的间隔为5秒\n");
            last_time = tv.tv_sec;
        }
    }

    return 0;
}
