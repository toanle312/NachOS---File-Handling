
#include "syscall.h"

#define MAX_STRING_LENGTH 255

int main() {
    OpenFileId fileId;
    char fileName[MAX_STRING_LENGTH];

    PrintString("Nhap ten file: ");
    ReadString(fileName,MAX_STRING_LENGTH);

    fileId = Open(fileName);
    if (fileId != -1) {
        int run = 1;
        char content[MAX_STRING_LENGTH];
        int numRead = 0;
        do {
            numRead = Read(content,MAX_STRING_LENGTH,fileId);
            PrintString(content);

            if (numRead < MAX_STRING_LENGTH) {
                run = 0;
            }
        }while(run);
    } else {
        PrintString("[Error] file is not exist\n");
    }
    Close(fileId);
    Halt();
}