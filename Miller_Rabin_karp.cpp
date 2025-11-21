/*
===============================================
   MILLER–RABIN PRIMALITY TEST (C++ Version)
===============================================

★ Why Miller–Rabin is probabilistic?
------------------------------------
For any odd composite number n:

    At most 1/4 of all bases a ∈ [2, n−2]
    will falsely say "n is prime".

    That means at least 3/4 bases...
    will correctly detect n as composite.

This is a mathematical theorem based on
the factorization structure of composite numbers.


★ Worst-case error for ONE base:
--------------------------------
P(wrong) ≤ 1/4

So the accuracy for one base is:

    P(correct) = 1 − 1/4 = 3/4 = 0.75


★ If we test k independent bases:
----------------------------------
Probability of being wrong for ALL bases is:

    (1/4)^k

Probability of correctness is:

    P(correct) = 1 − (1/4)^k


★ Accuracy Table:
-----------------
k = 1:
    1 − 1/4        = 0.75

k = 2:
    1 − 1/16       = 0.9375

k = 3:
    1 − 1/64       = 0.984375

k = 4:
    1 − 1/256      = 0.99609375

k = 5:
    1 − 1/1024     = 0.9990234   (> 99.9% accuracy)

k = 8:
    1 − 1/65536    = 0.9999847   (~ 99.998%)

This is why Miller–Rabin is used for RSA & DH.
Even a few bases give extremely high accuracy.

===============================================
                IMPLEMENTATION
===============================================
*/

#include <bits/stdc++.h>
using namespace std;

// Safe multiplication (a*b) % mod using __int128
long long mulmod(long long a, long long b, long long mod) {
    __int128 res = (__int128)a * b;
    return (long long)(res % mod);
}

// Fast exponentiation: (a^b) % mod
long long modexp(long long a, long long b, long long mod) {
    long long result = 1;
    while(b > 0) {
        if(b & 1)
            result = mulmod(result, a, mod);
        a = mulmod(a, a, mod);
        b >>= 1;
    }
    return result;
}

// Performs the Miller–Rabin test for one base 'a'
bool check_composite(long long a, long long d, long long n, int s) {
    long long x = modexp(a, d, n);

    // First check
    if(x == 1 || x == n - 1)
        return false; // n MAY be prime (pass)

    // Repeated squaring
    for(int r = 1; r < s; r++) {
        x = mulmod(x, x, n);

        if(x == n - 1)
            return false;  // n MAY be prime (pass)

        if(x == 1)
            return true;   // definitely composite
    }

    return true; // never hit -1 → composite
}

// Miller–Rabin primality test
bool isPrime(long long n) {
    if(n < 2) return false;
    if(n % 2 == 0) return (n == 2);

    // Write n−1 = 2^s * d with d odd
    long long d = n - 1;
    int s = 0;
    while((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    // Deterministic bases for 64-bit integers
    long long test_bases[] = {2, 3, 5, 7, 11, 13, 17};

    for(long long a : test_bases) {
        if(a >= n) continue;
        if(check_composite(a, d, n, s))
            return false; // definitely composite
    }

    return true; // probably prime (with >99.999% accuracy)
}

int main() {
    long long n;
    cout << "Enter a number: ";
    cin >> n;

    if(isPrime(n))
        cout << n << " is PRIME\n";
    else
        cout << n << " is COMPOSITE\n";

    return 0;
}
