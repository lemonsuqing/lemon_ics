#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <NDL.h>

int main(){
    NDL_Init(0);
    uint32_t sec = 0;
    uint32_t usec = 0;
    while(1) {
    while(usec/100000 < sec){
      usec = NDL_GetTicks();
      printf("usec is %d\n",usec);
    }
    usec = NDL_GetTicks();
    printf("some setence~~\n----------------\n");
    sec += 5;
  }
    return 0;
}

/*
int main() {
    struct timeval tv;
    struct timezone tz;
    time_t last_half_second_time = 0;

    while (1) {
        gettimeofday(&tv, &tz);
        double half_second_interval = (double)(tv.tv_sec - last_half_second_time);
        if (half_second_interval >= 0.5) {
            printf("Now Time: %lds, %ldus\n", tv.tv_sec, tv.tv_usec);
            printf("0.5 seconds later\n");
            last_half_second_time = tv.tv_sec;
        }
    }

    return 0;
}
*/