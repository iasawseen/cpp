#include <iostream>
#include <vector>
#include <algorithm>

//#define TEST

using namespace std;

using int_t = int64_t;

std::vector<int_t> get_z(std::string const& str) {
    size_t len = str.size();
    vector<int_t> z_array(len, 0);

    int_t left = 0;
    int_t right = 0;

    for (int_t i = 1; i < len; ++i) {
        if (i <= right) {
            z_array[i] = std::min(z_array[i - left], right - i + 1);
        }
        while (((i + z_array[i]) < len) && (str[z_array[i]] == str[i + z_array[i]])) {
            ++z_array[i];
        }
        if ((i + z_array[i] - 1) > right) {
            left = i;
            right = i + z_array[i] - 1;
        }
    }

    return z_array;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

#ifdef TEST
    freopen("input", "r", stdin);
#endif

    std::string str;
    cin >> str;
    size_t init_len = str.size();

    str = str + str;

    std::vector<int_t> z_array = get_z(str);

    size_t result = 1;

    for (size_t i = 0; i < init_len; ++i) {
        if (z_array[i] >= init_len) {
            continue;
        }
        if (str[i + z_array[i]] < str[z_array[i]]) {
            ++result;
        }
    }
    cout << result;

    return 0;
}