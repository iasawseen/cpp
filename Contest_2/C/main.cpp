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

    const uint_t base = 31;
//    const uint_t prime = 1200000127;

    std::string str;
    uint_t rows = 0;

    cin >> str >> rows;

    std::vector<uint_t> powers(str.size());
    powers[0] = 1;
    for (size_t i = 1; i < powers.size(); ++i) {
        powers[i] = powers[i - 1] * base;
    }

    std::vector<uint_t> hashes(str.size());
    for (size_t i = 0; i < str.size(); ++i) {
        hashes[i] = str[i] * powers[i];
        if (i != 0) {
            hashes[i] += hashes[i - 1];
        }
    }

    std::vector<bool> answers(rows);

    uint_t a = 0;
    uint_t b = 0;
    uint_t c = 0;
    uint_t d = 0;

    cout << "bye" << endl;

    for (size_t i = 0; i < rows; ++i) {
        cin >> a >> b >> c >> d;
        --a; --b; --c; --d;

        if ((b - a) != (d - c)) {
            cout << "hi" << endl;
            answers[i] = false;
        } else {
            uint_t hash1 = hashes[b] - hashes[a];
            uint_t hash2 = hashes[d] - hashes[c];
            cout << a << c << "powers[c - a]" << powers[c - a] << endl;
            if ((a <= c && hash1 * powers[c - a] == hash2) ||
                    (a > c && hash1 == hash2 * powers[a - c])) {
                answers[i] = true;
            } else {
                answers[i] = false;
            }
        }
    }

    cout << "mam" << endl;

    for (size_t i = 0; i < answers.size(); ++i) {
        if (answers[i]) {
            cout << "Yes" << endl;
            cout << "mem" << endl;
        } else {
            cout << "No" << endl;
        }
    }
    return 0;
}