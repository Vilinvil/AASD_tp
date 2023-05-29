#include "igraph.hpp"

#include <set>
#include <vector>

class SetGraph : public IGraph {
  public:
    SetGraph(int vertices_count);

    SetGraph(const IGraph &igraph);

    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

  private:
    int vertices_count_;
    std::vector<std::set<int>> graph_;
};