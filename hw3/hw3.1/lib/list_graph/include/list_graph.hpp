#include "igraph.hpp"

#include <vector>

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

    // std::vector<std::vector<int>> reverse_graph_;
    int vertices_count_;
};