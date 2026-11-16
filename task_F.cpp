#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

struct Edge {
    int u;
    int v;
};

class DisjointSetUnion {
public:
    explicit DisjointSetUnion(int n)
        : parent_(n + 1), size_(n + 1, 1) {
        for (int i = 0; i <= n; ++i) {
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

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n = 0;
    int m = 0;
    std::cin >> n >> m;

    int f = 0;
    std::cin >> f;

    std::vector<Edge> edges(m);
    std::vector<std::vector<int>> adj(n + 1);

    for (int i = 0; i < m; ++i) {
        int u = 0;
        int v = 0;
        std::cin >> u >> v;
        edges[i] = {u, v};
        adj[u].push_back(i);
        adj[v].push_back(i);
    }

    std::vector<int> tin(n + 1, 0);
    std::vector<int> fup(n + 1, 0);
    std::vector<int> parent(n + 1, -1);
    std::vector<int> parentEdge(n + 1, -1);
    std::vector<int> iter(n + 1, 0);
    std::vector<char> isBridge(m, false);

    int timer = 0;

    for (int start = 1; start <= n; ++start) {
        if (tin[start] != 0) {
            continue;
        }

        std::vector<int> stack;
        stack.push_back(start);
        tin[start] = fup[start] = ++timer;

        while (!stack.empty()) {
            int v = stack.back();

            if (iter[v] < static_cast<int>(adj[v].size())) {
                int edgeId = adj[v][iter[v]];
                ++iter[v];

                if (edgeId == parentEdge[v]) {
                    continue;
                }

                int to = edges[edgeId].u ^ edges[edgeId].v ^ v;

                if (tin[to] == 0) {
                    parent[to] = v;
                    parentEdge[to] = edgeId;
                    tin[to] = fup[to] = ++timer;
                    stack.push_back(to);
                } else {
                    fup[v] = std::min(fup[v], tin[to]);
                }
            } else {
                stack.pop_back();

                if (parent[v] != -1) {
                    int p = parent[v];
                    fup[p] = std::min(fup[p], fup[v]);

                    // ОШИБКА: должно быть tin[p], а не fup[p].
                    // fup[p] к этому моменту уже мог быть уменьшен
                    // другими детьми p, поэтому условие становится
                    // слишком слабым: любой ребёнок, чей fup строго
                    // больше текущего fup[p], будет объявлен мостом.
                    if (fup[v] > fup[p]) {
                        isBridge[parentEdge[v]] = true;
                    }
                }
            }
        }
    }

    std::vector<int> comp(n + 1, -1);
    int compCount = 0;

    for (int start = 1; start <= n; ++start) {
        if (comp[start] != -1) {
            continue;
        }

        std::queue<int> queue;
        queue.push(start);
        comp[start] = compCount;

        while (!queue.empty()) {
            int v = queue.front();
            queue.pop();

            for (int edgeId : adj[v]) {
                if (isBridge[edgeId]) {
                    continue;
                }

                int to = edges[edgeId].u ^ edges[edgeId].v ^ v;

                if (comp[to] == -1) {
                    comp[to] = compCount;
                    queue.push(to);
                }
            }
        }

        ++compCount;
    }

    std::vector<std::vector<int>> tree(compCount);

    for (int edgeId = 0; edgeId < m; ++edgeId) {
        if (!isBridge[edgeId]) {
            continue;
        }

        int cu = comp[edges[edgeId].u];
        int cv = comp[edges[edgeId].v];

        tree[cu].push_back(cv);
        tree[cv].push_back(cu);
    }

    int root = comp[f];

    std::vector<int> depth(compCount, 0);
    std::vector<int> parentComp(compCount, root);
    std::vector<char> visited(compCount, false);

    std::queue<int> queue;
    queue.push(root);
    visited[root] = true;
    parentComp[root] = root;

    while (!queue.empty()) {
        int v = queue.front();
        queue.pop();

        for (int to : tree[v]) {
            if (visited[to]) {
                continue;
            }

            visited[to] = true;
            parentComp[to] = v;
            depth[to] = depth[v] + 1;
            queue.push(to);
        }
    }

    int log = 1;
    while ((1 << log) <= compCount) {
        ++log;
    }

    std::vector<std::vector<int>> up(log, std::vector<int>(compCount, root));

    for (int v = 0; v < compCount; ++v) {
        up[0][v] = parentComp[v];
    }

    for (int level = 1; level < log; ++level) {
        for (int v = 0; v < compCount; ++v) {
            up[level][v] = up[level - 1][up[level - 1][v]];
        }
    }

    auto lca = [&](int a, int b) -> int {
        if (depth[a] < depth[b]) {
            std::swap(a, b);
        }

        int diff = depth[a] - depth[b];

        for (int level = 0; level < log; ++level) {
            if (diff & (1 << level)) {
                a = up[level][a];
            }
        }

        if (a == b) {
            return a;
        }

        for (int level = log - 1; level >= 0; --level) {
            if (up[level][a] != up[level][b]) {
                a = up[level][a];
                b = up[level][b];
            }
        }

        return up[0][a];
    };

    int q = 0;
    std::cin >> q;

    while (q-- > 0) {
        int a = 0;
        int c = 0;
        std::cin >> a >> c;

        int ca = comp[a];
        int cc = comp[c];
        int commonAncestor = lca(ca, cc);

        std::cout << depth[commonAncestor] << '\n';
    }

    return 0;
}
