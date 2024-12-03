#include "main.h"

// Objectives of this lab:
// Understand sizes of data types
// First look on the ARM assembly:
//   - The APSR register
//   - Arithmetic operations, data movement, branching
// Exercises on bitwise operations

// (10010111) 0 | 1 = 1
// (1001011) 10 | 1 = 11
// (100101) 110 | 1 = 111
// (10010) 1110 | 0 = 1110
// (1001) 11100 | 1 = 11101
// (100) 111000 | 0 = 111010
// (10) 1110000 | 0 = 1110100
// (1) 11100000 | 1 = 11101001
unsigned int reverseBits(unsigned int num) {
    unsigned int result = 0;
    while (num) {
        result <<= 1;
        result |= (num & 1);
        num >>= 1;
    }
    return result;
}

// While number != 0
// num AND 00000001
// Shift right num to get rid of the tested bit
int countSetBits(int num) {
    int count = 0;
    while (num) {
        count += num & 1;
        num >>= 1;
    }
    return count;
}

int main(void)
{
    while (1) {
        register int counter = 0;
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

        register int counter2 = 0;
        while (counter2 < 10) {
            counter2++;
        }

        unsigned int num = 0xF3;

        // Set 2nd bit
        num |= (1 << 2);

        // Clear 5th bit
        num &= ~(1 << 5);

        // Toggle 6th bit
        num ^= (1 << 6);

        // Test 7th bit
        if (num & (1 << 7)) {

        }

        // num should be now:
        // 0x97 (151)
        // 1001 0111

        countSetBits(num);
        reverseBits(num);
    }
}
