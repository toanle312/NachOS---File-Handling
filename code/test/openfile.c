#include "syscall.h"

int main()
{
    int open = Open("test.txt");
    Close(open);

    Halt();
    return 0;
}