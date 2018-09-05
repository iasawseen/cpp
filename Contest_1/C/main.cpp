#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

using uint_t = uint64_t;
using edge_t = std::pair<uint_t, std::pair<uint_t, uint_t>>;

struct graph {
    graph(uint_t V, uint_t E)
            : v(V), e(E), edges(E), parents(V, 0)
    {}

    void add_edge(uint_t i, uint_t b, uint_t e, uint_t w) {
        edges[i] = make_pair(w, make_pair(b, e));
    }

    uint_t find_set(uint_t v) {
        if (v == parents[v]) {
            return v;
        }
        return find_set(parents[v]);
    }

    void union_sets(uint_t a, uint_t b) {
        a = find_set(a);
        b = find_set(b);
        if (a != b) {
            parents[b] = a;
        }
    }

    uint_t get_min_weight() {

        for (uint_t i = 0; i < v; ++i) {
            parents[i] = i;
        }

        std::sort(edges.begin(), edges.end());

        uint_t weight = 0;

        for (uint_t i = 0; i < e; ++i) {
            uint_t a = edges[i].second.first;
            uint_t b = edges[i].second.second;
            uint_t w = edges[i].first;

            if (find_set(a) != find_set(b)) {
                weight += w;
                union_sets(a, b);
            }
        }
        return weight;
    }

private:
    uint_t v;
    uint_t e;
    vector<edge_t> edges;
    vector<uint_t> parents;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

//    freopen("input", "r", stdin);

    uint_t V = 0;
    uint_t E = 0;
    cin >> V >> E;

    graph grh(V, E);

    uint_t begin;
    uint_t end;
    uint_t weight;

    for (uint_t i = 0; i < E; ++i) {
        cin >> begin >> end >> weight;
        grh.add_edge(i, begin - 1, end - 1, weight);
    }

    cout << grh.get_min_weight();

    return 0;
}