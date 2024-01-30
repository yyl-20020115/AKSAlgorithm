#include <iostream>

typedef signed long long int64;
typedef unsigned long long uint64;

#ifndef _USE_INTRINSIC
extern "C" int64 find_msb_position(uint64 value);
//global find_msb_position
//; Input
//;   rcx - uint64 to find msb
//; Output
//;   rax - msb position
//find_msb_position :
//  test rcx, rcx
//  jz is_zero
//  bsr rax, rcx
//  ret
//is_zero:
//  sxor rax, rax
//  ret
#else
int64 find_msb_position(uint64 value) {
	int64 pos = 0;
	while (value != 0) {
		value >>= 1;
		pos++;
	}
	return --pos;
}
#endif


uint64 sqrt(uint64 n)
{
	int64 shift = find_msb_position(n);
	if (shift == -1LL) return 0;
	uint64 sqrt1 = 0;
	uint64 sqrt2 = 0;
	while (shift >= 0)
	{
		uint64 s = 1LL << shift;
		sqrt2 = ((sqrt1 << 1) + s) << shift;
		if (sqrt2 <= n)
		{
			sqrt1 += s;
			n -= sqrt2;
		}
		shift--;
	}
	return sqrt1;
}
//返回两个整数，其比值为n的平方根
uint64 sqrts(uint64 n, uint64* pcr = 0)
{
	uint64 sqrt1 = sqrt(n);
	uint64 m = sqrt1 * sqrt1;
	if (n > m) {
		uint64 d = n - m;
		int64 nc = find_msb_position(d);
		if (nc > 1) {
			if (pcr != 0) *pcr = (1ULL << (nc >> 1));
			n <<= nc;
			sqrt1 = sqrt(n);
		}
	}
	return sqrt1;
}


static bool is_prime_test(uint64 n) {
	if (n <= 1) return false;
	if (n == 2) return true;
	for (uint64 i = 2; i * i <= n; i++) {
		if (n % i == 0)return false;
	}
	return true;
}

// 求最大公约数的辗转相除法
static uint64 gcd(uint64 a, uint64 b) {
	return b == 0 ? a : gcd(b, a % b);
}

// 快速幂取模算法
// a^n mod m = (a mod m)^n mod m
static uint64 fast_pow_mod(uint64 a, uint64 n, uint64 m) {
	uint64 res = 1;
	a %= m;
	while (n > 0) {
		if (n & 1) {
			res = res * a % m;
		}
		a = a * a % m;
		n >>= 1;
	}
	return res;
}

// 判断一个数是否为素数
static bool is_prime(uint64 n) {
	if (n <= 1) return false;
	if (n <= 3) return true;
	if (n % 2 == 0 || n % 3 == 0) return false;

	uint64 r = sqrt(n);
	//快速跳过
	for (uint64 i = 5; i <= r; i += 6) {
		if (n % i == 0 || n % (i + 2) == 0) {
			return false;
		}
	}
	return true;
}

// 判断n是否是a的幂
static bool n_is_power_of(uint64 n, uint64* pa = 0) {
	if (n <= 1) {
		return true;
	}
	uint64 r = sqrt(n);
	for (uint64 a = 2; a <= r; ++a) {
		uint64 p = a;
		while (p <= n) {
			p *= a;
			if (p == n) {
				if (pa != 0) *pa = a;
				return true;
			}
		}
	}
	return false;
}
//1-20之间单独判断，20以上再做AKS处理，因为20是-10~+10区间（10~20是1~10的负数区间）
// AKS算法判断n是否为素数
static bool AKS(uint64 n) {
	//1~20
	if (n == 2
		|| n == 3
		|| n == 5
		|| n == 7
		|| n == 11
		|| n == 13
		|| n == 17
		|| n == 19) return true;
	if (n <= 1
		|| n % 2 == 0
		|| n % 3 == 0
		|| n % 5 == 0
		|| n % 7 == 0
		|| n % 11 == 0
		|| n % 13 == 0
		|| n % 17 == 0
		|| n % 19 == 0) return false;

	// 如果n是a的幂，则n不是素数
	if (n_is_power_of(n)) return false;

	// 寻找r，满足n^r-1 = (n-1)q，其中q为素数
	uint64 r = 2;
	uint64 s = sqrt(n);
	while (r <= s) {
		if (n % r == 0)
			return false;

		if (gcd(n, r) == 1) {
			bool is_valid = true;
			uint64 phi_r = r - 1;
			uint64 sqrt_phi_r = sqrt(phi_r);
			for (uint64 a = 2; a <= sqrt_phi_r && is_valid; ++a) {
				if (gcd(a, r) == 1) {
					is_valid = (fast_pow_mod(a, phi_r, n) != 1);
				}
			}
			if (is_valid) {
				uint64 q = (n - 1) / r;
				if (gcd(q, r) == 1 && fast_pow_mod(n, q, r) == 1) {
					return true;
				}
			}
		}
		++r;
	}
	return false;
}

int main() {
	//uint64 u = sqrt(0ULL);
	//for (uint64 i = 0; i <= 100; i++) {
	//	uint64 r = sqrt(i);
	//	std::cout << "i=" << i << ",sqrt=" << r << std::endl;
	//}
	//uint64 n = 31;
	//uint64 cr = 0;
	//uint64 r = sqrts(n, &cr);
	//double u = r / (double)cr;

	while (true) {
		uint64 n = 0;
		std::cout << "input an integer(-1 to exit):";
		std::cin >> n;


		if (n == -1) break;
		if (AKS(n)) {
			std::cout << "Y:" << n << std::endl;
		}
		else {
			std::cout << "N:" << n << std::endl;
		}
	}
	return 0;
}
