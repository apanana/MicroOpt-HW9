/*
 * Trivial implementation of converter function.
 * You may override this file or create your own.
 */
#include "converter.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int conv(char *str)
{
    int out = 0;
    int i = 0;
    for (; str[i] != '\0'; ++i)
        out = out*10 + str[i] - '0';
    return out;
}

int conv2(char *str)
{
    int out = 0;
    int i = 0;
    for (; str[i] != '\0'; ++i){
       out = (out<<1) + (out<<3) + (str[i] & 15);
       // (out<<1) + (out<<3) does the multiplication by 10
       // (str[i] & 15) converts the ascii character to an int
    }
    return out;
}

int conv3(char *str)
{
    int out = (str[0] & 15); //<- eliminates extra bitshifts from the loop
    int i = 1;
    for (; str[i] != '\0'; ++i){
       out = (out<<1) + (out<<3) + (str[i] & 15);
    }
    return out;
}

// Remove the for loop (which needs to do a check each iteration)
int conv4(char *str)
{
    int out = (str[0] & 15);
    out = (out<<1) + (out<<3) + (str[1] & 15);
    out = (out<<1) + (out<<3) + (str[2] & 15);
    if (str[3] =='\0') return out;
    out = (out<<1) + (out<<3) + (str[3] & 15);
    if (str[4] =='\0') return out;
    out = (out<<1) + (out<<3) + (str[4] & 15);
    return out;
}

/*
void conv5(char lines[],uint32_t *nums){
    __asm__ __volatile__ (
        // Load in our string and conv ascii to numbers
        "movl (%0), %%eax;"
        // "andl $252645135, %%eax;"
        // 1111 1111 0000 1111 0000 1111 0000 1111 in dec
        "andl $4279176975, %%eax;"
        
        // Move our first number into result register
        "movzb %%al, %%ecx;"

        // Second number
        "shl $1, %%ecx;"
        "movl %%ecx, %%ebx;"
        "shl $2, %%ebx;"
        "addl %%ebx, %%ecx;"
        "shr $8, %%eax;"
        "addb %%al, %%cl;"

        // Third number
        "shl $1, %%ecx;"
        "movl %%ecx, %%ebx;"
        "shl $2, %%ebx;"
        "addl %%ebx, %%ecx;"
        "movzb %%ah, %%ebx;" // in case of overflow
        "addl %%ebx, %%ecx;"

        // Check if we are done
        "shr $16, %%eax;"
        "cmp $0, %%eax;"
        "je end;"

        // If not, load in character 4-5 (up to 5th)
        "add $3, %0;"
        "movl (%0), %%eax;"

        // Fourth number
        "shl $1, %%ecx;"
        "movl %%ecx, %%ebx;"
        "shl $2, %%ebx;"
        "addl %%ebx, %%ecx;"
        "andb $15, %%al;"
        "movzb %%al, %%ebx;" // in case of overflow
        "addl %%ebx, %%ecx;"

        // Check if we are done
        "cmp $0, %%ah;"
        "je end;"

        // If not, add 5th number
        "shl $1, %%ecx;"
        "movl %%ecx, %%ebx;"
        "shl $2, %%ebx;"
        "addl %%ebx, %%ecx;"
        "andb $15, %%ah;"
        "movzb %%ah, %%ebx;" // in case of overflow
        "addl %%ebx, %%ecx;"

        "end:"
        "movl %%ecx, (%1);"

        : "+r" (lines)
        : "r" (nums)
        : "memory", "cc", "eax", "bl", "al", "ah", "ebx", "ecx", "cl"
        );
}
*/

void conv6(char lines[],uint32_t *nums){
    __asm__ __volatile__ (
        // Move our first number into result register
        "movl (%0), %%ecx;"
        "andl $15, %%ecx;"
        
        // Shift over our string and convert from ascii to int
        "add $1, %0;"
        "movl (%0), %%eax;"
        // 1111 1111 1111 1111 0000 1111 0000 1111 in decimal
        "andl $4294905615, %%eax;"

        // Second number
        "shl $1, %%ecx;"
        "movl %%ecx, %%ebx;"
        "shl $2, %%ebx;"
        "addl %%ebx, %%ecx;"
        "addb %%al, %%cl;"

        // Third number
        "shl $1, %%ecx;"
        "movl %%ecx, %%ebx;"
        "shl $2, %%ebx;"
        "addl %%ebx, %%ecx;"
        "movzb %%ah, %%ebx;" // in case of overflow
        "addl %%ebx, %%ecx;"

        // Check if we are done
        "shr $16, %%eax;"
        "cmp $0, %%al;"
        "je end;"

        // Fourth number
        "shl $1, %%ecx;"
        "movl %%ecx, %%ebx;"
        "shl $2, %%ebx;"
        "addl %%ebx, %%ecx;"
        "andb $15, %%al;"
        "movzb %%al, %%ebx;" // in case of overflow
        "addl %%ebx, %%ecx;"

        // Check if we are done
        "cmp $0, %%ah;"
        "je end;"

        // If not, add 5th number
        "shl $1, %%ecx;"
        "movl %%ecx, %%ebx;"
        "shl $2, %%ebx;"
        "addl %%ebx, %%ecx;"
        "andb $15, %%ah;"
        "movzb %%ah, %%ebx;" // in case of overflow
        "addl %%ebx, %%ecx;"

        "end:"
        "movl %%ecx, (%1);"

        : "+r" (lines)
        : "r" (nums)
        : "memory", "cc", "eax", "al", "ah", "ebx", "bl", "ecx", "cl"
        );
}

// Just a function I made to see if everything was indeed 
// either 3,4, or 5 digits long.
void counter(unsigned nlines, char *lines[], quote_t nums[])
{
    unsigned i = 0; //<- pretty sure this doesn't do much to help
    int three,four,five;
    three=four=five=0;
    for (; i < nlines; ++i)
    {
        if (strlen(lines[i]) == 3) ++three;
        if (strlen(lines[i]) == 4) ++four;
        if (strlen(lines[i]) == 5) ++five;
        nums[i] = conv3(lines[i]);
    }
    printf("nlines: %d\n",nlines);
    printf("3: %d\t4: %d\t5: %d\t\n",three,four,five);
    printf("sum:%d\n",three+four+five);
}

void convert_all(unsigned nlines, char *lines[], quote_t nums[])
{
    unsigned i = 0; //<- pretty sure this doesn't do much to help
    for (; i < nlines; ++i)
        nums[i] = conv4(lines[i]);
        // conv6(lines[i],&nums[i]);
}
