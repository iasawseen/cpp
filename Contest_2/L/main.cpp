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

    uint_t num = 0;
    cin >> num;

    std::vector<uint_t> results(num + 1, 0);
    results[1] = 1;

    uint_t max_diff = 0;
    uint_t cur_diff = 0;
    uint_t x = 0;
    uint_t y = 0;

    for (size_t i = 2; i < results.size() - 1; ++i) {
        results[i] = (num - (num / i) * results[num % i] % num) % num;
        if (results[i] > i) {
            cur_diff = results[i] - i;
        } else {
            cur_diff = i - results[i];
        }

        if (results[i] <= 0) {
            continue;
        }

        if (cur_diff > max_diff) {
            max_diff = cur_diff;
            x = i;
            y = results[i];
        }

    }


    if (max_diff == 0) {
        cout << "1 1" << endl;
    } else {
        cout << x << " " << y << endl;
    }

    return 0;
}