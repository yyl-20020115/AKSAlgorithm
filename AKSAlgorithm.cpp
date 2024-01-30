#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
//AKS algorithm for testing if a number is prime
typedef unsigned long long uint64;

static uint64 gcd(uint64 n, uint64 r)
{
	return r == 0 ? n : gcd(r, n % r);
}

static bool check_power(uint64 n)
{
	for (double i = 2; i <= ceil(log(n) / log(2)); i++)
	{
		double p = pow(n, 1.0 / i);
		if (p - floor(p) == 0)
			return true;
	}
	return false;
}
static bool test(uint64 r)
{
	uint64 i = 2;
	while (i <= sqrt(r)) {
		if (i % r == 0)
			return false;
		i++;
	}
	return true;
}

static uint64 syz(uint64 r) {
	uint64 i = 2;
	while (i <= sqrt(r)) {
		if (i % r != 0)
			i++;
		else 
			r /= i;
	}
	return r;
}
static uint64 pf(uint64 a, uint64 b, uint64 r) {
	uint64 z = 1;
	while (b != 0)
	{
		if (b % 2 == 0)
		{
			b >>= 1;
			a = (a * a) % r;
		}
		else
		{
			b --;
			z = (z * a) % r;
		}
	}
	return z;
}

static uint64 mods(uint64 n, uint64 r, uint64 a)
{
	int x = 5;//
	uint64 f = 1, g = x * a, y = n, h; 
	while (y != 0)
	{
		if (y % 2 == 0)
		{
			y >>= 1;
			h = g * g;
			g = h % (uint64)(pow(x, r) - 1);
		}
		else
		{
			y --;
			h = f * g;
			f = h % (uint64)(pow(x, r) - 1);
		}
	}
	return f;
}
int main1()
{
	uint64 a = 1;
	int x = 5;
	uint64 n; 
	printf("please input integer n:"); 
	scanf("%lu", &n);
	if (n < 2)
	{
		printf("n should be greater or equals to 2\n");
		return 0;
	}
	if (check_power(n))
	{
		printf("%lu is a composite number!\n", n);
		return 0;
	}
	else //false
	{
		uint64 r = 2;
		while (r < n)
		{
			if (gcd(n, r) != 1)
			{
				printf("%lu is a composite number\n", n);
				return 0;
			}
			else if (test(r))
			{
				uint64 q = syz(r - 1);
				if (q >= 4 * sqrt(r) * (uint64)(log(n) / log(2)) && pf(n, (r - 1) / q, r) != 1)
					break;
			}
			r ++;
		}
		for (a = 1; a <= 2 * sqrt(r) * ((uint64)(log(n) / log(2))); a++)
		{
			if (mods(n, r, a) != (((uint64)pow(x, n) - a) % ((uint64)pow(x, r) - 1)))
			{
				printf("%lu is a prime number\n", n);
				return 0;
			}
		}
		printf("%lu is a composite number\n", n);
	}
	return 0;
}
