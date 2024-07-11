#include <common.h>
#include "syscall.h"
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    case 0://SYS_exit系统调用
      c->GPRx=0;
      // printf("SYS_exit, c->GPRx=%d\n",c->GPRx);
      halt(c->GPRx);
      break;
    case 1://SYS_yield系统调用
      // printf("SYS_yield, c->GPRx=%d\n",c->GPRx);
      yield();break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
