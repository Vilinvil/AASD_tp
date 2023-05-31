// Требуется отыскать самый выгодный маршрут между городами.
// Требования: время работы O((N+M)logN), где N-количество городов, M-известных
// дорог между ними. Формат входных данных. Первая строка содержит число N –
// количество городов. Вторая строка содержит число M - количество дорог. Каждая
// следующая строка содержит описание дороги (откуда, куда, время в пути).
// Последняя строка содержит маршрут (откуда и куда нужно доехать).
// Формат выходных данных.
// Вывести длину самого выгодного маршрута.

#include <iostream>
#include <limits>
#include <set>
#include <utility>
#include <vector>

namespace {
// may be vertex=from and may be vertex=to in different applications
struct Edge {
    int vertex;
    int weight;
};

struct IGraph {
    virtual ~IGraph(){};

    virtual void AddEdge(int from, int to, int weight) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<Edge> GetNextVertices(int vertex) const = 0;
    virtual std::vector<Edge> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph {
  public:
    ListGraph(int vertices_count);

    ListGraph(const IGraph &igraph);

    void AddEdge(int from, int to, int weight) override;

    int VerticesCount() const override;

    std::vector<Edge> GetNextVertices(int vertex) const override;

    std::vector<Edge> GetPrevVertices(int vertex) const override;

  private:
    std::vector<std::vector<Edge>> graph_;

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

void ListGraph::AddEdge(int from, int to, int weight) {
    graph_[from].push_back(Edge{to, weight});
};

int ListGraph::VerticesCount() const { return vertices_count_; };

std::vector<Edge> ListGraph::GetNextVertices(int vertex) const {
    std::vector<Edge> result;
    result.reserve(graph_[vertex].size());
    for (int i = 0; i < result.capacity(); i++) {
        result.push_back(graph_[vertex][i]);
    }

    return result;
};

// if shoud be need use, may add reverse graph
std::vector<Edge> ListGraph::GetPrevVertices(int vertex) const {
    std::vector<Edge> result;
    for (int i = 0; i < graph_.size(); i++) {
        for (int j = 0; j < graph_[i].size(); j++) {
            if (graph_[i][j].vertex == vertex) {
                result.push_back(Edge{i, graph_[i][j].weight});
            }
        }
    }

    return result;
};

// may be easy add vector parents to creating path from to
int calculMinPath(IGraph &igraph, int from, int to) {
    constexpr int INT_MAX = std::numeric_limits<int>::max();
    std::vector<int> dist_vec(igraph.VerticesCount(), INT_MAX);

    // first = distance, second = vertex
    using dist = std::pair<int, int>;
    std::set<dist> q;
    q.insert({0, from});
    while (!q.empty()) {
        dist cur_vertex = *q.begin();
        q.erase(cur_vertex);
        auto children = igraph.GetNextVertices(cur_vertex.second);
        for (auto child : children) {
            if (dist_vec[child.vertex] == INT_MAX) {
                dist_vec[child.vertex] = cur_vertex.first + child.weight;
                q.insert(dist{dist_vec[child.vertex], child.vertex});
            } else if (dist_vec[child.vertex] >
                       cur_vertex.first + child.weight) {
                q.erase(dist{dist_vec[child.vertex], child.vertex});
                q.insert(dist{cur_vertex.first + child.weight, child.vertex});
                dist_vec[child.vertex] = cur_vertex.first + child.weight;
            }
        }
    }

    return dist_vec[to];
};

void run(std::istream &in, std::ostream &out) {
    int count_vertices;
    in >> count_vertices;
    ListGraph l_graph(count_vertices);

    int count_edge;
    in >> count_edge;
    for (int i = 0; i < count_edge; i++) {
        int from, to, weight;
        in >> from >> to >> weight;
        l_graph.AddEdge(from, to, weight);
        l_graph.AddEdge(to, from, weight);
    }

    int from_path, to_path;
    in >> from_path >> to_path;
    int result = calculMinPath(l_graph, from_path, to_path);
    out << result;
}
}   // namespace

int main() {
    try {
        run(std::cin, std::cout);
    } catch (std::exception &e) {
        throw std::runtime_error("in main: " +
                                 static_cast<std::string>(e.what()));
    }
}