#include "syscall.h"

int main()
{
    int open = Open("test1.txt");
    Close(open);

    Halt();
    return 0;
}