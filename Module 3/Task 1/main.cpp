#include <iostream>

#include "ListGraph.h"
#include "MatrixGraph.h"
#include "SetGraph.h"
#include "ArcGraph.h"

void PrintGraph(const IGraph& graph) {
    for (int vertex = 0; vertex < graph.VerticesCount(); ++vertex) {
        std::cout << vertex << ": ";

        std::vector<int> nextVertices = graph.GetNextVertices(vertex);

        for (int next : nextVertices) {
            std::cout << next << " ";
        }

        std::cout << std::endl;
    }
}

void PrintPrevVertices(const IGraph& graph, int vertex) {
    std::cout << "Prev vertices for " << vertex << ": ";

    std::vector<int> prevVertices = graph.GetPrevVertices(vertex);

    for (int prev : prevVertices) {
        std::cout << prev << " ";
    }

    std::cout << std::endl;
}

int main() {
    ListGraph listGraph(4);

    listGraph.AddEdge(0, 1);
    listGraph.AddEdge(0, 2);
    listGraph.AddEdge(1, 3);
    listGraph.AddEdge(2, 3);

    std::cout << "ListGraph:" << std::endl;
    PrintGraph(listGraph);
    PrintPrevVertices(listGraph, 3);

    MatrixGraph matrixGraph(listGraph);

    std::cout << std::endl;
    std::cout << "MatrixGraph:" << std::endl;
    PrintGraph(matrixGraph);
    PrintPrevVertices(matrixGraph, 3);

    SetGraph setGraph(matrixGraph);

    std::cout << std::endl;
    std::cout << "SetGraph:" << std::endl;
    PrintGraph(setGraph);
    PrintPrevVertices(setGraph, 3);

    ArcGraph arcGraph(setGraph);

    std::cout << std::endl;
    std::cout << "ArcGraph:" << std::endl;
    PrintGraph(arcGraph);
    PrintPrevVertices(arcGraph, 3);

    return 0;
}