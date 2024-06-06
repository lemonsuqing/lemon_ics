#ifndef __TRACE__
#define __TRACE__

#include "./common.h"

#define word_t uint32_t
void iringbuf_inst(word_t pc, uint32_t data);
void dissplay_inst();
void mtrace_print(paddr_t addr, int len, word_t data,char op);
#undef word_t
#endif