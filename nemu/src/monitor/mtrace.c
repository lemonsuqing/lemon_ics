#include <trace.h>

void mtrace_print(vaddr_t addr, int len, word_t data,char op){
    printf("%c\n",op);
    if(op == 'r'){
        printf("123\n");
        printf("read 0x%08x %d byte. Data: 0x%08x\n", addr, len, data);
    }else{
        log_write(ANSI_FMT("write %d byte data into 0x%08x. data: 0x%08x", ANSI_BG_WHITE)"\n",len, addr, data);
    }
}