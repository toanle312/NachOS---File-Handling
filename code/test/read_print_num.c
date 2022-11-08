/* read_print_num.c
 *	Simple program to test whether the systemcall interface works.
 *	
 *	Just do a read_print_num syscall that read a interger then print it to console
 */

#include "syscall.h"

int
main()
{
  PrintString("Enter a integer: ");
  PrintNum(ReadNum());
  PrintChar('\n');

  Halt();
  /* not reached */
}
