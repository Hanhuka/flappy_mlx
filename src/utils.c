#include "../flappy.h"

static int	num_size(int n)
{
	int	size;

	size = 0;
	if (n < 0)
	{
		size++;
		n *= -1;
	}
	while (n > 0)
	{
		n /= 10;
		size++;
	}
	return (size);
}

static char	*converter(int n, char *result, int size)
{
	if (n < 0)
	{
		result[0] = '-';
		n *= -1;
	}
	result[size - 1] = '\0';
	while (n > 0)
	{
		result[size - 2] = n % 10 + '0';
		n /= 10;
		size--;
	}
	return (result);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		num;
	int		size;

	num = n;
	if (n == -2147483648)
		return (strdup("-2147483648"));
	if (n == -0)
		return (strdup("0"));
	size = (num_size(num) + 1);
	result = malloc((size) * 1);
	if (!result)
		return (0);
	return (converter(n, result, size));
}