#include <common.h>
#include "syscall.h"
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    case 0:c->GPRx=0;printf("SYS_exit， do_syscall此时 c->GPRx=%d\n",c->GPRx);halt(c->GPRx);//SYS_exit系统调用
    case 1:printf("SYS_yield， do_syscall此时c->GPRx=%d\n",c->GPRx);yield(); //SYS_yield系统调用
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
