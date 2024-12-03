#include "main.h"

// Objectives of this lab:
// 1. Familiarize with the STM32CubeIDE: Debug, run, breakpoints
// Views: Variables, breakpoints, registers, expressions, disassembly, SFRs, memory, memory browser
// Memory and instructions as numbers

int main(void)
{
    while (1) {
//        register int counter = 0;
        int counter = 0;
        counter++;
        counter++;
        counter++;
        counter++;
        counter++;
        counter++;
        counter++;
        counter--;
        counter++;
        counter++;
        counter++;
        counter++;
        counter++;
        counter++;

        int use_counter = counter;
    }
}
