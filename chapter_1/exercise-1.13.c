#include <stdio.h>

#define MAX_WORD_LENGTH 10
#define MAX_FREQUENCY 10

#define MIN(a, b) ( (a) < (b) ? (a) : (b) )  


/* Write a program to print a histogram of the lengths of words in its input.
It is easy to draw the histogram with the bars horizontal; a vertical orientation is more challenging. */

int main(void)
{
    int i, j, c, cnt;

    int wlen[MAX_WORD_LENGTH + 1];

    for(i = 0; i <= MAX_WORD_LENGTH; wlen[i++] = 0);
   
        while( (c = getchar()) != EOF)
        {
            if(c == ' ' || c == '\n' || c == '\t')
            {
                wlen[MIN(cnt, MAX_WORD_LENGTH)]++;
                cnt = 0;
            }
            else
            {
                cnt++;
            }
        }

        printf("Histogram of the length of words in its input:\n");
    
        for(i = MAX_FREQUENCY; i >= 1; i--)
        {
            printf("\n");
            
            printf("%02d | ", i);

            for(j = 1; j <= MAX_WORD_LENGTH; j++)
                printf(wlen[j] >= i ? "\u25A0  " : "   ");
        }

        printf("\n   +");

        for(i = 1; i <= MAX_WORD_LENGTH; i++)
            printf("———");
        
        printf("\nF/L ");

        for(i = 1; i <= MAX_WORD_LENGTH; i++)
            printf("%02d ", i);

        return 0;
}