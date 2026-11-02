#include <iostream>
#include <vector>

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int N, M;
    std::cin >> N >> M;

    std::vector<std::vector<int>> graph(N + 1);
    std::vector<std::vector<int>> reversed(N + 1);

    for (int i = 0; i < M; ++i) {
        int from, to;
        std::cin >> from >> to;

        graph[from].push_back(to);
        reversed[to].push_back(from);
    }

    std::vector<bool> used(N + 1, false);
    std::vector<int> nextEdge(N + 1, 0);
    std::vector<int> order;
    order.reserve(N);

    for (int start = 1; start <= N; ++start) {
        if (used[start]) {
            continue;
        }

        std::vector<int> stack;
        stack.push_back(start);
        used[start] = true;

        while (!stack.empty()) {
            int v = stack.back();

            if (nextEdge[v] == static_cast<int>(graph[v].size())) {
                order.push_back(v);
                stack.pop_back();
                continue;
            }

            int to = graph[v][nextEdge[v]++];

            if (!used[to]) {
                used[to] = true;
                stack.push_back(to);
            }
        }
    }

    std::vector<int> component(N + 1, 0);
    int componentCount = 0;

    for (int i = static_cast<int>(order.size()) - 1; i >= 0; --i) {
        int start = order[i];

        if (component[start] != 0) {
            continue;
        }

        ++componentCount;

        std::vector<int> stack;
        stack.push_back(start);
        component[start] = componentCount;

        while (!stack.empty()) {
            int v = stack.back();
            stack.pop_back();

            for (int to : reversed[v]) {
                if (component[to] == 0) {
                    component[to] = componentCount;
                    stack.push_back(to);
                }
            }
        }
    }

    std::cout << componentCount << '\n';

    for (int v = 1; v <= N; ++v) {
        if (v > 1) {
            std::cout << ' ';
        }
        std::cout << component[v];
    }
    std::cout << '\n';

    return 0;
}
