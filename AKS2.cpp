#include <iostream>

typedef long long ll;
#ifdef _USE_INTRINSIC
extern "C" ll find_msb_position(ll value);
#else
ll find_msb_position(ll value) {
	ll pos = 0;

	// 如果num为0，直接返回0
	if (value == 0)
		return 0;

	// 计算最高位的位置
	while (value != 0) {
		value = value >> 1;
		pos++;
	}
	return --pos;
}
#endif

static bool is_prime_test(ll n) {
	if (n <= 1) return false;
	if (n == 2) return true;
	for (ll i = 2; i * i <= n; i++) {
		if (n % i == 0)return false;
	}
	return true;
}

static ll sqrt_bv(ll n)
{
	ll sqrt1 = 0;
	ll shift = find_msb_position(n); //最高位的位置

	ll sqrt2 = 0;
	while (shift >= 0)
	{
		ll s = 1LL << shift;
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

// 求最大公约数的辗转相除法
static ll gcd(ll a, ll b) {
	return b == 0 ? a : gcd(b, a % b);
}

// 快速幂取模算法
// a^n mod m = (a mod m)^n mod m
static ll fast_pow_mod(ll a, ll n, ll m) {
	ll res = 1;
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
static bool is_prime(ll n) {
	if (n <= 1) return false;
	if (n <= 3) return true;
	if (n % 2 == 0 || n % 3 == 0) return false;

	ll r = sqrt_bv(n);
	//快速跳过
	for (ll i = 5; i <= r; i += 6) {
		if (n % i == 0 || n % (i + 2) == 0) {
			return false;
		}
	}
	return true;
}

// 判断n是否是a的幂
static bool n_is_power_of(ll n, ll* pa = 0) {
	if (n <= 1) {
		return true;
	}
	ll r = sqrt_bv(n);
	for (ll a = 2; a <= r; ++a) {
		ll p = a;
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
static bool AKS(ll n) {
	//1~20
	if (n == 2 
		|| n == 3 
		|| n == 5 
		|| n==7 
		|| n==11
		|| n==13 
		|| n == 17 
		|| n == 19) return true;
	if (n <= 1
		||n % 2 == 0 
		|| n % 3 == 0 
		|| n % 5 == 0 
		|| n % 7 == 0
		|| n % 11 == 0
		|| n % 13 == 0
		|| n % 17 ==0
		|| n % 19 == 0) return false;

	// 如果n是a的幂，则n不是素数
	if (n_is_power_of(n)) return false;

	// 寻找r，满足n^r-1 = (n-1)q，其中q为素数
	ll r = 2;
	ll s = sqrt_bv(n);
	while (r <= s) {
		if (n % r == 0)
			return false;

		if (gcd(n, r) == 1) {
			bool is_valid = true;
			ll phi_r = r - 1;
			ll sqrt_phi_r = sqrt_bv(phi_r);
			for (ll a = 2; a <= sqrt_phi_r && is_valid; ++a) {
				if (gcd(a, r) == 1) {
					is_valid = (fast_pow_mod(a, phi_r, n) != 1);
				}
			}
			if (is_valid) {
				ll q = (n - 1) / r;
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
	while (true) {
		ll n = 0LL;
		std::cout << "input an integer(-1 to exit):";
		std::cin >> n;
		if (n == -1LL) break;
		if (AKS(n)) {
			std::cout << "Y:" << n << std::endl;
		}
		else {
			std::cout << "N:" << n << std::endl;
		}
	}
	return 0;
}
