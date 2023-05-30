#include <iostream>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

struct IGraph {
    virtual ~IGraph(){};

    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph {
  public:
    ListGraph(int vertices_count);

    ListGraph(const IGraph &igraph);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

  private:
    std::vector<std::vector<int>> graph_;

    int vertices_count_;
};

ListGraph::ListGraph(int vertices_count) : vertices_count_(vertices_count) {
    graph_.resize(vertices_count);
};

ListGraph::ListGraph(const IGraph &igraph) {
    graph_.resize(igraph.VerticesCount());
    vertices_count_ = igraph.VerticesCount();
    for (int i = 0; i < vertices_count_; i++) {
        graph_[i] = igraph.GetNextVertices(i);
    }
};

void ListGraph::AddEdge(int from, int to) { graph_[from].push_back(to); };

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

int calculCountPath(IGraph &igraph, int from, int to) {
    std::vector<bool> visited_vec(igraph.VerticesCount(), false);
    std::vector<int> dist_vec(igraph.VerticesCount());
    std::vector<int> path_count_vec(igraph.VerticesCount());

    std::queue<int> q;
    q.push(from);
    dist_vec[from] = 0;
    path_count_vec[from] = 1;
    visited_vec[from] = true;

    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        for (int child : igraph.GetNextVertices(cur)) {
            if (!visited_vec[child]) {
                q.push(child);
                visited_vec[child] = true;
            }
            if (dist_vec[child] == 0 || dist_vec[child] > (dist_vec[cur] + 1)) {
                dist_vec[child] = dist_vec[cur] + 1;
                path_count_vec[child] = path_count_vec[cur];
            } else if (dist_vec[child] == (dist_vec[cur] + 1)) {
                path_count_vec[child] += path_count_vec[cur];
            }
        }
    }

    return path_count_vec[to];
}

void run(std::istream &in, std::ostream &out) {
    int count_vertices;
    in >> count_vertices;
    ListGraph l_graph(count_vertices);

    int count_edge;
    in >> count_edge;
    for (int i = 0; i < count_edge; i++) {
        int from, to;
        in >> from >> to;
        l_graph.AddEdge(from, to);
        l_graph.AddEdge(to, from);
    }

    int from_path, to_path;
    in >> from_path >> to_path;
    int result = calculCountPath(l_graph, from_path, to_path);
    out << result;
}

int main() {
    try {
        run(std::cin, std::cout);
    } catch (std::exception &e) {
        throw std::runtime_error("in main: " +
                                 static_cast<std::string>(e.what()));
    }
}