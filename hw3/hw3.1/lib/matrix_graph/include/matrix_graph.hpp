#include "igraph.hpp"

#include <vector>

class MatrixGraph : public IGraph {
  public:
    MatrixGraph(int verticeces_count);

    MatrixGraph(const IGraph &igraph);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

  private:
    int vertices_count_;
    std::vector<std::vector<int>> graph_;
};