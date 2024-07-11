#include <common.h>

static Context* do_event(Event e, Context* c) {
   printf("do_event\n");
  // printf("do_event's e.event=%d\n",e.event);
  switch (e.event) {
    case 1:// yeild
      // printf("yeild: e.event = 1\n");break;
      halt(0);break;
    case 2:// syscall
      do_syscall(c);break;//EVENT_SYSCALL
    default: panic("Unhandled event ID = %d", e.event);
  }
  c->mepc+=4;
  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
