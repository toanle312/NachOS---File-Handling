
#include "syscall.h"


int main() {
    int fileId = Open("test.txt");
    
    Write("Hello world",13,fileId);

    Close(fileId);

    Halt();
}