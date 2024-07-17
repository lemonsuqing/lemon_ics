#include <common.h>
#include "syscall.h"
#include <sys/time.h>

void Sys_Write(intptr_t *buf, size_t count){
  for (int i = 0; i < count; i++) {
    putch(*((char*)buf + i));
  }
}

int SYS_Read(int fd, intptr_t *buf, size_t count){
  int r_len = fs_read(fd, buf, count);
  return r_len;
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
    case SYS_exit:
      c->GPRx=0;
      halt(c->GPRx);
      break;
    case SYS_yield:
      yield();break;
    case SYS_write:// 4
      Sys_Write((intptr_t *)(c->GPR2),c->GPR3);
      break;
    case SYS_brk:// 9
      c->GPRx=0;
      break;
    case SYS_read:
      // c->GPRx = fs_read(c->GPR2, (void *)c->GPR3, (size_t)c->GPR4);
      c->GPRx = SYS_Read((int)(c->GPR2), (intptr_t *)(c->GPR3), (size_t)(c->GPR4));
      break;
    case SYS_open://2
      c->GPRx = fs_open((const char *)c->GPR2, c->GPR3, c->GPR4);
      break;
    case SYS_gettimeofday:
      c->GPRx = sys_gettimeofday((struct timeval *)(c->GPR2),(struct timezone *)(c->GPR3));
      break;
    case SYS_lseek:
      c->GPRx = fs_lseek(c->GPR2, (size_t)c->GPR3, c->GPR4);
      //Log("fs_lseek(%d, %d, %d) = %d", c->GPR2, c->GPR3, c->GPR4, ret);
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
