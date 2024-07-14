#include <common.h>
#include "syscall.h"
#include <sys/time.h>

void Sys_Write(intptr_t *buf, size_t count){
  for (int i = 0; i < count; i++) {
    putch(*((char*)buf + i));
  }
}

int sys_gettimeofday(struct timeval *tv, struct timezone *tz) {
    uint64_t us = io_read(AM_TIMER_UPTIME).us;
    tv->tv_sec = us / 1000000;
    tv->tv_usec = us - us / 1000000 * 1000000;
    return 0;
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;//存储系统调用的类型
  // printf("a[0] = %d\n", a[0]);
  switch (a[0]) {
    case 0:// SYS_exit
      c->GPRx=0;
      halt(c->GPRx);
      // halt(0);
      break;
    case 1:// SYS_yield
      yield();break;
    case 4:// SYS_write
      Sys_Write((intptr_t *)(c->GPR2),c->GPR3);
      break;
    case 9:// SYS_brk
      // 若SYS_brk系统调用成功, 该系统调用会返回0。
      c->GPRx=0;
      break;
    case SYS_gettimeofday:
      c->GPRx = sys_gettimeofday((struct timeval *)(c->GPR2),(struct timezone *)(c->GPR3));
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
