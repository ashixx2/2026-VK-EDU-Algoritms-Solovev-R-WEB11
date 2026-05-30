#include "SetGraph.h"

SetGraph::SetGraph(int verticesCount)
    : adjacencySets(verticesCount) {
}

SetGraph::SetGraph(const IGraph& graph)
    : adjacencySets(graph.VerticesCount()) {
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        std::vector<int> nextVertices = graph.GetNextVertices(from);

        for (int to : nextVertices) {
            AddEdge(from, to);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    adjacencySets[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return static_cast<int>(adjacencySets.size());
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    return std::vector<int>(
        adjacencySets[vertex].begin(),
        adjacencySets[vertex].end()
    );
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prevVertices;

    for (int from = 0; from < VerticesCount(); ++from) {
        if (adjacencySets[from].count(vertex) > 0) {
            prevVertices.push_back(from);
        }
    }

    return prevVertices;
}