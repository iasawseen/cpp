#include <iostream>
#include <vector>
#include <algorithm>

//#define TEST

using namespace std;

using uint_t = int64_t;

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

    std::vector<uint_t> powers(str.size(), 0);

    powers[0] = 1;

    for (size_t i = 1; i < powers.size(); ++i) {
        powers[i] = powers[i - 1] * base % prime;
    }

    std::vector<uint_t> hashes(str.size() + 1, 0);

    for (size_t i = 1; i <= str.size(); ++i) {
        hashes[i] = (str[i - 1] * powers[i - 1]) % prime;
        if (i != 0) {
            hashes[i] += hashes[i - 1];
            hashes[i] %= prime;
        }
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

            uint_t hash1 = ((hashes[b + 1] - hashes[a]) + prime) % prime;
            uint_t hash2 = ((hashes[d + 1] - hashes[c]) + prime) % prime;

            if ((a <= c && ((hash1 * powers[c - a]) % prime) == hash2) ||
                (a > c && hash1 == ((hash2 * powers[a - c])) % prime)) {
                answers[i] = true;
            } else {
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