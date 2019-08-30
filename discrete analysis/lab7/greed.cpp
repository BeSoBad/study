#include <iostream>
#include <vector>

int main() {
    long long n, p;
    std::cin >> n >> p;
    std::vector < long long > exchange(n);
    std::vector < long long > count_denom(n);
    long long tmp = 1;
    for (int i = 0; i < n; i++) {
        exchange[i] = tmp;
        tmp *= p;
    }
    long long m;
    std::cin >> m;
    while (m != 0) {
        long long curr;
        for (int i = n - 1; i >= 0; i--) {
            if (exchange[i] <= m) {
                curr = exchange[i];
                count_denom[i] += m / curr;
                m -= (m / curr) * curr;
                break;
            }
        }
    }
    for (int i = 0; i < n; i++) {
        std::cout << count_denom[i] << std::endl;
    }
}
