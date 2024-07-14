#include <stdio.h>
#include <NDL.h>

int main() {
  NDL_Init(0);
  while (1) {
    char buf[64];
    printf("1\n");
    if (NDL_PollEvent(buf, sizeof(buf))) {
      printf("1\n");
      printf("receive event: %s\n", buf);
    }
  }
  return 0;
}
