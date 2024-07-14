#include <stdio.h>
#include <sys/time.h>

int main() {
    struct timeval tv;
    int ret;

    // 调用gettimeofday
    ret = gettimeofday(&tv, NULL);
    if (ret == -1) {
        perror("gettimeofday");
        return 1;
    }

    // 打印结果
    printf("Seconds: %ld\n", tv.tv_sec);
    printf("Microseconds: %ld\n", tv.tv_usec);

    return 0;
}
