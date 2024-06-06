#include "trace.h"
#define IRINGBUF_SIZE 16

typedef struct ItraceNode{
    word_t pc;
    uint32_t data;
}ItraceNode;

ItraceNode iringbuf[IRINGBUF_SIZE];
int iringbuf_index = 0;
bool flag = 0;

void iringbuf_inst(word_t pc, uint32_t data){
    iringbuf[iringbuf_index].pc = pc;
    iringbuf[iringbuf_index].data = data;
    iringbuf_index = (iringbuf_index + 1)%IRINGBUF_SIZE;
    flag = flag || iringbuf_index == 0;
}

void dissplay_inst(){
    if(!flag && !iringbuf_index) return;

    int end = iringbuf_index;
    int i = flag ? iringbuf_index : 0;
    void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
    char buf[128];
    char *p;
    do{
        p = buf;
        p += sprintf(buf, "%s" FMT_WORD ": %08x ", (i+1)%IRINGBUF_SIZE==end?" --> ":"     ", iringbuf[i].pc, iringbuf[i].data);
        disassemble(p, buf+sizeof(buf)-p, iringbuf[i].pc, (uint8_t *)&iringbuf[i].data, 4);

        if ((i+1)%IRINGBUF_SIZE==end) printf(ANSI_FG_RED);
        printf("1");
        puts(buf);
    }while((i = (i+1)%IRINGBUF_SIZE) != end);
    puts(ANSI_NONE);
}