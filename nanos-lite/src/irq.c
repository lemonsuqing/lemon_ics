#include <common.h>

static Context* do_event(Event e, Context* c) {
  printf("do_event's e.event=%d\n",e.event);
  switch (e.event) {
    case 1:
      printf("e.event = 1\n");break;
    case 2:do_syscall(c);break;//EVENT_SYSCALL
    default: panic("Unhandled event ID = %d", e.event);
  }
  c->mepc+=4;
  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
