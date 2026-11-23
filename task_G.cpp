#include <algorithm>
#include <array>
#include <cstdint>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

struct Node {
    std::uint64_t board;
    int parent;
    int g;
    int h;
    int zero;
    char move;
};

struct QueueItem {
    int f;
    int h;
    int g;
    int id;
};

struct Compare {
    bool operator()(const QueueItem& a, const QueueItem& b) const {
        if (a.f != b.f) {
            return a.f > b.f;
        }
        return a.h > b.h;
    }
};

struct Solver {
    std::array<std::array<int, 4>, 16> neighbors{};
    std::array<std::array<char, 4>, 16> moves{};
    std::array<int, 16> degree{};
    std::array<std::vector<unsigned char>, 3> patterns;

    static int transpose(int pos) {
        return (pos % 4) * 4 + pos / 4;
    }

    void buildNeighbors() {
        for (int i = 0; i < 16; ++i) {
            if (i >= 4) {
                neighbors[i][degree[i]] = i - 4;
                moves[i][degree[i]++] = 'D';
            }
            if (i < 12) {
                neighbors[i][degree[i]] = i + 4;
                moves[i][degree[i]++] = 'U';
            }
            if (i % 4 > 0) {
                neighbors[i][degree[i]] = i - 1;
                moves[i][degree[i]++] = 'R';
            }
            if (i % 4 < 3) {
                neighbors[i][degree[i]] = i + 1;
                moves[i][degree[i]++] = 'L';
            }
        }
    }

    void buildPatterns() {
        for (int group = 0; group < 3; ++group) {
            patterns[group].assign(1u << 24, 255);
            std::uint32_t target = 15;
            for (int i = 0; i < 5; ++i) {
                int tile = group * 5 + i + 1;
                target |= static_cast<std::uint32_t>(tile - 1)
                          << (4 * (i + 1));
            }

            std::deque<std::uint32_t> queue;
            queue.push_back(target);
            patterns[group][target] = 0;

            while (!queue.empty()) {
                std::uint32_t current = queue.front();
                queue.pop_front();
                int zero = current & 15u;
                int distance = patterns[group][current];

                for (int i = 0; i < degree[zero]; ++i) {
                    int nextZero = neighbors[zero][i];
                    std::uint32_t next = (current & ~15u) |
                        static_cast<std::uint32_t>(nextZero);
                    int cost = 0;

                    for (int j = 1; j <= 5; ++j) {
                        int shift = 4 * j;
                        if (((current >> shift) & 15u) ==
                            static_cast<std::uint32_t>(nextZero)) {
                            next = (next & ~(15u << shift)) |
                                (static_cast<std::uint32_t>(zero) << shift);
                            cost = 1;
                            break;
                        }
                    }

                    int nextDistance = distance + cost;
                    if (nextDistance < patterns[group][next]) {
                        patterns[group][next] =
                            static_cast<unsigned char>(nextDistance);
                        if (cost == 0) {
                            queue.push_front(next);
                        } else {
                            queue.push_back(next);
                        }
                    }
                }
            }
        }
    }

    int heuristic(std::uint64_t board, int zero) const {
        std::array<std::uint32_t, 3> state{};
        std::array<std::uint32_t, 3> transposedState{};
        int manhattan = 0;

        for (int group = 0; group < 3; ++group) {
            state[group] = zero;
            transposedState[group] = transpose(zero);
        }

        for (int pos = 0; pos < 16; ++pos) {
            int tile = (board >> (4 * pos)) & 15u;
            if (tile == 0) {
                continue;
            }

            int target = tile - 1;
            manhattan += std::abs(pos / 4 - target / 4) +
                         std::abs(pos % 4 - target % 4);

            int group = target / 5;
            int shift = 4 * (target % 5 + 1);
            state[group] |= static_cast<std::uint32_t>(pos) << shift;

            int mapped = transpose(target);
            int transposedGroup = mapped / 5;
            int transposedShift = 4 * (mapped % 5 + 1);
            transposedState[transposedGroup] |=
                static_cast<std::uint32_t>(transpose(pos))
                << transposedShift;
        }

        int h = 0;
        int transposedH = 0;
        for (int group = 0; group < 3; ++group) {
            h += patterns[group][state[group]];
            transposedH += patterns[group][transposedState[group]];
        }
        return std::max({h, transposedH, manhattan});
    }

    static std::string restore(const std::vector<Node>& nodes, int id) {
        std::string answer;
        while (nodes[id].parent != -1) {
            answer.push_back(nodes[id].move);
            id = nodes[id].parent;
        }
        std::reverse(answer.begin(), answer.end());
        return answer;
    }

    std::string solve(std::uint64_t start, int startZero) const {
        const std::uint64_t goal = 0x0FEDCBA987654321ULL;
        std::vector<Node> nodes;
        nodes.reserve(100000);
        std::unordered_map<std::uint64_t, int> best;
        best.reserve(100000);
        std::priority_queue<QueueItem,
                            std::vector<QueueItem>, Compare> queue;

        int startH = heuristic(start, startZero);
        nodes.push_back({start, -1, 0, startH, startZero, ' '});
        best[start] = 0;
        queue.push({startH, startH, 0, 0});

        while (!queue.empty()) {
            QueueItem item = queue.top();
            queue.pop();
            Node current = nodes[item.id];
            if (item.g != current.g) {
                continue;
            }
            if (current.board == goal) {
                return restore(nodes, item.id);
            }

            for (int i = 0; i < degree[current.zero]; ++i) {
                int nextZero = neighbors[current.zero][i];
                int tile = (current.board >> (4 * nextZero)) & 15u;
                
                int nextG = current.g + 1;
                
                std::uint64_t nextBoard = current.board ^
                    (static_cast<std::uint64_t>(tile) << (4 * current.zero)) ^
                    (static_cast<std::uint64_t>(tile) << (4 * nextZero));

                auto it = best.find(nextBoard);
                int nextId;
                int nextH;

                if (it != best.end()) {
                    nextId = it->second;
                    
                    if (nextG >= nodes[nextId].g) {
                        continue;
                    }
                    nodes[nextId].g = nextG;
                    nodes[nextId].parent = item.id;
                    nodes[nextId].move = moves[current.zero][i];
                    nextH = nodes[nextId].h;
                } else {
                    nextH = heuristic(nextBoard, nextZero);
                    nextId = static_cast<int>(nodes.size());
                    nodes.push_back({nextBoard, item.id, nextG, nextH,
                                     nextZero, moves[current.zero][i]});
                    best[nextBoard] = nextId;
                }
                queue.push({nextG + nextH, nextH, nextG, nextId});
            }
        }
        return "";
    }
};

int main() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::uint64_t start = 0;
    int zero = 0;
    int inversions = 0;
    std::array<int, 16> board{};

    for (int i = 0; i < 16; ++i) {
        std::cin >> board[i];
        start |= static_cast<std::uint64_t>(board[i]) << (4 * i);
        if (board[i] == 0) {
            zero = i;
        }
        for (int j = 0; j < i; ++j) {
            if (board[i] != 0 && board[j] != 0 &&
                board[j] > board[i]) {
                ++inversions;
            }
        }
    }

    
    if ((inversions + zero / 4) % 2 == 0) {
        std::cout << -1 << '\n';
        return 0;
    }

    Solver solver;
    solver.buildNeighbors();
    solver.buildPatterns();

    std::string answer = solver.solve(start, zero);
    if (answer.empty() && start != 0x0FEDCBA987654321ULL) {
        std::cout << -1 << '\n';
    } else {
        std::cout << answer.size() << '\n';
        std::cout << answer << '\n';
    }
    return 0;
}
