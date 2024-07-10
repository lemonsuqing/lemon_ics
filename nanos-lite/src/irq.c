#include <common.h>

static Context* do_event(Event e, Context* c) {
  switch (e.event) {
    case 1:
      printf("e.event = 1\n");break;
    case 4:
      printf("e.event = 4\n");break;
    default: panic("Unhandled event ID = %d", e.event);
  }
  c->mepc+=4;
  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
