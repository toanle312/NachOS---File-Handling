#include "syscall.h"

int main()
{
    if (Create("test1.txt") == -1)
    {
      PrintString("Can not create file test.txt\n");
    } else
    {
      PrintString("Create file test.txt successfull\n");
    }
      
    Halt();
    return 0;
}