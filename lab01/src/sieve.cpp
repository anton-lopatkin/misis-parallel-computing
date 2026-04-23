#include "sieve.hpp"

int sieve(int N, std::vector<bool>& prime) {
    prime.assign(N + 1, true);
    prime[0] = prime[1] = false;

    for (int i = 2; (long long)i * i <= N; i++)
        if (prime[i])
            for (int j = i * i; j <= N; j += i)
                prime[j] = false;

    int count = 0;
    for (int i = 2; i <= N; i++)
        if (prime[i]) count++;

    return count;
}
