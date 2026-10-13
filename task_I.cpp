#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

std::vector<std::vector<int>> mul(const std::vector<std::vector<int>>& A, const std::vector<std::vector<int>>& B, int mod) {
    int S = A.size();
    std::vector<std::vector<int>> C(S, std::vector<int>(S, 0));
    for (int i = 0; i < S; ++i) {
        for (int j = 0; j < S; ++j) {
            long long sum = 0;
            for (int k = 0; k < S; ++k) {
                sum += static_cast<long long>(A[i][k] * B[k][j]);
            }
            C[i][j] = sum % mod;
        }
    }
    return C;
}

std::string subtract_one(std::string s) {
    int i = s.length() - 1;
    while (i >= 0 && s[i] == '0') {
        s[i] = '9';
        --i;
    }
    if (i >= 0) s[i]--;
    int pos = s.find_first_not_of('0');
    if (pos == std::string::npos) return "0";
    return s.substr(pos);
}

std::vector<int> to_binary(std::string s) {
    std::vector<int> bits;
    if (s == "0") return bits;
    while (s != "0") {
        int rem = 0;
        for (int i = 0; i < (int)s.length(); ++i) {
            int digit = (s[i] - '0') + rem * 10;
            s[i] = (digit / 2) + '0';
            rem = digit % 2;
        }
        bits.push_back(rem);
        int pos = s.find_first_not_of('0');
        if (pos == std::string::npos) s = "0";
        else s = s.substr(pos);
    }
    return bits;
}

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::string n_str;
    int m = 0, mod = 0;
    std::cin >> n_str >> m >> mod;

    if (n_str == "1") {
        int ans = (1 << m) % mod;
        std::cout << ans << '\n';
        return 0;
    }

    int S = 1 << m;

    std::vector<std::vector<int>> T(S, std::vector<int>(S, 0));
    for (int a = 0; a < S; ++a) {
        for (int b = 0; b < S; ++b) {
            bool ok = true;
            for (int j = 0; j + 1 < m; ++j) {
                int a0 = (a >> j) & 1;
                int a1 = (a >> (j + 1)) & 1;
                int b0 = (b >> j) & 1;
                int b1 = (b >> (j + 1)) & 1;
                if (a0 == a1 && a1 == b0 && b0 == b1) {
                    ok = false;
                    break;
                }
            }
            T[a][b] = ok ? 1 : 0;
        }
    }

    std::string e_str = subtract_one(n_str);
    std::vector<int> bits = to_binary(e_str);

    std::vector<std::vector<int>> R(S, std::vector<int>(S, 0));
    for (int i = 0; i < S; ++i) R[i][i] = 1;
    std::vector<std::vector<int>> B = T;

    for (int bit : bits) {
        if (bit) {
            R = mul(R, B, mod);
        }
        B = mul(B, B, mod);
    }

    long long total = 0;
    for (int i = 0; i < S; ++i) {
        for (int j = 0; j < S; ++j) {
            total = (total + R[i][j]) % mod;
        }
    }

    std::cout << total << '\n';
    return 0;
}
