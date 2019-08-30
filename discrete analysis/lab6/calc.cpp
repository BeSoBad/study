#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>


class TNumber {
public:
    std::vector < uint64_t > digits;
    uint64_t& operator[](int i) { 
        return digits[i]; 
    }
    int Size() const {
        return digits.size();
    }
    void Push(int a) {
        digits.push_back(a);
    }
    void Pop() {
        digits.pop_back();
    }
    static const int radix = 1e9;
    static const int radix_power = 9;
};

bool operator==(const TNumber& lhs, const TNumber& rhs) {
    if (lhs.digits.size() != rhs.digits.size()) {
        return false;
    }
    else {
        for (int i = lhs.digits.size() - 1; i >= 0; i--) {
            if (lhs.digits[i] != rhs.digits[i]) {
                return false;
            }
        } 
    }
    return true;
}


bool operator<(const TNumber& lhs, const TNumber& rhs) {
    if (lhs.digits.size() > rhs.digits.size()) {
        return false;
    }
    else if (lhs.digits.size() < rhs.digits.size()) {
        return true;
    }
    else {
        for (int i = lhs.digits.size() - 1; i >= 0; i--) {
            if (lhs.digits[i] < rhs.digits[i]) {
                return true;
            }
            else if (lhs.digits[i] > rhs.digits[i]) {
                return false;
            }
        } 
    }
    return false;
}

TNumber operator*(const TNumber& lhs, const TNumber& rhs) {
    TNumber res;
    if ((lhs.Size() == 1 && lhs.digits[0] == 0) || (rhs.Size() == 1 && rhs.digits[0] == 0)) {
        res.Push(0);
        return res;
    }
    int k, n = lhs.digits.size(), m = rhs.digits.size();
    res.digits.resize(n + m);
    for (int i = 0; i < n; i++) {
        if (lhs.digits[i] == 0) {
            continue;
        }
        k = 0;
        for (int j = 0; j < m; j++) {
            uint64_t t = lhs.digits[i] * rhs.digits[j] + res.digits[i + j] + k;
            k = t / TNumber::radix;
            res.digits[i + j] = t % TNumber::radix;
        }
        res.digits[i + m] = k;
    }
    while (res.digits.back() == 0 && res.Size() > 1) {
        res.digits.pop_back();
    }
    return res;
}


TNumber operator-(const TNumber& lhs, const TNumber& rhs) {
    TNumber res;
    if (lhs == rhs) {
        res.Push(0);
        return res;
    }
    int n = lhs.digits.size(), k = 0;
    res.digits.resize(n);
    for (int i = 0; i < n; i++) {
        int64_t tmp = lhs.digits[i] - ((i < rhs.Size()) ? rhs.digits[i]:0) - k;
        if (tmp < 0) {
            k = 1;
            res.digits[i] = TNumber::radix + tmp;
        }
        else {
            res.digits[i] = tmp;
            k = 0;
        }
    }
    return res;
}

TNumber operator/(const TNumber& lhs, const TNumber& rhs) {
    TNumber res;
    if (lhs < rhs || (lhs.Size() == 1 && lhs.digits[0] == 0)) {
        res.Push(0);
        return res;
    }
    int n = rhs.Size(), m = lhs.Size() - n, b = TNumber::radix;
    TNumber u, v, q;
    for (int i = 0; i < m + 1; i++) {
        res.Push(0);
    }
    uint64_t d = TNumber::radix / (rhs.digits[n - 1] + 1);
    if (rhs.digits.back() < TNumber::radix / 2){
        TNumber tmp;
        tmp.Push(d);
        u = lhs * tmp;
        v = rhs * tmp;
    }
    if (d == 1) {
        u = lhs;
        v = rhs;
        u.Push(0);
    }
    for (int j = m; j >= 0; j--) {
        uint64_t q_s = std::min(((unsigned long long)u[j + n] * b + u[j + n - 1]) / v[n - 1], (unsigned long long)TNumber::radix - 1);
        TNumber u_p, tn_res;
        for (int i = j; i <= j + n; i++) {
            u_p.Push(u[i]);
        }
        TNumber tmp_tn, qq;
        qq.Push(q_s);
        tmp_tn = v * qq;
        for (int i = 0; i < 2 && u_p < tmp_tn; i++) {
            q_s--;
            qq.Pop();
            qq.Push(q_s);
            tmp_tn.digits.clear();
            tmp_tn = v * qq;
        }
        tn_res = u_p - tmp_tn;
        tn_res.digits.resize(u_p.Size());
        int k = tn_res.Size() - 1;
        for (int i = j + n; i >= j; i--) {
            if (k >= 0) {
                u[i] = tn_res[k];
                k--;
            }
            else {
                break;
            }
        }
        res[j] = q_s;
    }
    return res;
}

