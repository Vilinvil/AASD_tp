#include "igraph.hpp"

#include <queue>

void bfs(const IGraph &graph, void (*callback)(int v)) {
    std::vector<bool> visited(graph.VerticesCount(), false);

    for (int i = 0; i < graph.VerticesCount(); i++) {
        if (!visited[i]) {

            std::queue<int> q;
            q.push(i);

            while (!q.empty()) {
                int v = q.front();
                q.pop();
                callback(v);

                std::vector<int> children = graph.GetNextVertices(v);
                for (int child : children) {
                    if (!visited[child]) {
                        visited[child] = true;
                        q.push(child);
                    }
                }
            }
        }
    }
};