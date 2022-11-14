
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
    Copy all data in file 2 to the EOF position of file 1
*/
int main() {

    OpenFileId fileId1;
    char fileName1[MAX_STRING_LENGTH];

    OpenFileId fileId2;
    char fileName2[MAX_STRING_LENGTH];

    // Input name for file1
    PrintString("Input source file: ");
    ReadString(fileName1,MAX_STRING_LENGTH);

    // Input name for file2
    PrintString("Input destination file: ");
    ReadString(fileName2,MAX_STRING_LENGTH);

    // Open file1
    fileId1 = Open(fileName1);

    // Open file2
    fileId2 = Open(fileName2);

    // if both files are opened
    if (fileId1 != -1 && fileId2 != -1) 
    {
        int isDone = 0;
        char data[MAX_STRING_LENGTH];

        // check number read bytes of data to check reach EOF, 
        // if numRead < MAX_STRING_LENGTH mean it reach EOF file2
        int numRead = 0;

        // Seek to EOF position of file1
        Seek(-1,fileId1);

        // Concatenate file
        do 
        {
            // Copy data from file 2
            numRead = Read(data,MAX_STRING_LENGTH,fileId2);

            // Write copied data of file2 to the end position of file1
            Write(data,numRead,fileId1);

            // check if reach end of the file2
            if (numRead < MAX_STRING_LENGTH) {
                isDone = 1;
            }
        }while(!isDone);
        PrintString("Concatenate successed!!!\n");
    } 
    else // open file failed
    {
        PrintString("[Error] file is not exist\n");
    }
    // Close file1
    Close(fileId1);

    // Close file2
    Close(fileId2);

    Halt();
}