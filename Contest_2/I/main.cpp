#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <limits>
#include <queue>
#include <bitset>
#include <complex>

#define TEST

using namespace std;

using int_t = int64_t;

using comp = complex<double>;

const double PI = 3.141592653589793238463;

void advanced_fft(std::vector<comp>& vec, bool inv) {
    int_t num = (int_t) vec.size();

    for (size_t i = 1, j = 0; i < num; ++i) {
        int_t bit = num >> 1;
        for (; j >= bit; bit >>= 1) {
            j -= bit;
        }
        j += bit;
        if (i < j) {
            std::swap(vec[i], vec[j]);
        }
    }

    for (int_t len = 2; len <= num; len <<= 1) {
        double angle = 2 * PI / len;
        if (inv) {
            angle = -angle;
        }
        comp w_num(cos(angle), sin(angle));

        for (size_t i = 0; i < num; i += len) {
            comp w(1);
            for (size_t j = 0; j < len / 2; ++j) {
                comp u = vec[i + j];
                comp v = vec[i + j + len / 2] * w;
                vec[i + j] = u + v;
                vec[i + j + len / 2] = u - v;
                w *= w_num;

            }
        }
    }

    if (inv) {
        for (size_t i = 0; i < num; ++i) {
            vec[i] /= num;
        }
    }
}

std::vector<comp> forward_fft(std::vector<int_t> const& vec1,
                              std::vector<int_t> const& vec2, int_t& size) {
    std::vector<comp> f_vec1(vec1.begin(), vec1.end());
    std::vector<comp> f_vec2(vec2.begin(), vec2.end());

    size_t n = 1;

    while (n < max(f_vec1.size(), f_vec2.size())) {
        n <<= 1;
    }
    n <<= 1;

    f_vec1.resize(n);
    f_vec2.resize(n);

    advanced_fft(f_vec1, false);
    advanced_fft(f_vec2, false);

    for (size_t i = 0; i < n; ++i) {
        f_vec1[i] *= f_vec2[i];
    }

    size = n;

    return f_vec1;
}

std::vector<int_t> reverse_fft(std::vector<comp> fft_vec, int_t n) {
    advanced_fft(fft_vec, true);

    std::vector<int_t> result(n);

    for (size_t i = 0; i < n; ++i) {
        result[i] = (int_t) (fft_vec[i].real() + 0.5);
    }

    return result;
}

std::vector<int_t> transform_first(std::string const& first, char nucleotide) {
    std::vector<int_t> vec(first.size(), 0);
    for (size_t i = 0; i < first.size(); ++i) {
        if (first[i] == nucleotide) {
            vec[i] = 1;
        }
    }

    reverse(vec.begin(), vec.end());
    vec.resize(first.size() * 2);

    return vec;
}

std::vector<int_t> transform_second(std::string const& second, char nucleotide) {
    std::vector<int_t> vec(second.size() * 2, 0);

    for (size_t i = 0; i < second.size(); ++i) {
        if (second[i] == nucleotide) {
            vec[i] = 1;
            vec[i + second.size()] = 1;
        }
    }

    return vec;
}

std::vector<comp> get_fft(std::string const& dna1,
                          std::string const& dna2,
                          char nucleotide, int_t& size) {
    std::vector<int_t> first = transform_first(dna1, nucleotide);
    std::vector<int_t> second = transform_second(dna2, nucleotide);

    return forward_fft(first, second, size);
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

#ifdef TEST
    freopen("input", "r", stdin);
#endif

    size_t M = 0;
    std::string dna1;
    std::string dna2;

    cin >> M >> dna1 >> dna2;

    int_t size = 0;
    std::vector<comp> f_a = get_fft(dna1, dna2, 'A', size);
    std::vector<comp> f_c = get_fft(dna1, dna2, 'C', size);
    std::vector<comp> f_g = get_fft(dna1, dna2, 'G', size);
    std::vector<comp> f_t = get_fft(dna1, dna2, 'T', size);

    for (size_t i = 0; i < size; ++i) {
        f_a[i] += (f_c[i] + f_g[i] + f_t[i]);
    }

    std::vector<int_t> result = reverse_fft(f_a, size);

    result = std::vector<int_t>(result.begin() + dna1.size(), result.begin() + dna1.size() * 2);

    size_t shift = 0;
    size_t max_val = 0;

    for (int_t i = M - 1; i >= 0; --i) {
        if (result[i] > max_val) {
            shift = i;
            max_val = result[i];
        }
    }

    cout << max_val << " " << (M - shift - 1);

    return 0;
}


