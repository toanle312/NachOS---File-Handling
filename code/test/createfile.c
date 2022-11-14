#include "syscall.h"

int main()
{
    char fileName[255];
    int isValidFileName = 1;
    int i = 0;

    PrintString("Input name of file that you want to create: ");
    ReadString(fileName, 255);

    if(!fileName[0])
    {
      isValidFileName = 0;
    }
  
    if(isValidFileName == 0)
    {
      PrintString("Unvalid file name !!!\n");
    }
    else{
      if(Create(fileName) == 1)
      {
        PrintString("Create file ");
        PrintString(fileName);
        PrintString(" succeed !!!\n");
      }
      else{
        PrintString("Can not create file ");
        PrintString(fileName);
        PrintString("\n");
      }
    }
    Halt();
    return 0;
}