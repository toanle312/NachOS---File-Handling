/* bubbel_sort.c 
 *    Test program use bubble sort to sort a large number of integers.
 */

#include"syscall.h"
#define SIZE 100

int main()
{
    int n = 0;
    int array[SIZE + 1];
    int check = 0;
    int i = 0;
    int j =0;
    int choose = 0;
    int temp = 0;

    // input n is size of array following: 0 <= n <= 100
    // if wrong user input then input again
    do{
        PrintString("Please enter interger n (0 <= n <= 100): ");

        n = ReadNum();

        if(n < 0 || n > 100)
        {
            PrintString("Wrong input, n must be be an interger between 0 and 100. Please try again following instruction !!!\n");
        }

    }while(n < 0 || n > 100);
    
    // if correct user input then input the elements in the array
    for(i = 0; i < n; i++)
    {
        PrintString("Array[");PrintNum(i);PrintString("] = ");
        array[i] = ReadNum();
    }

    // input choose is sort order: 0: ascending  , 1: descending, by default is: 0
    // if wrong user input then input again
    do{
        PrintString("Please choose your sort order (0: ascending  , 1: descending, by default is: 0): ");
        
        choose = ReadNum();

        if(choose != 0 && choose != 1)
        {
            PrintString("Wrong input, your chosen must be 0 or 1. Please try again following instruction !!!\n");
        }

    }while(choose != 0 && choose != 1);

    // bubble sort algorithm
    for(i = 0; i < n; i++)
    {
        for(j = 0; j < n - 1; j++)
        {
            // if choose = 0 then sort ascending
            if(choose == 0)
            {
                if(array[j] > array[j + 1])
                {
                    temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
                }
            }
            // if choose = 1 then sort descending
            else if(choose == 1) 
            {
                if(array[j] < array[j + 1])
                {
                    temp = array[j];
                    array[j] = array[j + 1];
                    array[j + 1] = temp;
                }
            }
        }
    }

    // print array after sort
    for(i = 0; i < n; i++)
    {
        PrintNum(array[i]);
        PrintChar(' ');
    }
    PrintChar('\n');

    Halt();
}