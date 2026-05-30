#include "ListGraph.h"

ListGraph::ListGraph(int verticesCount)
    : adjacencyLists(verticesCount) {
}

ListGraph::ListGraph(const IGraph& graph)
    : adjacencyLists(graph.VerticesCount()) {
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        adjacencyLists[from] = graph.GetNextVertices(from);
    }
}

void ListGraph::AddEdge(int from, int to) {
    adjacencyLists[from].push_back(to);
}

int ListGraph::VerticesCount() const {
    return static_cast<int>(adjacencyLists.size());
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    return adjacencyLists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prevVertices;

    for (int from = 0; from < VerticesCount(); ++from) {
        for (int to : adjacencyLists[from]) {
            if (to == vertex) {
                prevVertices.push_back(from);
            }
        }
    }

    return prevVertices;
}