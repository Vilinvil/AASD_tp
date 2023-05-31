#include "matrix_graph.hpp"

namespace m_graph {
MatrixGraph::MatrixGraph(int verticeces_count)
    : vertices_count_(verticeces_count) {
    graph_.resize(verticeces_count);
    for (int i = 0; i < verticeces_count; i++) {
        graph_[i].resize(verticeces_count);
    }
};

MatrixGraph::MatrixGraph(const IGraph &igraph) {
    graph_.resize(igraph.VerticesCount());
    vertices_count_ = igraph.VerticesCount();
    for (int i = 0; i < vertices_count_; i++) {
        graph_[i].resize(vertices_count_);
        auto next_vec = igraph.GetNextVertices(i);
        for (int next : next_vec) {
            AddEdge(i, next);
        }
    }
};

void MatrixGraph::AddEdge(int from, int to) { graph_[from][to] = 1; };

int MatrixGraph::VerticesCount() const { return vertices_count_; };

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;
    for (int i = 0; i < vertices_count_; i++) {
        if (graph_[vertex][i] == 1) {
            result.push_back(i);
        }
    }

    return result;
};

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;
    for (int i = 0; i < vertices_count_; i++) {
        if (graph_[i][vertex] == 1) {
            result.push_back(i);
        }
    }

    return result;
};
}   // namespace m_graph
