/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"
#include "synchconsole.h"
#include "filesys.h"
#include "syscall.h"
#include "openfile.h"

#define LINE_FEED '\n'
#define CARRIAGE_RETURN '\r'
#define TAB '\t'
#define SPACE ' '

#define MAX_LENGTH_INT32 11
char numberBuffer[MAX_LENGTH_INT32 + 2];
#define MAX_READ_STRING_LENGTH 255

bool flagMin = false;

/** 
 * @brief Halt the system 
 * 
 * @return void
*/
void SysHalt()
{
  kernel->interrupt->Halt();
}

/** 
 * @brief Add 2 interger
 * @param op1 interger 1
 * @param op2 interger 2
 * @return int
*/
int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

/** 
 * @brief check special charater
 * @param c character
 * @return True/False
*/
bool isSpecialChar(char c)
{
  return (c == LINE_FEED || c == CARRIAGE_RETURN || c == TAB || c == SPACE);
}

/** 
 * @brief compare interger number vs buffer of this number
 * @param number interger number
 * @param str buffer of this number
 * @return True/False
*/
bool compareIntVsStr(int number, const char *str)
{
  int n = strlen(str);

  bool neg = (str[0] == '-');

  // Check if number is negative
  if(neg)
  {
    number = -number;
  }

  // check elements in number with elements is buffer
  while(number && n > neg)
  {
    // check inverse buffer with inverse number
    // --n to continue check next element
    if((number % 10) != (str[--n] - '0')) 
    {
      return false;
    }
    number = number / 10;
  }

  // if (n = postion of '-' in buffer) then return true
  return (n == neg);

}

/** 
 * @brief Read interger number from console
 * 
 * @return int
*/
int SysReadNum()
{
  // init value for numberBuffer
  memset(numberBuffer, 0, sizeof(numberBuffer));

  // read character from console
  char c = kernel->synchConsoleIn->GetChar();

  // if first c is end of file then number = 0
  if(c == EOF)
  {
    DEBUG(dbgSys, "Error EOF !!! Unvalid Format Data Number.\n");
    return 0;
  }

  // if first c special char then number = 0
  if(isSpecialChar(c))
  {
    DEBUG(dbgSys, "Error Special Character !!! Unvalid Format Data Number.\n");
    return 0;
  }

  int size = 0;

  // if next c is not special character and end of file then continue
  while(!(isSpecialChar(c) || c == EOF))
  {
    // insert character to buffer
    numberBuffer[size++] = c;

    if(size > MAX_LENGTH_INT32)
    {
      DEBUG(dbgSys, "Number Is Too Large.\n");
      break;
    }
    // continue read character from console
    c = kernel->synchConsoleIn->GetChar();
  }

  // if number is not MIN_INT32 then turn off (False) flagMin (by default is True)
  for(int i = 0; i < 11; i++)
  {
    if(numberBuffer[i] != "-2147483648"[i])
    {
      flagMin = false;
    }
  }

  int n = strlen(numberBuffer);

  if(n == 0)
  {
    return 0;
  }

  // check if number is negative number
  bool neg = (numberBuffer[0] == '-');

  // firstZero is first characrter of buffer is '0'
  bool firstZero = (numberBuffer[0] == '0');

  // secondChar is second characrter of buffer is between '0' and '9'
  bool secondChar = (numberBuffer[1] >= '0' && numberBuffer[1] <= '9');

  // secondZero is second characrter of buffer is '0'
  bool secondZero = (numberBuffer[neg] == '0');

  // check if number is 0x or -0x or 00x
  if((firstZero && secondChar) || (neg && secondZero))
  {
    DEBUG(dbgSys, "Unvalid Format Data Number.\n");
    return 0;
  }

  int number = 0;

  // convert numberBuffer(char[]) to number(int)
  for(int i = neg; i < n; i++)
  {
    char temp = numberBuffer[i];

    if(temp < '0' || temp > '9')
    {
      DEBUG(dbgSys, "Number is too large.\n");
      return 0;
    }

    number = number * 10 + (temp - '0');
  }
  
  // if first character is '-' then number is negative
  if(neg)
  {
    number = -number;
  }

  // if number is different with numberBuffer => number is out of range
  if(compareIntVsStr(number, numberBuffer) == false)
  {
    DEBUG(dbgSys, "Out Of Range Interger Number.\n");
    return 0;
  }

  return number;
}

/** 
 * @brief Print interger number to  console
 * 
 * @return void
*/
void SysPrintNum(int number)
{
  // if flagMin is turn on that mean number is MIN_INT32
  if(flagMin)
  {
    kernel->synchConsoleOut->PutChar('-');
    for(int i = 0; i < 10; i++)
    {
      kernel->synchConsoleOut->PutChar("2147483648"[i]);
    }
    return;
  }

  // if number = 0 then print 0 to console
  if(number == 0)
  {
    kernel->synchConsoleOut->PutChar('0');
    return;
  }
  
  // if number < 0 then print '-' to console ahead then print abs(number)
  if(number < 0)
  {
    number = -number;
    kernel->synchConsoleOut->PutChar('-');
  }

  // convert number to buffer
  int index = 0;
  while(number)
  {
    numberBuffer[index++] = (number % 10);
    number = number / 10;
  }

  // loop buffer to print character to console
  for(int i = index - 1; i >= 0; i--)
  {
    kernel->synchConsoleOut->PutChar(numberBuffer[i] + '0');
  }

}

