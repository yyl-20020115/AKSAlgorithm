#include <iostream>
#include <cmath>

typedef long long ll;

// �����Լ����շת�����
static ll gcd(ll a, ll b) {
    return b == 0 ? a : gcd(b, a % b);
}

// ������ȡģ�㷨
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

// �ж�һ�����Ƿ�Ϊ����
static bool is_prime(ll n) {
    if (n <= 1) {
        return false;
    }
    if (n <= 3) {
        return true;
    }
    if (n % 2 == 0 || n % 3 == 0) {
        return false;
    }
    ll r = sqrt(n);
    //��������
    for (ll i = 5; i <= r; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

// �ж�n�Ƿ���a����
static bool n_is_power_of(ll n, ll* pa = 0) {
    if (n <= 1) {
        return true;
    }
    ll r = sqrt(n);
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

// AKS�㷨�ж�n�Ƿ�Ϊ����
static bool AKS(ll n) {
    // ���n��a���ݣ���n��������
    if (n_is_power_of(n)) return false;

    // Ѱ��r������n^r-1 = (n-1)q������qΪ����
    ll r = 2;
    ll s = sqrt(n);
    while (r <= s) {
        if (n % r == 0)
            return false;

        if (gcd(n, r) == 1) {
            bool is_valid = true;
            ll phi_r = r - 1;
            ll sqrt_phi_r = sqrt(phi_r);
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
    ll n;
    std::cin >> n;
    if (AKS(n)) {
        std::cout << n << " is prime" << std::endl;
    }
    else {
        std::cout << n << " is not prime" << std::endl;
    }
    return 0;
}
