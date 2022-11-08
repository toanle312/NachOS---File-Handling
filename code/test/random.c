/* random.c
 *	Simple program to test whether the systemcall interface works.
 *	
 *	Just do a random syscall that random the interger number and returns the number.
 *
 */

#include "syscall.h"

int
main()
{
  int result;
  
  result = RandomNum();
  PrintNum(result);
  PrintChar('\n');

  Halt();
  /* not reached */
}
