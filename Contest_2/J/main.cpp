#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <limits>
#include <queue>

//#define TEST

using namespace std;

using int_t = int64_t;

bool has_chered_path(int_t ride,
                     std::vector<vector<int_t>>& graph,
                     std::vector<int_t>& match,
                     std::vector<bool>& visited) {

    if (visited[ride]) {
        return false;
    }

    visited[ride] = true;

    for (int_t i = 0; i < graph[ride].size(); ++i) {
        int_t next = graph[ride][i];
        if (match[next] == -1 || has_chered_path(match[next], graph, match, visited)) {
            match[next] = ride;
            return true;
        }
    }

    return false;
}

//bool can_be_


struct ride {
    ride(int_t a, int_t b, int_t c, int_t d, int_t start, int_t end)
            : a(a), b(b), c(c), d(d), start(start), end(end)
    {}

    int_t a;
    int_t b;
    int_t c;
    int_t d;
    int_t start;
    int_t end;
};


int_t get_time_between(ride& r1, ride& r2) {
    return std::abs(r1.c - r2.a) + std::abs(r1.d - r2.b);
}


int main() {
    ios_base::sync_with_stdio(false);
//    cin.tie(nullptr);

#ifdef TEST
    freopen("input", "r", stdin);
#endif

    size_t m = 0;

    cin >> m;

    std::vector<vector<int_t>> can_follow(m, std::vector<int_t>());
    std::vector<ride> rides;

    std::string time;
    int_t a;
    int_t b;
    int_t c;
    int_t d;
    int_t hours;
    int_t start_time;
    int_t end_time;

    for (size_t i = 0; i < m; ++i) {
        cin >> time >> a >> b >> c >> d;
        hours = std::stoi(time.substr(0, 2));
        start_time = hours * 60 + std::stoi(time.substr(3, 5));
        end_time = start_time + std::abs(a - c) + std::abs(b - d);
//        cout << start_time << " " << end_time << endl;
        rides.emplace_back(a, b, c, d, start_time, end_time);
    }

    for (int_t i = 0; i < m; ++i) {
        for (int_t j = i + 1; j < m; ++j) {

//            cout << "i: " << i << " j: " << j << " " << get_time_between(rides[i], rides[j]) << endl;

            if ((rides[i].end + get_time_between(rides[i], rides[j])) < rides[j].start) {
                can_follow[i].push_back(j);
//                can_follow[i][j] = true;
            }
        }
    }

    std::vector<int_t> match(m, -1);
    std::vector<bool> visited(m, false);

    int_t taxies = m;

    for (int_t r = 0; r < m; ++r) {
        std::fill(visited.begin(), visited.end(), false);
        if (has_chered_path(r, can_follow, match, visited)) {
            taxies -= 1;
        }
    }

    cout << taxies << endl;

    return 0;
}