/** 
 * @brief Read character from console
 * 
 * @return char
*/
char SysReadChar() { return kernel->synchConsoleIn->GetChar(); }

/** 
 * @brief Print character to  console
 * 
 * @return void
*/
void SysPrintChar(char character) { return kernel->synchConsoleOut->PutChar(character); }

/** 
 * @brief Read string from console
 * @param length length of string that you want to read
 * @return char*
*/
char* SysReadString(int length) {
    char* buffer = new char[MAX_READ_STRING_LENGTH];
    char oneChar;
    int i = 0;
    do {
      oneChar = SysReadChar();
      if (oneChar == '\n') {
        break;
      }
      buffer[i++] = oneChar;
    } while (true);
    buffer[length-1] = '\0';
    return buffer;
}

/** 
 * @brief Print string to  console
 * @param buffer kernel string that will print out
 * @return void
*/
void SysPrintString(char* buffer) {
    int len = strlen(buffer);
    for (int i = 0; i < len; i++) {
        kernel->synchConsoleOut->PutChar(buffer[i]);
    }
}

/** 
 * @brief Random interger number
 * 
 * @return int
*/
int SysRadomNum()
{
  flagMin = false;
  int result;
  
  srand(time(NULL));

  // rand() will return integer between 0 and RAND_MAX
  // but 0 is not positive integer
  // so we need to add 1 so that function can always return positive integer
  result = rand()+1; 

  return result;
}

OpenFileId SysOpenFile(char* name)
{
  // check status of file
  OpenFile* opf = kernel->fileSystem->Open(name);

  if(strcmp("stdin", name)==0)
  {
    return 0;
  }

  if(strcmp("stdout", name)==0)
  {
    return 1;
  }
  
  OpenFileId id = kernel->fileSystem->index;

  if(id < 0 || id > 10)
  {
    return -1;
  }

  // file is open
  if(opf != NULL)
  {
    // update status of file
    // save status of file into openf to manage
    kernel->fileSystem->openf[id] = opf;
    return id;
  }
  // file is not open
  else
  {
    // update status of file
    kernel->fileSystem->openf[id] = NULL;
    return -1;
  }
}

int SysCloseFile(OpenFileId id)
{
   // check status of file
  OpenFile* opf = kernel->fileSystem->openf[id];

  // file is open
  if(opf != NULL && id != -1) 
  {
    // update status of file -> close file
    delete kernel->fileSystem->openf[id];
    kernel->fileSystem->openf[id] = NULL;
    return 0;
  }
  // file is not open
  else
  {
    // can not close file
    return -1;
  }
}

/*
Lê Thanh Tùng - 18120640
Input:
	@fileName: file name you want to remove
Output:
  1: remove file successed
  0: remove file failed because the file is opening
  -1: remove file failed because the file does not exist
*/
int SysRemoveFile(char* name){return -1;}
// {
//   bool isFileOpening = FALSE;

//   // Check if file is opening
//   for (int i = 0; i < 10; i++) {
//     if(name == kernel->fileSystem->openf[i]->name) {
//       isFileOpening = TRUE;
//       break;
//     }
//   }
//   // File is opening
//   if (!isFileOpening)
//   {
//     // Remove file failed because the file is open
//     return 0;
//   }
//   bool isRemove = kernel->fileSystem->Remove(name);

//   if (isRemove) {
//     // Remove file successed
//     return 1;
//   }
//   // Remove file failed because the file does not exits
//     return -1;
// }

int SysSeekFile(int position, OpenFileId id)
{
  OpenFile* opf = kernel->fileSystem->openf[id];

  if(id < 0 || id >10)
  {
    return -1;
  }

  if(opf == NULL)
  {
    //SysPrintString("Seek fail \n");
    return -1;
  }else{
    if(position == -1)
    {
      position = kernel->fileSystem->openf[id]->Length();
    }

    if(position > kernel->fileSystem->openf[id]->Length() || position < 0)
    {
      return -1;
    }
    else{
      kernel->fileSystem->openf[id]->Seek(position);
      return position;
    }
  }
}

bool isOpen(OpenFileId id) {
  OpenFile* opf = kernel->fileSystem->openf[id];
  if (opf == NULL) {
    return FALSE;
  }
  return TRUE;
}


int StringLength(char str[]) {
  int len = 0;
  char c;
  do {
    c = str[len++];
    if (len == MAX_READ_STRING_LENGTH) {break;}
  } while (c != '\0');

  return --len;
}

#endif /* ! __USERPROG_KSYSCALL_H__ */