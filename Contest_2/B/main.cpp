#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <limits>
#include <queue>
#include <bitset>

//#define TEST

using namespace std;

using int_t = int64_t;

const int_t N = 200;

bool solve_slau(std::vector<std::bitset<N + 1>> mat,
                size_t rows, size_t cols, std::bitset<N>& result) {
    std::vector<int_t> mapping(cols, -1);

    for (size_t column = 0, row = 0; column < cols && row < rows; ++column) {
        for (size_t i = row; i < rows; ++i) {
            if (mat[i][column]) {
                std::swap(mat[i], mat[row]);
                break;
            }
        }

        if (!mat[row][column]) {
            continue;
        }

        mapping[column] = row;
        for (size_t i = 0; i < rows; ++i) {
            if (i != row && mat[i][column]) {
                mat[i] ^= mat[row];
            }
        }
        ++row;
    }

    for (size_t i = 0; i < cols; ++i) {
        if (mapping[i] != -1) {
            result[i] = mat[mapping[i]][cols] / mat[mapping[i]][i];
        }
    }

    for (size_t i = 0; i < rows; ++i) {
        size_t summy = 0;
        for (size_t j = 0; j < cols; ++j) {
            summy += result[j] * mat[i][j];
        }
        summy %= 2;
        if ((summy - mat[i][cols]) != 0) {
            return false;
        }
    }

    return true;

}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

#ifdef TEST
    freopen("input", "r", stdin);
#endif

    size_t size = 0;
    size_t dependent = 0;
    size_t index = 0;
    bool bit = false;

    cin >> size;

    std::vector<std::bitset<N + 1>> mat(size);
    std::bitset<N> result;

    for (size_t i = 0; i < size; ++i) {
        cin >> dependent;
        for (size_t j = 0; j < dependent; ++j) {
            cin >> index;
            mat[index - 1][i] = true;
        }
    }

    for (size_t i = 0; i < size; ++i) {
        cin >> bit;
        mat[i][size] = bit;
    }

    bool solved = solve_slau(mat, size, size, result);

    if (!solved) {
        result = std::bitset<N>();
        for (size_t i = 0; i < size; ++i) {
            mat[i][size] = 1 ^ mat[i][size];
        }
        solved = solve_slau(mat, size, size, result);
    }

    if (!solved) {
        cout << -1 << endl;
        return 0;
    }

    std::vector<size_t> results;

    size_t L = 0;

    for (size_t i = 0; i < result.size(); ++i) {
        if (result[i]) {
            results.push_back(i + 1);
            ++L;
        }
    }

    cout << L << endl;

    for (auto click : results) {
        cout << click << " ";
    }
    return 0;
}

