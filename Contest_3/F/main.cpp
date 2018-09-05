#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>

//#define TEST

using namespace std;

using int_t = int64_t;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

#ifdef TEST
    freopen("input", "r", stdin);
#endif

    std::string str;

    std::getline(cin, str);
    str += '\n';

    std::vector<int_t> suff_array(str.size() - 1, 0);

    std::iota(suff_array.begin(), suff_array.end(), 0);

    std::sort(suff_array.begin(), suff_array.end(),
              [&](int_t i, int_t j) {
                  while ((i < str.size() && j < str.size()) && str[i] == str[j]) {
                      ++i;
                      ++j;
                  };
                  if (i >= str.size()) {
                      return true;
                  }
                  if (j >= str.size()) {
                      return false;
                  }
                  return str[i] < str[j];
              });

    std::for_each(suff_array.begin(), suff_array.end(),
             [](int_t index){cout << index + 1 << " ";});

    return 0;
}