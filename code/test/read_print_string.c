/* read_print_string.c
 *	Simple program to test whether the systemcall PrintString and ReadString work.
 *	
 *	
 *
 */

#include "syscall.h"

int main()
{
    char str[255];

    PrintString("Enter a string (length < 255): ");
    ReadString(str, 255);

    PrintString(str);
    PrintChar('\n');
    
    Halt();
  /* not reached */
}