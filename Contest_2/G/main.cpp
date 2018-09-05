#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

#define TEST

using namespace std;

using int_t = int64_t;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

#ifdef TEST
    freopen("input", "r", stdin);
#endif

    std::unordered_map<int_t, int_t> mapa;

    size_t num = 0;
    int_t a = 0;
    int_t b = 0;

    cin >> num;

    for (size_t i = 0; i < num; ++i) {
        cin >> a >> b;

        auto iter_a = mapa.find(a);
        if (iter_a == mapa.end()) {
            mapa[a] = a;
        }

        auto iter_b = mapa.find(b);
        if (iter_b == mapa.end()) {
            mapa[b] = b;
        }

        int_t tmp = mapa[a];
        mapa[a] = mapa[b];
        mapa[b] = tmp;
        cout << std::abs(mapa[a] - mapa[b]) << endl;

    }

    return 0;
}