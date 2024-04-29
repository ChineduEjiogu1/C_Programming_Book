#include <stdio.h>

#define LOWER 0
#define UPPER 300
#define STEP 20

float fahr_to_celsius(float fahr);

int main(void)
{
    int fahr = LOWER;

    printf("Fahrenheit Celsius\n");

    while (fahr <= UPPER)
    {
        printf("%10d %7.1f\n", fahr, fahr_to_celsius(fahr));
        fahr = fahr + STEP;
    }

    return 0;
}

float fahr_to_celsius(float fahr)
{
    return (5.0 / 9.0) * (fahr - 32.0);
}