#include <common.h>
#include "syscall.h"

void SYS_write(intptr_t *buf, size_t count){
  for (int i = 0; i < count; i++) {
    putch(*((char*)buf + i));
  }
}


void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;//存储系统调用的类型
  switch (a[0]) {
    case 0:// SYS_exit
      c->GPRx=0;
      halt(c->GPRx);
      break;
    case 1:// SYS_yield
      yield();break;
    case 4:// SYS_write
      SYS_write((intptr_t *)c->GPR2,c->GPR3);
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
