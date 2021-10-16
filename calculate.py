import re

def prime_divisors(n : int):
    """
        Find all prime divisors of n
    """
    primes = {}

    for p in range(2, n+1):
        while n % p == 0:
            n = n // p
            primes[p] = 1 if p not in primes else primes[p] + 1
        
        if n == 1:
            return primes

for line in open('cores.txt', 'r', encoding='utf-8'):
    phi_x, x = re.findall(r"(\d+) +(\d+)->", line)[0]
    phi_x, x = int(phi_x), int(x)

    print(f"{x}\t{prime_divisors(x)}\t->\t{phi_x}\t{prime_divisors(phi_x)}")