#include "syscall.h"

#include"copyright.h"
#define MAX_STRING_LENGTH 255

int main() 
{
    char src[255];
    char dest[255];
    char srcBuf[255];

    int srcId;
    int destId;

    int i;
    int isValidFileName = 1;
    int temp;

    PrintString("Input name of source file that you want to copy: ");
    ReadString(src, 255);

    PrintString("Input name of destination file that you want to paste: ");
    ReadString(dest, 255);

    srcId = Open(src);

    if(srcId != -1)
    {
        Read(srcBuf,255,srcId);

        if(!dest[0])
        {
            isValidFileName = 0;
        }

        if(isValidFileName != 0)
        {
            if(Create(dest) == 1)
            {
                destId = Open(dest);

                if(destId != -1)
                {
                    Write(srcBuf,255,destId);

                    PrintString("Copy succeed !!!\n");

                    Close(destId);
                }
                else{
                    PrintString("Can not open destination file !!!\n");
                }
            }
            else{
                PrintString("Can not create destination file !!!\n");
            }
        }
        else{
            PrintString("Unvalid destination file name!!!\n");
        }
    }
    else{
        PrintString("Can not open source file !!!\n");
    }

    Close(srcId);

    Halt();
}