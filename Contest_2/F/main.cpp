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

    std::string name;
    int_t value;
    std::unordered_map<std::string, int_t> mapa;

    while(cin >> name >> value) {
        auto iter = mapa.find(name);
        if (iter == mapa.end()) {
            mapa[name] = value;
        } else {
            mapa[name] += value;
        }
        cout << mapa[name] << endl;
    }

    return 0;
}