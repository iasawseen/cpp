#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <limits>
#include <queue>
#include <bitset>
#include <complex>

//#define TEST

using namespace std;

using int_t = int64_t;

using comp = complex<double>;

const double PI = 3.141592653589793238463;

void fft(std::vector<comp>& vec, bool inv) {
    int_t len = vec.size();
    if (len == 1) {
        return;
    }

    std::vector<comp> vec0(len / 2);
    std::vector<comp> vec1(len / 2);

    for (size_t i = 0, j = 0; i < len; i += 2, ++j) {
        vec0[j] = vec[i];
        vec1[j] = vec[i + 1];
    }

    fft(vec0, inv);
    fft(vec1, inv);

    double angle = 2 * PI / len;
    if (inv) {
        angle = -angle;
    }

    comp w(1);
    comp wn(cos(angle), sin(angle));

    for (size_t i = 0; i < len / 2; ++i) {
        vec[i] = vec0[i] + w * vec1[i];
        vec[i + len / 2] = vec0[i] - w * vec1[i];
        if (inv) {
            vec[i] /= 2;
            vec[i + len / 2] /= 2;
        }
        w *= wn;
    }
}

std::vector<int_t> multiply(std::vector<int_t> const& vec1,
                            std::vector<int_t> const& vec2) {

    std::vector<comp> f_vec1(vec1.begin(), vec1.end());
    std::vector<comp> f_vec2(vec2.begin(), vec2.end());

    size_t n = 1;

    while (n < max(f_vec1.size(), f_vec2.size())) {
        n <<= 1;
    }
    n <<= 1;

    f_vec1.resize(n);
    f_vec2.resize(n);

    fft(f_vec1, false);
    fft(f_vec2, false);

    for (size_t i = 0; i < n; ++i) {
        f_vec1[i] *= f_vec2[i];
    }

    fft(f_vec1, true);

    std::vector<int_t> result(n);

    for (size_t i = 0; i < n; ++i) {
        result[i] = (int_t) (f_vec1[i].real() + 0.5);
    }

    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

#ifdef TEST
    freopen("input", "r", stdin);
#endif

    std::string trees;
    std::vector<int_t> vec;

    cin >> trees;


    for (size_t i = 0; i < trees.size(); ++i) {
        if (trees[i] == '0') {
            vec.push_back(0);
        }
        if (trees[i] == '1') {
            vec.push_back(1);
        }
    }

    std::vector<int_t> b = multiply(vec, vec);

    size_t answer = 0;

    for (size_t i = 0; i < vec.size(); ++i) {
        if (vec[i] == 1) {
            answer += (b[2 * i] - 1) / 2;
        }
    }

    cout << answer;

    return 0;
}

