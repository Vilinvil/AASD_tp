// Дан базовый интерфейс для представления ориентированного графа : struct
// IGraph {
//     virtual ~IGraph() {}

//     // Добавление ребра от from к to.
//     virtual void AddEdge(int from, int to) = 0;

//     virtual int VerticesCount() const = 0;

//     virtual std::vector<int> GetNextVertices(int vertex) const = 0;
//     virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
// };

// Необходимо написать несколько реализаций интерфейса
//     : ListGraph,
//       хранящий граф в виде массива списков смежности,
//       MatrixGraph,
//       хранящий граф в виде матрицы смежности,
//       SetGraph,
//       хранящий граф в виде массива хэш -
//           таблиц / сбалансированных деревьев поиска,
//       ArcGraph,
//       хранящий граф в виде одного массива пар{from, to}.Также
//           необходимо реализовать конструктор,
//       принимающий const IGraph
//           &.Такой конструктор должен скопировать переданный граф в
//           создаваемый
//               объект.Для каждого класса создавайте отдельные h и cpp файлы
//                   .Число вершин графа задается в конструкторе каждой
//                   реализации.

#include "arc_graph.hpp"
#include "igraph.hpp"
#include "list_graph.hpp"
#include "matrix_graph.hpp"
#include "set_graph.hpp"

#include <assert.h>
#include <iostream>
#include <set>
#include <sstream>
#include <vector>

namespace {
static std::set<int> setFromVec(const std::vector<int> &vec) {
    std::set<int> result;
    for (int i : vec) {
        result.insert(i);
    }

    return result;
}

void testMainLogic() {
    int size = 10;
    l_graph::ListGraph graph_list(size);

    graph_list.AddEdge(0, 1);
    graph_list.AddEdge(0, 2);
    graph_list.AddEdge(0, 4);
    graph_list.AddEdge(0, 6);
    graph_list.AddEdge(0, 7);

    graph_list.AddEdge(1, 2);
    graph_list.AddEdge(1, 3);
    graph_list.AddEdge(1, 5);

    graph_list.AddEdge(2, 4);
    graph_list.AddEdge(4, 8);
    graph_list.AddEdge(6, 9);

    bfs(graph_list, [](int v) { std::cout << "bfs: " << v << std::endl; });
    std::cout << "________" << std::endl;

    m_graph::MatrixGraph graph_matrix = m_graph::MatrixGraph(graph_list);
    bfs(graph_matrix, [](int v) { std::cout << "bfs: " << v << std::endl; });
    std::cout << "________" << std::endl;

    s_graph::SetGraph graph_set = s_graph::SetGraph(graph_matrix);
    bfs(graph_set, [](int v) { std::cout << "bfs: " << v << std::endl; });
    std::cout << "________" << std::endl;

    a_graph::ArcGraph graph_arc = a_graph::ArcGraph(graph_set);
    bfs(graph_arc, [](int v) { std::cout << "bfs: " << v << std::endl; });
    std::cout << "________" << std::endl;

    assert(graph_list.VerticesCount() == graph_matrix.VerticesCount());
    assert(graph_matrix.VerticesCount() == graph_set.VerticesCount());
    assert(graph_set.VerticesCount() == graph_arc.VerticesCount());

    for (int i = 0; i < size; i++) {
        auto next_l = setFromVec(graph_list.GetNextVertices(i));
        auto next_m = setFromVec(graph_matrix.GetNextVertices(i));
        auto next_s = setFromVec(graph_set.GetNextVertices(i));
        auto next_a = setFromVec(graph_arc.GetNextVertices(i));

        assert(next_l == next_m);
        assert(next_m == next_s);
        assert(next_s == next_a);

        auto prev_l = setFromVec(graph_list.GetPrevVertices(i));
        auto prev_m = setFromVec(graph_matrix.GetPrevVertices(i));
        auto prev_s = setFromVec(graph_set.GetPrevVertices(i));
        auto prev_a = setFromVec(graph_arc.GetPrevVertices(i));

        assert(prev_l == prev_m);
        assert(prev_m == prev_s);
        assert(prev_s == prev_a);
    }
    std::cout << "testMainLogic() OK" << std::endl;
}

}   // namespace

int main() {
    testMainLogic();
    return 0;
}