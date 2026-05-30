#include "ArcGraph.h"

ArcGraph::ArcGraph(int verticesCount)
    : verticesCount(verticesCount) {
}

ArcGraph::ArcGraph(const IGraph& graph)
    : verticesCount(graph.VerticesCount()) {
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        std::vector<int> nextVertices = graph.GetNextVertices(from);

        for (int to : nextVertices) {
            AddEdge(from, to);
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    edges.push_back({ from, to });
}

int ArcGraph::VerticesCount() const {
    return verticesCount;
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    std::vector<int> nextVertices;

    for (const auto& edge : edges) {
        if (edge.first == vertex) {
            nextVertices.push_back(edge.second);
        }
    }

    return nextVertices;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prevVertices;

    for (const auto& edge : edges) {
        if (edge.second == vertex) {
            prevVertices.push_back(edge.first);
        }
    }

    return prevVertices;
}