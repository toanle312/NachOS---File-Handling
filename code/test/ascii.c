
#include "syscall.h"

int main(void) {
    int i;
    
    PrintString("Dec\tChar\n");

    for (i = 32; i < 127; i++) {
        PrintNum(i);
        PrintChar('\t');
        PrintChar(i);
        PrintChar('\n');
    }

    Halt();
}