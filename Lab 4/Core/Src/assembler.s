/*
 * assembler.s
 *
 *  Created on: Dec 4, 2024
 *      Author: P70092343
 */
.syntax unified
.text
.global compare_numbers
.global sum_numbers
.global max_of_three
.global if_else_example
.global fibonacci
.thumb_func
.equ COUNTER, 10000

compare_numbers:
    // Inputs: r0 = first number, r1 = second number
    cmp r0, r1         // Compare r0 and r1
    bgt greater       // Branch to "greater" if r0 > r1
    movs r0, #0      // Set result to 0 (r0 <= r1 case)
    bx lr                   // Return
greater:
    movs r0, #1      // Set result to 1 (r0 > r1 case)
    bx lr                   // Return

sum_numbers:
    // Input: r0 = n (the upper limit)
    movs r1, #0        // Initialize sum in r1
    movs r2, #1        // Initialize counter i = 1

loop:
    cmp r2, r0         // Compare counter i (r2) with n (r0)
    bgt end_loop       // If i > n, break out of the loop
    adds r1, r1, r2    // sum = sum + i
    adds r2, r2, #1    // i = i + 1
    b loop             // Repeat the loop

end_loop:
    mov r0, r1         // Store the result in r0
    bx lr              // Return

max_of_three:
    // Inputs: r0 = first number, r1 = second number, r2 = third number
    cmp r0, r1         // Compare first two numbers (r0, r1)
    bgt check_third     // If r0 > r1, check with third number
    mov r0, r1         // Otherwise, set r0 = r1 (max so far)

check_third:
    cmp r0, r2         // Compare max(r0) with r2
    bgt end            // If r0 > r2, we're done
    mov r0, r2         // Otherwise, set r0 = r2 (max is r2)

end:
    bx lr              // Return the result

if_else_example:
    // Input: r0 = number
    cmp r0, #0            // Compare input with 0
    beq return_zero       // If input == 0, go to return_zero
    bmi return_negative   // If input < 0 (negative), go to return_negative

    // Input is positive
    movs r0, #10          // Return 10 for positive input
    bx lr                 // Return

return_negative:
    movs r0, #-10         // Return -10 for negative input
    bx lr                 // Return

return_zero:
    movs r0, #0           // Return 0 for zero input
    bx lr                 // Return

fibonacci:
    // Input: r0 = n (the Fibonacci number to compute)
    cmp r0, #1            // Compare n with 1
    ble done              // If n <= 1, return n (either 0 or 1)

    // Initialize first two Fibonacci numbers
    movs r1, #0           // r1 = fib(0) = 0
    movs r2, #1           // r2 = fib(1) = 1
    subs r0, r0, #1       // r0 = n - 1 (remaining iterations)

fib_loop:
    subs r0, r0, #1       // Decrement n (r0 = r0 - 1)
    beq done              // If n == 0, we are done
    adds r3, r1, r2       // r3 = r1 + r2 (fib(i-2) + fib(i-1))
    mov r1, r2            // r1 = fib(i-1)
    mov r2, r3            // r2 = fib(i)
    b fib_loop            // Repeat the loop

done:
    mov r0, r2            // r0 = nth Fibonacci number
    bx lr                 // Return
