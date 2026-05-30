#include "MatrixGraph.h"

MatrixGraph::MatrixGraph(int verticesCount)
    : matrix(verticesCount, std::vector<bool>(verticesCount, false)) {
}

MatrixGraph::MatrixGraph(const IGraph& graph)
    : MatrixGraph(graph.VerticesCount()) {
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        std::vector<int> nextVertices = graph.GetNextVertices(from);

        for (int to : nextVertices) {
            AddEdge(from, to);
        }
    }
}

void MatrixGraph::AddEdge(int from, int to) {
    matrix[from][to] = true;
}

int MatrixGraph::VerticesCount() const {
    return static_cast<int>(matrix.size());
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    std::vector<int> nextVertices;

    for (int to = 0; to < VerticesCount(); ++to) {
        if (matrix[vertex][to]) {
            nextVertices.push_back(to);
        }
    }

    return nextVertices;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    std::vector<int> prevVertices;

    for (int from = 0; from < VerticesCount(); ++from) {
        if (matrix[from][vertex]) {
            prevVertices.push_back(from);
        }
    }

    return prevVertices;
}