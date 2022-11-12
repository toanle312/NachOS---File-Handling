
#include "syscall.h"


int main() {
    int fileId = Open("test1.txt");
    
    char* buff = "Hello world";
    Write(buff,20,fileId);

    Close(fileId);

    Halt();
}