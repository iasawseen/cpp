#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <limits>
#include <queue>

#define TEST

using namespace std;

using int_t = int64_t;


bool bfs(int_t source, int_t sink,
         std::vector<vector<int_t>>& graph,
         std::vector<int_t>& parents,
         std::vector<bool>& visited) {

    std::fill(parents.begin(), parents.end(), -1);
    std::fill(visited.begin(), visited.end(), false);

    std::queue<int_t> queue;
    queue.push(source);
    visited[source] = true;

    while(!queue.empty()) {
        int_t vertex = queue.front();
        queue.pop();

        for (int_t u = 0; u < parents.size(); ++u) {
            if (!visited[u] && graph[vertex][u] > 0) {
                queue.push(u);
                parents[u] = vertex;
                visited[u] = true;
            }
        }
    }

    return visited[sink];
}


int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

#ifdef TEST
    freopen("input", "r", stdin);
#endif

    std::unordered_map<int_t, int_t> mapa;

    size_t n = 0;
    size_t m = 0;

    size_t from = 0;
    size_t to = 0;
    size_t capacity = 0;

    cin >> n >> m;

    std::vector<vector<int_t>> edges(n, std::vector<int_t>(n, -1));
    std::vector<vector<int_t>> initial_graph(n, std::vector<int_t>(n, -1));
    std::vector<vector<int_t>> graph(n, std::vector<int_t>(n, -1));

    for (size_t i = 0; i < m; ++i) {
        cin >> from >> to >> capacity;
        edges[from - 1][to - 1] = i;
        edges[to - 1][from - 1] = i;
        initial_graph[from - 1][to - 1] = capacity;
        initial_graph[to - 1][from - 1] = capacity;
        graph[from - 1][to - 1] = capacity;
        graph[to - 1][from - 1] = capacity;
    }

    int_t source = 0;
    int_t sink = n - 1;

    std::vector<int_t> parents(n, -1);

    int_t v = 0;
    int_t u = 0;

    std::vector<bool> visited(n, false);

    while(bfs(source, sink, graph, parents, visited)) {
        int_t flow = std::numeric_limits<int_t>::max();

        v = sink;
        while (v != source) {
            u = parents[v];
            flow = std::min(flow, graph[u][v]);
            v = parents[v];
        }

        v = sink;
        while (v != source) {
            u = parents[v];
            graph[u][v] -= flow;
            graph[v][u] += flow;
            v = parents[v];
        }
    }

    std::vector<int_t> results;
    size_t flow = 0;

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (visited[i] && !visited[j] && initial_graph[i][j] > 0) {
                results.push_back(edges[i][j]);
                flow += initial_graph[i][j];
            }
        }
    }

    std::sort(results.begin(), results.end());

    cout << results.size() << " " << flow << endl;
    for (auto edge : results) {
        cout << (edge + 1) << " ";
    }


    return 0;
}