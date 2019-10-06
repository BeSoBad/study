#include <iostream>
#include <vector>

using namespace std;

uint64_t toInt(string& s, int l, int r) {
    uint64_t result = 0, mult = 1;
    for (int i = r; i >= l; i--) {
        result += (s[i] - '0') * mult;
        mult *= 10;
    }
    return result;
}

uint64_t upDividend(uint64_t a, uint64_t m) {
    return a - a % m + ((a % m) ? m:0);
}

uint64_t downDividend(uint64_t a, uint64_t m) {
    return a - a % m;
}

int main() {
    string n;
    uint64_t m;
    cin >> n >> m;
    vector < uint64_t > dp(n.length(), 0);
    uint64_t mult = 10;
    dp[0] =  downDividend(toInt(n, 0, 0), m)/m -  upDividend(1, m)/m + 1;
    for (int i = 1; i < n.length(); i++) {
        uint64_t curr_num = toInt(n, 0, i);
        dp[i] = dp[i - 1] + downDividend(curr_num, m)/m -  upDividend(mult, m)/m + 1;
        mult *= 10;
    }
    if (toInt(n, 0, n.length() - 1) % m == 0)
        dp[n.length() - 1]--;
    cout << dp[n.length() - 1] << endl;
    return 0;
}
