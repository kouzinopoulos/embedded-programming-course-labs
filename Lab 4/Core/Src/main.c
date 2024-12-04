#include "main.h"
#include "assembler.h"

// Objectives of this lab:
// Understand pointers and pointer arithmetics
// Have a first look at the ARM ISA:
//   - Simple arithmetics
//   - Data movement
int main(void)
{
    while (1) {
        unsigned int result;
        result = compare_numbers(5, 10);
        result = sum_numbers(10);
        result = max_of_three(15, 10, 5);
        int result2 = if_else_example(-20);
        result = fibonacci(6);
    }
}
