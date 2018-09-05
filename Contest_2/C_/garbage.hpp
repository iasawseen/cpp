//
// Created by ivan on 14.05.17.
//

#ifndef C_GARBAGE_HPP
#define C_GARBAGE_HPP

#include <iostream>
#include <vector>
#include <algorithm>

#define TEST


using namespace std;

using uint_t = uint64_t;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

#ifdef TEST
    freopen("input", "r", stdin);
#endif

    const uint_t base = 1019;
    const uint_t prime = 1000000007;

    std::string str;
    uint_t rows = 0;

    cin >> str >> rows;

    std::vector<uint_t> powers(str.size() + 1, 0);

    powers[1] = 1;

    for (size_t i = 2; i <= powers.size(); ++i) {
        powers[i] = powers[i - 1] * base % prime;
    }

    std::vector<uint_t> hashes(str.size() + 1, 0);

    for (size_t i = 1; i <= str.size(); ++i) {

        hashes[i] = str[i] * powers[i];
        hashes[i] %= prime;
        hashes[i] += hashes[i - 1];
        hashes[i] %= prime;
    }

    std::vector<bool> answers(rows);

    uint_t a = 0;
    uint_t b = 0;
    uint_t c = 0;
    uint_t d = 0;

    for (size_t i = 0; i < rows; ++i) {
        cin >> a >> b >> c >> d;
        --a; --b; --c; --d;

        if ((b - a) != (d - c)) {
            answers[i] = false;
        } else {
            uint_t hash1 = hashes[b + 1] - hashes[a];
            uint_t hash2 = hashes[d + 1] - hashes[c];

            if ((a <= c && (hash1 * powers[c - a + 1] % prime) == hash2) ||
                (a > c && hash1 == (hash2 * powers[a - c + 1] % prime))) {
                size_t hash1_mod = hash1 * powers[c - a + 1] % prime;
                size_t hash2_mod = hash2 * powers[a - c + 1] % prime;


                answers[i] = true;
            } else {

                size_t hash1_mod = hash1 * powers[c - a + 1] % prime;
                size_t hash2_mod = hash2 * powers[a - c + 1] % prime;
                answers[i] = false;
            }
        }
    }

    for (size_t i = 0; i < answers.size(); ++i) {
        if (answers[i]) {
            cout << "Yes" << endl;
        } else {
            cout << "No" << endl;
        }
    }

    return 0;
}


#endif //C_GARBAGE_HPP
