#include "set_graph.hpp"

namespace s_graph {
SetGraph::SetGraph(int vertices_count) : vertices_count_(vertices_count) {
    graph_.resize(vertices_count);
};

SetGraph::SetGraph(const IGraph &igraph) {
    graph_.resize(igraph.VerticesCount());
    vertices_count_ = igraph.VerticesCount();
    for (int i = 0; i < vertices_count_; i++) {
        auto next_vec = igraph.GetNextVertices(i);
        for (int next : next_vec) {
            AddEdge(i, next);
        }
    }
};

void SetGraph::AddEdge(int from, int to) { graph_[from].insert(to); };

int SetGraph::VerticesCount() const { return vertices_count_; };

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;
    result.reserve(graph_[vertex].size());
    for (int next : graph_[vertex]) {
        result.push_back(next);
    }

    return result;
};

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;
    for (int i = 0; i < graph_.size(); i++) {
        // i == vertex оставил, т.к. мб есть петля
        if (auto it = graph_[i].find(vertex); it != graph_[i].end()) {
            result.push_back(i);
        }
    }

    return result;
};
}   // namespace s_graph
