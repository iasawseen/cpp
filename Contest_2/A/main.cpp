//
// Created by ivan on 28.11.16.
//

#include <iostream>

#include <vector>
#include <fstream>
#include <cmath>
#include <limits>

using namespace std;
using int_t = int64_t;
using vec_1d = std::vector<int_t>;
using vec_2d = std::vector<vec_1d>;

int_t get_pred(vec_2d const& ancestors, vec_1d const& depth, int_t v, int_t dist) {
    if (depth[v] < dist) {
        return 0;
    } else {
        while (dist > 0) {
            size_t step = static_cast<size_t>(log2(dist));
            v = ancestors[v][step];
            dist -= pow(2, step);
        }
        return v;
    }
}

int_t get_lca(vec_2d const& ancestors, vec_1d const& depth, int_t a, int_t b) {
    if (depth[a] < depth[b]) {
        b = get_pred(ancestors, depth,  b, depth[b] - depth[a]);
    } else if (depth[a] > depth[b]){
        a = get_pred(ancestors, depth, a, depth[a] - depth[b]);
    }

    if (a == b) {
        return a;
    }

    for (int i = floor(log2(depth.size())); i >= 0; --i) {
        size_t dep = depth[a];
        if (dep < pow(2, i)) {
            continue;
        } else if (ancestors[a][i] != ancestors[b][i]) {
            a = ancestors[a][i];
            b = ancestors[b][i];
        }
    }
    return ancestors[a][0];
}

int_t min_edge_on_path_to_root(vec_2d const& ancestors, vec_2d const& weights,
               vec_1d const& depth, int_t v, int_t dist) {

    int_t min_weight = numeric_limits<int_t>::max();

    if (depth[v] < dist) {
        return numeric_limits<int_t>::max();
    } else {
        while (dist > 0) {
            size_t step = static_cast<size_t>(log2(dist));
            min_weight = min(min_weight, weights[v][step]);
            v = ancestors[v][step];
            dist -= pow(2, step);
        }
        return min_weight;
    }
}

int_t min_edge_on_path(vec_2d const& ancestors, vec_2d const& weights,
        vec_1d const& depth, int_t v1, int_t v2) {
    int_t lca = get_lca(ancestors, depth, v1, v2);

    int_t min_v1_to_lca = min_edge_on_path_to_root(ancestors, weights, depth,
                                                   v1, depth[v1] - depth[lca]);

    int_t min_v2_to_lca = min_edge_on_path_to_root(ancestors, weights, depth,
                                                   v2, depth[v2] - depth[lca]);

    return min(min_v1_to_lca, min_v2_to_lca);
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

//    freopen("input", "r", stdin);

    uint64_t N = 0;
    cin >> N;

    vec_1d depths(N + 1, 0);
    vec_1d ancestors(N + 1, 0);
    vec_1d edge_weights(N + 1, 0);

    int_t parent = 0;
    int_t weight = 0;
    int_t max_depth = 0;

    for (int_t i = 2; i <= N; ++i) {
        cin >> parent;
        cin >> weight;
        ancestors[i] = parent;
        depths[i] = depths[parent] + 1;
        max_depth = max(depths[i], max_depth);
        edge_weights[i] = weight;
    }

    uint64_t log = static_cast<uint64_t>(ceil(log2(N))) + 1;
    vec_2d dp_ancestors(N + 1, vector<int_t>(log + 1, 0));
    vec_2d dp_weights(N + 1, vector<int_t>(log + 1, 0));

    for (int_t i = 0; i <= N; ++i) {
        dp_ancestors[i][0] = ancestors[i];
        dp_weights[i][0] = edge_weights[i];
    }

    for (int_t j = 1; j <= log; ++j) {
        for (int_t i = 0; i <= N; ++i) {
            dp_ancestors[i][j] = dp_ancestors[dp_ancestors[i][j - 1]][j - 1];
            dp_weights[i][j] = min(dp_weights[i][j - 1], dp_weights[dp_ancestors[i][j - 1]][j - 1]);
        }
    }

    cin >> N;
    int_t a = 0;
    int_t b = 0;

    for (size_t i = 0; i < N; ++i) {
        cin >> a;
        cin >> b;
        cout << min_edge_on_path(dp_ancestors, dp_weights, depths, a, b) << endl;
    }

    return 0;
}