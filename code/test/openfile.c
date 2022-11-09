#include"syscall.h"

int main()
{
    OpenFileId open = Open("test.txt");
    Close(open);

    Halt();
    return 0;
}