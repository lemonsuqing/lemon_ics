#ifndef __TRACE__
#define __TRACE__

#include "./common.h"

#define word_t uint32_t
void iringbuf_inst(word_t pc, uint32_t data);
void dissplay_inst();
void mtrace_print(paddr_t addr, int len, word_t data,char op);
void init_elf(const char *elf_file);
void init_ftrace(const char *trace_file);
void f_trace_call(paddr_t pc, paddr_t target);
void f_trace_fun_ret(paddr_t pc);
#undef word_t
#endif