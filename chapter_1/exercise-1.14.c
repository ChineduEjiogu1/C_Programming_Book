/**
 * Write a program to print a histogram of the frequencies of different characters in its input.
*/

#include <stdio.h>

#define ASCII 128

int main(void)
{
    int c, i, j, max_freq = 0;

    int freq[ASCII];

    for(i = 0; i < ASCII; freq[i++] = 0);

    while( (c = getchar()) != EOF ) 
    {
        if(c != ' ' && c != '\n' && c != '\t')
        {
            freq[c]++;
        }        
    }

    for(i = 0; i < ASCII; i++)
    {
        if(freq[i] > max_freq)
        {
            max_freq = freq[i];
        } 
    }

    for(i = 0; i < ASCII; i++)
    {
        if(freq[i] > 0)
        {
            printf("%c | ", i);

            for(j = 0; j < freq[i]; j++)
            {
                printf("\u25A0  ");
            }

            printf("\n");
        }
    }

    printf("   +");

    for(i = 1; i <= max_freq; i++)
    {
        printf("———");
    }

    printf("\nW/F ");

    for(i = 1; i <= max_freq; i++)
    {
        printf("%02d ", i);
    }


    return 0;
}