#include <am.h>
#include <riscv/riscv.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    printf("111\n");
    printf("c->mcause = 0x%08x\n", c->mcause);
    switch (c->mcause) {
      case 0:
        printf("333\n");
        ev.event=EVENT_YIELD;
        break;
      default: ev.event = EVENT_ERROR;
        printf("444\n");
        break;
    }

    c = user_handler(ev, c);
    assert(c != NULL);
  }
  // printf("mcause:0x%08x, mstatus:0x%08x, mepc:0x%08x",c->mcause,c->mstatus,c->mepc);
  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrw mtvec, %0" : : "r"(__am_asm_trap));

  // register event handler
  user_handler = handler;

  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  return NULL;
}

void yield() {
#ifdef __riscv_e
  asm volatile("li a5, -1; ecall");
#else
  asm volatile("li a7, -1; ecall");
#endif
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
