#include "common.hpp"

#include <limits.h>
#include <stdbool.h>

static bool is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\v' || c == '\f');
}

static void skip_whitespace(const char **str)
{
    while (is_space(**str))
        (*str)++;
}

static int handle_sign(const char **str)
{
    int sign = 1;

    if (**str == '-' || **str == '+')
    {
        if (**str == '-')
            sign = -1;
        (*str)++;
    }
    return sign;
}

static bool check_limits(long long result, int digit, int sign)
{
    const long long max_abs_limit = (long long)INT_MAX;
    const long long min_abs_limit = (long long)INT_MIN * -1;
    long long limit;
    
    if (sign == 1)
        limit = max_abs_limit;
    else
        limit = min_abs_limit;
    if (result > limit / 10)
        return true;
    if (result == limit / 10 && digit > limit % 10)
        return true;
    return false;
}

int ft_atoi(const char *str)
{
    long long result;
    int sign;
    int digit;

    result = 0;
    skip_whitespace(&str);
    sign = handle_sign(&str);
    while (*str >= '0' && *str <= '9')
    {
        digit = *str - '0';
        if (check_limits(result, digit, sign))
        {
            if (sign == 1)
                return INT_MAX;
            else
                return INT_MIN;
        }
        result = result * 10 + digit;
        str++;
    }
    return (int)(result * sign);
}
