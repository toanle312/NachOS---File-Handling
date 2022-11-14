
#include "syscall.h"
#define MAX_STRING_LENGTH 255

/*
Nguyen Anh Tuan - 20120395
Input:
	@fileName1  source file 1
    @fileName2  source file 2
Output:
    modified file 1
Purpose:
    Copy data in file 2 to the EOF position of file 1
*/
int main() {

    OpenFileId fileId1;
    char fileName1[MAX_STRING_LENGTH];

    OpenFileId fileId2;
    char fileName2[MAX_STRING_LENGTH];

    // Input file name
    PrintString("Nhap ten file nguon 1: ");
    ReadString(fileName1,MAX_STRING_LENGTH);

    PrintString("Nhap ten file nguon 2: ");
    ReadString(fileName2,MAX_STRING_LENGTH);

    fileId1 = Open(fileName1);
    fileId2 = Open(fileName2);

    Seek(-1,fileId1);

    // both files is open
    if (fileId1 != -1 && fileId2 != -1) {
        int run = 1;
        char content[MAX_STRING_LENGTH];
        int numRead = 0;

        Seek(-1, fileId1);

        do {
            // Copy data from file 2
            numRead = Read(content,MAX_STRING_LENGTH,fileId2);

            // Write copied data of file2 to the end position of file1
            Write(content,numRead,fileId1);

            // check if reach end of the file
            if (numRead < MAX_STRING_LENGTH) {
                run = 0;
            }
        }while(run);
    } else {
        PrintString("[Error] file is not exist\n");
    }
    Close(fileId2);
    Close(fileId1);

    Halt();
}