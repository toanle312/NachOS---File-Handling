#include "syscall.h"

int main() {
    PrintString("===== About team =====\n");
    PrintString("18120640 - Le Thanh Tung\n");
    PrintString("20120386 - Le Phuoc Toan\n");
    PrintString("20120395 - Nguyen Anh Tuan\n");

    PrintString("\n===== About ASCII program =====\n");
    PrintString("ASCII program will just print the readable characters with its decimal code\n");
    PrintString("between 32 and 126.\n");
    PrintString(
        "To run: \n"
        "\tStep 1: go to 'code' directory.\n"
        "\tStep 2: type 'build.linux/nachos -x test/ascii'.\n"
        "\tAfter all these steps, the program will print the ascii table and then halt.\n");

    PrintString("\n===== About Sorting program =====\n");
    PrintString(
        "To run: \n"
        "\tStep 1: Go to 'code' directory.\n"
        "\tStep 2: Type 'build.linux/nachos -x test/bubble_sort'.\n"
        "\tStep 3: Enter n (the length of the array, 0 <= n <= 100).\n"
        "\tStep 4: Enter elements of the array.\n"
        "\tStep 5: Enter the order you want to sort (0: increasing, 1: decreasing).\n"
        "\tAfter all these steps, the program will print out the sorted array and then halt.\n");

    Halt();
}
