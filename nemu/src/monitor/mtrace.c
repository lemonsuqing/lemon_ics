#include <trace.h>

void mtrace_print(vaddr_t addr, int len, word_t data,char op){
    if(op == 'r'){
        printf(ANSI_FMT("read 0x%08x %d byte. Data: 0x%08x\n", ANSI_FG_YELLOW), addr, len, data);
    }else{
        printf(ANSI_FMT("write %d byte data into 0x%08x. data: 0x%08x\n", ANSI_BG_WHITE)"\n",len, addr, data);
    }
}