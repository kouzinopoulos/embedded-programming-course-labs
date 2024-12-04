/*
 * assembler.s
 *
 *  Created on: Dec 4, 2024
 *      Author: P70092343
 */

.syntax unified
.text
.global ASM_Function
.thumb_func
.equ COUNTER, 10000

ASM_Function:

MOV r1, #0x30
MOV r2, #0x1a
ADD r3, r1, r2      // r3 = r1 + r2
LSL r3, r3, #2      // r3 = r3 * 4
EOR r4, r1, r2      // r4 = r1 XOR r2
LSR r5, r2, #3      // r5 = r2 >> 3
LSL r6, r1, #3      // r6 = r1 * 8

MOV r0, #0x1234      // Load value into r0
LDR r1, =0x20000000  // Load memory address
STR r0, [r1]         // Store value
LDR r2, [r1]         // Load value back into r2
ADD r2, r2, #1       // Increment value
STR r2, [r1]         // Store incremented value

// Array base address: 0x20000000
LDR R0, =0x20000000  // Load base address of array into R0

// Store array in memory
MOV R1, #0x9
STR R1, [R0]
MOV R1, #0x25
STR R1, [R0, #4]
MOV R1, #0x46
STR R1, [R0, #8]

// 1. Sum the first three elements
LDR R1, [R0]          // Load first element into R1
LDR R2, [R0, #4]      // Load second element into R2
LDR R3, [R0, #8]      // Load third element into R3
ADD R4, R1, R2        // Add first and second elements
ADD R4, R4, R3        // Add third element

// 2. Multiply the second element by 3 and store it back
LDR R1, [R0, #4]      // Load second element into R1
MOV R2, #0x3
MUL R1, R1, R2        // R1 = R1 * 3
STR R1, [R0, #4]      // Store result back at second element's address
