#include "arc_graph.hpp"

#include <utility>

namespace a_graph {
ArcGraph::ArcGraph(int vertices_count) : vertices_count_(vertices_count){};

ArcGraph::ArcGraph(const IGraph &igraph) {
    vertices_count_ = igraph.VerticesCount();
    for (int i = 0; i < vertices_count_; i++) {
        auto next_vec = igraph.GetNextVertices(i);
        for (int next : next_vec) {
            AddEdge(i, next);
        }
    }
};

void ArcGraph::AddEdge(int from, int to) {
    graph_.push_back(std::pair<int, int>(from, to));
};

int ArcGraph::VerticesCount() const { return vertices_count_; };

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;
    for (auto pair : graph_) {
        if (pair.first == vertex) {
            result.push_back(pair.second);
        }
    }

    return result;
};

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;
    for (auto pair : graph_) {
        if (pair.second == vertex) {
            result.push_back(pair.first);
        }
    }

    return result;
};
}   // namespace a_graph
