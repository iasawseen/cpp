#include <iostream>
#include <vector>
#include <algorithm>

//#define TEST

using namespace std;

using int_t = int64_t;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

#ifdef TEST
    freopen("input", "r", stdin);
#endif

    const int_t base = 1019;
    const int_t prime = 1000000007;

    std::string str;
    std::string pattern;
    std::vector<int_t> results;

    cin >> str >> pattern;

    int_t str_hash = str[0];
    int_t pattern_hash = pattern[0];
    int_t power = 1;

    for (size_t i = 1; i < pattern.size(); ++i) {
        power = (power * base) % prime;
        str_hash = (str_hash * base + str[i]) % prime;
        pattern_hash = (pattern_hash * base + pattern[i]) % prime;
    }

    if (str_hash == pattern_hash) {
        results.push_back(1);
    }

    for (size_t i = pattern.size(); i < str.size(); ++i) {
        str_hash -= (str[i - pattern.size()] * power) % prime;
        str_hash += prime;
        str_hash %= prime;
        str_hash = (str_hash * base + str[i]) % prime;

        if (str_hash == pattern_hash) {
            results.push_back(i + 2 - pattern.size());
        }

    }

    cout << results.size() << endl;
    for (auto item : results) {
        cout << item << " ";
    }

    return 0;
}