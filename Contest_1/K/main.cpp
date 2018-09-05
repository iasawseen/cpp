#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <numeric>
#include <set>

using namespace std;
using int_t = uint64_t;

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    std::map<string, string> person_to_city;
    std::map<string, int_t> person_fortune;
    using pair = std::pair<int_t, string>;
    auto comp = []
            (pair const& pair1, pair const& pair2) {
                if (pair1.first == pair2.first) {
                    return pair1.second > pair2.second;
                }
                return pair1.first > pair2.first ;
    };

    std::set<std::pair<int_t, string>, decltype(comp)> money_to_city(comp);
    std::map<string, int_t> city_to_money;

    freopen("input", "r", stdin);

    int_t n = 0;
    cin >> n;
    string name;
    string new_city;
    int_t fortune = 0;

    for (size_t i = 0; i < n; ++i) {
        cin >> name;
        cin >> new_city;
        cin >> fortune;

        person_to_city.insert(make_pair(name, new_city));
        person_fortune.insert(make_pair(name, fortune));

        int_t old_value = 0;
        old_value = city_to_money[new_city];
        city_to_money[new_city] += fortune;

        money_to_city.erase(make_pair(old_value, new_city));
        money_to_city.insert(make_pair(old_value + fortune, new_city));

    }


    int_t days = 0;
    int_t days_with_info = 0;

    std::map<string, int_t> richests;

    cin >> days;
    cin >> days_with_info;
    int day = -1;

    cin >> day;
    cin >> name;
    cin >> new_city;
    int_t counter = 1;

    for (int i = 1; i < days + 1; ++i) {

        auto first_it = std::begin(money_to_city);
        int_t first_max = (*first_it).first;
        string city_leader = (*first_it).second;
        ++first_it;

        if (first_it == money_to_city.end() || first_max > (*first_it).first) {
            richests[city_leader] += 1;
        }

        while (i == day && counter <= days_with_info) {
            string old_city = person_to_city[name];

            int_t old_money = city_to_money[old_city];
            city_to_money[old_city] -= person_fortune[name];
            money_to_city.erase(make_pair(old_money, old_city));
            money_to_city.insert(make_pair(old_money - person_fortune[name], old_city));

            old_money = city_to_money[new_city];
            city_to_money[new_city] += person_fortune[name];
            money_to_city.erase(make_pair(old_money, new_city));
            money_to_city.insert(make_pair(old_money + person_fortune[name], new_city));
            person_to_city[name] = new_city;

            cin >> day;
            cin >> name;
            cin >> new_city;
            ++counter;
        }
    }

    for(auto& rich : richests) {
        cout << rich.first << " " << rich.second << endl;
    }

}