TNumber operator^(TNumber& lhs, TNumber& rhs) {
    TNumber res;
    if (lhs.Size() == 1 && lhs.digits[0] == 0) {
        res.Push(0);
        return res;
    }
    else if (rhs.Size() == 1 && rhs.digits[0] == 0) {
        res.Push(1);
        return res;
    }
    else if (lhs.Size() == 1 && lhs.digits[0] == 1) {
        res.Push(1);
        return res;
    }
    res.Push(1);
    while (rhs.Size() != 1 || rhs.digits[0] != 0) {
        if (rhs.digits[rhs.Size() - 1] % 2) {
            TNumber tmp;
            res = res * lhs;
        }
        TNumber tmp, tmp2;
        lhs = lhs * lhs;
        TNumber two;
        two.Push(2);
        rhs = rhs / two;
    }
    return res;
}

TNumber operator+(const TNumber& lhs, const TNumber& rhs) {
    TNumber res;
    int n = std::max(lhs.digits.size(), rhs.digits.size());
    res.digits.resize(n);
    int k = 0;
    for (int i = 0; i < n; i++) {
        res.digits[i] = ((i < lhs.Size()) ? lhs.digits[i]:0) + ((i < rhs.Size()) ? rhs.digits[i]:0) + k;
        k = res.digits[i] / TNumber::radix;
        res.digits[i] = res.digits[i] % TNumber::radix;
    }
    if (k > 0) {
        res.digits.resize(n + 1);
        res.digits[n] = k;
    }
    return res;
}

void PrintNumber(TNumber& tn) {
    if (tn.Size() == 1 && tn.digits[0] == 0) {
        std::cout << 0 << std::endl;
        return;
    }
    int i = tn.digits.size();
    for (i = tn.digits.size() - 1; i > 0; i--) {
        if (tn.digits[i] != 0) {
            break;
        }
    }
    tn.digits.resize(i + 1);
    for (int i = tn.digits.size() - 1; i >= 0; i--) {
        int t = tn.digits[i];
        std::vector < int > t_v;
        while (t != 0) {
            t_v.push_back(t % 10);
            t /= 10;
        }
        if (i != tn.digits.size() - 1) {
            while (t_v.size() < TNumber::radix_power) {
                t_v.push_back(0);
            }
        }
        for (int j = t_v.size() - 1; j >= 0; j--) {
            std::cout << t_v[j];
        }
    }
    std::cout << std::endl;
}

bool operator>(const TNumber& lhs, const TNumber& rhs) {
    if (lhs.digits.size() < rhs.digits.size()) {
        return false;
    }
    else if (lhs.digits.size() > rhs.digits.size()) {
        return true;
    }
    else {
        for (int i = lhs.digits.size() - 1; i >= 0; i--) {
            if (lhs.digits[i] > rhs.digits[i]) {
                return true;
            }
            else if (lhs.digits[i] < rhs.digits[i]) {
                return false;
            }
        } 
    }
    return false;
}

void ReadNumber(TNumber& num) {
    int cnt = 0;
    std::string d;
    if(!(std::cin >> d))
        return;
    num.digits.push_back(0);
    for (int i = d.length() - 1; i >= 0; i--) {
        num.digits[num.digits.size() - 1] += (d[i] - '0') * std::pow(10, cnt);
        cnt++;
        if (cnt == TNumber::radix_power) {
            num.digits.push_back(0);
            cnt = 0;
        }
    }
    if (num.digits.back() == 0) {
        num.digits.pop_back();
    }
}

int main() {
    while (1) {
        TNumber num1, num2;
        ReadNumber(num1);
        ReadNumber(num2);
        while (num1.Size() != 0) {
            if (num1[num1.Size() - 1] == 0) {
                num1.digits.pop_back();
            }
            else
                break;
        }
        while (num2.Size() != 0) {
            if (num2[num2.Size() - 1] == 0) {
                num2.digits.pop_back();
            }
            else
                break;
        }
        if (num1.Size() == 0) {
            num1.Push(0);
        }
        if (num2.Size() == 0) {
            num2.Push(0);
        }
        char op;
        if(!(std::cin >> op)) {
            break;
        }
        if (op == '=') {
            if (num1 == num2) {
                std::cout << "true" << std::endl;
            }
            else {
                std::cout << "false" << std::endl;
            }
        }
        if (op == '<') {
            if (num1 < num2) {
                std::cout << "true" << std::endl;
            }
            else {
                std::cout << "false" << std::endl;
            }
        }
        if (op == '>') {
            if (num1 > num2) {
                std::cout << "true" << std::endl;
            }
            else {
                std::cout << "false" << std::endl;
            }
        }
        TNumber res;
        if (op == '+') {
            res = num1 + num2;
            PrintNumber(res);
        }
        if (op == '-') {
            if (num1 < num2) {
                std::cout << "Error" << std::endl;
            }
            else {
                res = num1 - num2;
                PrintNumber(res);
            }
        }
        if (op == '*') {
            res = num1 * num2;
            PrintNumber(res);
        }
        if (op == '/') {
            if (num2.Size() == 1 && num2[0] == 0) {
                std::cout << "Error" << std::endl;
            }
            else {
                res = num1 / num2;
                PrintNumber(res);
            }
        }
        if (op == '^') {
            if (num1.Size() == 1 && num1[0] == 0 && num2.Size() == 1 && num2[0] == 0) {
                std::cout << "Error" << std::endl;
            }
            else {
                res = num1 ^ num2;
                PrintNumber(res);
            }
        }
    }
    return 0;
}
