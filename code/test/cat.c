
#include "syscall.h"

#define MAX_STRING_LENGTH 255

/*
Le Thanh Tung - 18120640
Input:
	@fileName  source file
Output:
    All data is store in file
Purpose:
    Show file data to console
*/
int main() {
    OpenFileId fileId;
    char fileName[MAX_STRING_LENGTH];

    // Input file name
    PrintString("Nhap ten file: ");
    ReadString(fileName,MAX_STRING_LENGTH);

    // Open file
    fileId = Open(fileName);

    // open file successed
    if (fileId != -1) 
    {    
        int isDone = 0;
        char content[MAX_STRING_LENGTH];

        // check number read bytes of data to check reach EOF, 
        // if numRead < MAX_STRING_LENGTH mean it reach EOF file
        int numRead = 0;

        do {
            // Read content from file
            numRead = Read(content,MAX_STRING_LENGTH,fileId);

            // Print content to console
            PrintString(content);

            // check if it reach end of file
            if (numRead < MAX_STRING_LENGTH) {
                isDone = 1;
            }
        } while(!isDone);
    } 
    else 
    {
        PrintString("[Error] file is not exist\n");
    }
    // close file
    Close(fileId);
    
    Halt();
}