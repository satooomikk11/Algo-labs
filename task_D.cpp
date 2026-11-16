#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

struct Edge {
    int from;
    int to;
    int weight;
};

class DisjointSetUnion {
public:
    explicit DisjointSetUnion(int n)
        : parent_(n + 1), size_(n + 1, 1) {
        for (int i = 1; i <= n; ++i) {
            parent_[i] = i;
        }
    }

    int Find(int v) {
        if (parent_[v] == v) {
            return v;
        }
        parent_[v] = Find(parent_[v]);
        return parent_[v];
    }

    bool Unite(int a, int b) {
        int rootA = Find(a);
        int rootB = Find(b);

        if (rootA == rootB) {
            return false;
        }

        if (size_[rootA] < size_[rootB]) {
            std::swap(rootA, rootB);
        }

        parent_[rootB] = rootA;
        size_[rootA] += size_[rootB];

        return true;
    }

private:
    std::vector<int> parent_;
    std::vector<int> size_;
};

struct TreeEdge {
    int to;
    int weight;
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    int k = 0;
    std::cin >> n >> m >> k;

    std::vector<Edge> edges;
    edges.reserve(m);

    for (int i = 0; i < m; ++i) {
        int from = 0;
        int to = 0;
        int weight = 0;
        std::cin >> from >> to >> weight;
        edges.push_back({from, to, weight});
    }

    std::sort(edges.begin(), edges.end(),
              [](const Edge& left, const Edge& right) {
                  return left.weight > right.weight;
              });

    DisjointSetUnion dsu(n);
    std::vector<std::vector<TreeEdge>> tree(n + 1);

    int added = 0;

    for (const Edge& edge : edges) {
        if (dsu.Unite(edge.from, edge.to)) {
            tree[edge.from].push_back({edge.to, edge.weight});
            tree[edge.to].push_back({edge.from, edge.weight});
            ++added;

            if (added == n - 1) {
                break;
            }
        }
    }

    int log = 1;
    while ((1 << log) <= n) {
        ++log;
    }

    const int INF = std::numeric_limits<int>::max();

    std::vector<int> depth(n + 1, 0);
    std::vector<int> parent(n + 1, 1);
    std::vector<int> parentWeight(n + 1, INF);
    std::vector<char> visited(n + 1, false);

    std::vector<int> stack;
    stack.push_back(1);
    visited[1] = true;
    parent[1] = 1;
    parentWeight[1] = INF;

    while (!stack.empty()) {
        int v = stack.back();
        stack.pop_back();

        for (const TreeEdge& edge : tree[v]) {
            int to = edge.to;

            if (visited[to]) {
                continue;
            }

            visited[to] = true;
            parent[to] = v;
            parentWeight[to] = edge.weight;
            depth[to] = depth[v] + 1;
            stack.push_back(to);
        }
    }

    std::vector<std::vector<int>> up(log, std::vector<int>(n + 1, 1));
    std::vector<std::vector<int>> minEdge(log, std::vector<int>(n + 1, INF));

    for (int v = 1; v <= n; ++v) {
        up[0][v] = parent[v];
        minEdge[0][v] = parentWeight[v];
    }

    for (int level = 1; level < log; ++level) {
        for (int v = 1; v <= n; ++v) {
            int middle = up[level - 1][v];

            up[level][v] = up[level - 1][middle];
            minEdge[level][v] = std::min(
                minEdge[level - 1][v],
                minEdge[level - 1][middle]
            );
        }
    }

    auto maxBottleneck = [&](int a, int b) -> int {
        if (a == b) {
            return 0;
        }

        int answer = INF;

        if (depth[a] < depth[b]) {
            std::swap(a, b);
        }

        int diff = depth[a] - depth[b];

        for (int level = 0; level < log; ++level) {
            if ((diff & (1 << level)) != 0) {
                answer = std::min(answer, minEdge[level][a]);
                a = up[level][a];
            }
        }

        if (a == b) {
            return answer;
        }

        for (int level = log - 1; level >= 0; --level) {
            if (up[level][a] != up[level][b]) {
                answer = std::min(answer, minEdge[level][a]);
                answer = std::min(answer, minEdge[level][b]);

                a = up[level][a];
                b = up[level][b];
            }
        }

        answer = std::min(answer, minEdge[0][a]);
        answer = std::min(answer, minEdge[0][b]);

        return answer;
    };

    for (int query = 0; query < k; ++query) {
        int a = 0;
        int b = 0;
        std::cin >> a >> b;

        std::cout << maxBottleneck(a, b) << '\n';
    }

    return 0;
}
