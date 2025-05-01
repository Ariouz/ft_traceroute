#include "ft_ping.h"

double ft_pow(double base, int exp)
{
    if (exp == 0)
        return 1.0;

    double result = 1.0;
    int negative = 0;

    if (exp < 0)
    {
        negative = 1;
        exp = -exp;
    }

    while (exp)
    {
        if (exp % 2 == 1)
            result *= base;
        base *= base;
        exp /= 2;
    }

    if (negative)
        return 1.0 / result;
    else
        return result;
}


double ft_sqrt(double n)
{
    if (n < 0)
        return -1;
    if (n == 0 || n == 1)
        return n;

    double x = n;
    double res = 0.0;
    double precision = 1e-10;

    while (x - res > precision)
    {
        res = x;
        x = 0.5 * (x + n / x);
    }
    return x;
}
