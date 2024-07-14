#include <stdio.h>
#include <sys/time.h>
#include <time.h>

int main() {
    struct timeval tv;
    struct timezone tz;
    time_t last_time = 0;
    int flag = 1;

    while (1) {
        gettimeofday(&tv, &tz);
        if(flag == 1){
            flag = 0;
            printf("Now Time: %lds, %ldus\n...\n", tv.tv_sec, tv.tv_usec);
        }
        if (tv.tv_sec >= last_time + 5) {
            printf("Now Time: %lds, %ldus\n", tv.tv_sec, tv.tv_usec);
            printf("5 seconds later\n");
            last_time = tv.tv_sec;
        }
    }

    return 0;
}
