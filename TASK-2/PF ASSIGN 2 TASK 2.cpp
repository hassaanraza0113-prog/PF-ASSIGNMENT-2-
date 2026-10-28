#include <stdio.h>

int sumProperDivisors(int number)
{
    int divisor, sum = 0;

    for (divisor = 1; divisor <= number / 2; divisor++) {
        if (number % divisor == 0)
            sum += divisor;
    }

    return sum;
}

int main(void)
{
    int n, first, second, pairCount = 0;

    printf("Enter n: ");
    scanf("%d", &n);

    if (n < 2) {
        printf("Enter a number greater than 1.\n");
        return 0;
    }

    printf("Amicable pairs up to %d:\n", n);

    for (first = 2; first <= n; first++) {
        second = sumProperDivisors(first);

        /* second > first makes each pair appear only once
           and prevents a number from pairing with itself. */
        if (second > first && second <= n &&
            sumProperDivisors(second) == first) {
            printf("(%d, %d)\n", first, second);
            pairCount++;
        }
    }

    if (pairCount == 0)
        printf("No amicable pairs found.\n");
    else
        printf("Total pairs: %d\n", pairCount);

    return 0;
}

