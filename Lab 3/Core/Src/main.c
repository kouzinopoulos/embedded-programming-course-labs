#include "main.h"

// Objectives of this lab:
// Understand pointers and pointer arithmetics
// Have a first look at the ARM ISA:
//   - Simple arithmetics
//   - Data movement
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main(void)
{
    while (1) {
        int counter = 0;

        int *p_int;
        p_int = &counter;
        (*p_int) = 0;

        while (*p_int < 10) {
            (*p_int)++;
        }

        p_int = (int*) 0x20000012;
        *p_int = 0x5A5A5A5A;

        int x = 10;
        int *ptr = &x;  // Reference
        *ptr = 20;      // Dereference and modify

        int arr[3] = {5, 10, 15};
        int *ptr2 = arr;
        int sum = *ptr2 + *(ptr2 + 1);

        int nums[4] = {2, 4, 6, 8};
        int *ptr3 = nums;
        for (int i = 0; i < 4; i++) {
            *(ptr3 + i) *= 2;  // Multiply each element by 2
        }

        int xx = 7, yy = 9;
        swap(&xx, &yy);

        ASM_Function();
    }
}
