#include "sieve.h"

int sieve(int N, bool* prime) {
    for (int i = 0; i <= N; i++)
        prime[i] = true;

    prime[0] = prime[1] = false;

    for (int i = 3; i * i <= N; i += 2)
        if (prime[i]) 
            for (int j = i * i; j <= N; j += 2 * i)
                prime[j] = false;

    int count = 1;
    for (int i = 3; i <= N; i += 2)
        if (prime[i])
            count++;

    return count;
}