#include "list_graph.hpp"

ListGraph::ListGraph(int vertices_count) : vertices_count_(vertices_count) {
    graph_.resize(vertices_count);
    // reverse_graph_.resize(vertices_count);
};

ListGraph::ListGraph(const IGraph &igraph) {
    graph_.resize(igraph.VerticesCount());
    vertices_count_ = igraph.VerticesCount();
    for (int i = 0; i < vertices_count_; i++) {
        graph_[i] = igraph.GetNextVertices(i);
    }
};

void ListGraph::AddEdge(int from, int to) {
    graph_[from].push_back(to);
    // reverse_graph_[to].push_back(from);
};

int ListGraph::VerticesCount() const { return vertices_count_; };

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    std::vector<int> result;
    result.reserve(graph_[vertex].size());
    for (int i = 0; i < result.capacity(); i++) {
        result.push_back(graph_[vertex][i]);
    }

    return result;
};

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    std::vector<int> result;
    for (int i = 0; i < graph_.size(); i++) {
        for (int j = 0; j < graph_[i].size(); j++) {
            if (graph_[i][j] == vertex) {
                result.push_back(i);
            }
        }
    }

    return result;
};