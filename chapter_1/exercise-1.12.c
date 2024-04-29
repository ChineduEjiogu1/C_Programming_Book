/* Write a program that prints its input one word per line. */

#include <stdio.h>

#define IN 1
#define OUT 0

int main()
{
    int inputChar, state;
    state = OUT;

    while((inputChar = getchar()) != EOF)
    {
        if(inputChar == ' ' || inputChar == '\t' || inputChar == '\n')
        {
            state = OUT;
        }
        else if(state == OUT)
        {
            state = IN;
            putchar('\n');
        }
        putchar(inputChar);
    }

    return 0;
}