/* read_print_char.c
 *	Simple program to test whether the systemcall interface works.
 *	
 *	Just do a read_print_char syscall that read a char then print it to console.
 *
 */
#include "syscall.h"

int main(void) {

    char oneChar;
    
    PrintString("Enter a character: ");
    oneChar = ReadChar();
    PrintChar(oneChar);
    PrintChar('\n');

    Halt();
}