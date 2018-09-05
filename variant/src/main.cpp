#include <iostream>
#include <vector>
#include "variant.h"

using namespace std;


void test1() {
    std::vector<int> vec{1, 4, 56, 7, 23, 5, 7, 5, 7, 2, 4, 1, 2, 3};

    au::variant<std::vector<int>, double> var(vec);

    au::variant<std::vector<int>, double> var2 = var;

    au::variant<std::vector<int>, double> var3;
    var3 = var2;

    for (auto item : au::get<0>(var3)) {
        cout << item << endl;
    }

    var3 = 5.6;

    cout << au::get<1>(var3) << endl;

    au::variant<std::vector<int>, double> var4(std::move(var));

    for (auto item : au::get<std::vector<int>>(var4)) {
        cout << item << endl;
    }

}


int main() {

    test1();

    return 0;
}