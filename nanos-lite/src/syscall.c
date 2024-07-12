#include <common.h>
#include "syscall.h"
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
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
