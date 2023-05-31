#include "igraph.hpp"

#include <vector>

namespace a_graph {
class ArcGraph : public IGraph {
  public:
    ArcGraph(int vertices_count);
    ArcGraph(const IGraph &igraph);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

  private:
    int vertices_count_;
    std::vector<std::pair<int, int>> graph_;
};
}   // namespace a_graph
