
#include "syscall.h"


int main() {
    int fileId = Open("test2.txt");
    
    char* buff = "This is beautiful";
    char buf[25];
    Write(buff,20,fileId);

    Seek(0,fileId);
    Read(buf,25,fileId);
    PrintString(buf);

    Close(fileId);

    Halt();
}