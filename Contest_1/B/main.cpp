#include <iostream>
#include <vector>
#include <cmath>
#include <limits>


using namespace std;
using int_t = int64_t;
using uint_t = uint64_t;
using double_t = double;

double_t infinity = numeric_limits<double_t>::infinity();

struct city {
    city(int_t x, int_t y)
            : x(x), y(y)
    {}

    city() : x(0), y(0) {}


    city(city const& other) {
        x = other.x;
        y = other.y;
    }

    int_t x;
    int_t y;
};

struct cities {
    cities(uint_t size)
            : cities_(size),
              visited_(size, false),
              min_distance_(size, infinity)
    {}

    void insert_city(uint_t position, city new_city) {
        cities_[position] = new_city;
    }

    double_t get_distance(uint_t pos1, uint_t pos2) {
        return std::sqrt(std::pow((cities_[pos1].x - cities_[pos2].x), 2) +
                                 std::pow((cities_[pos1].y - cities_[pos2].y), 2));
    }

    void visited(uint_t pos) {
        visited_[pos] = true;
    }

    bool is_visited(uint_t pos) {
        return visited_[pos];
    }

    double_t get_min_dist(uint_t pos) {
        return min_distance_[pos];
    }

    void update_min_dist(uint_t pos, double_t new_dist) {
        min_distance_[pos] = new_dist;
    }


    vector<city> cities_;
    vector<bool> visited_;
    vector<double_t> min_distance_;
};

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.precision(20);

//    freopen("input", "r", stdin);

    uint_t N = 0;
    cin >> N;
    cities city_arr(N);
    int_t x = 0;
    int_t y = 0;

    for (uint_t i = 0; i < N; ++i) {
        cin >> x >> y;
        city_arr.insert_city(i, city(x, y));
    }

    double_t sum = 0;

    city_arr.update_min_dist(0, 0);
    city_arr.visited(0);

    for (uint_t j = 0; j < N; ++j) {
        if (!city_arr.is_visited(j)) {
            double_t dist = city_arr.get_distance(0, j);

            if (dist + city_arr.get_min_dist(0) < city_arr.get_min_dist(j)) {
                city_arr.update_min_dist(j, dist + city_arr.get_min_dist(0));
            }
        }
    }

    for (uint_t i = 1; i < N; ++i) {
        uint_t next_city = 0;
        double_t min_dist = infinity;

        for (uint_t j = 0; j < N; ++j) {
            if (!city_arr.is_visited(j) &&
                    (city_arr.get_min_dist(j) < min_dist)) {
                next_city = j;
                min_dist = city_arr.get_min_dist(j);
            }
        }

        sum += min_dist;
        city_arr.visited(next_city);

        double_t dist = infinity;

        for (uint_t j = 0; j < N; ++j) {
            if (!city_arr.is_visited(j)) {
                dist = city_arr.get_distance(next_city, j);

                if (dist < city_arr.get_min_dist(j)) {
                    city_arr.update_min_dist(j, dist);
                }
            }
        }
    }

    cout << sum << endl;

    return 0;
}