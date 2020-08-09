#include "graph.h"

Graph::Graph(std::deque<Vertex> vertices, std::deque<Edge> edges) :
    m_vertices{ vertices }, m_edges{ edges } {};


void Graph::append_vertex(const Vertex& vertex ) {
    m_vertices.push_back( vertex );
}

void Graph::append_edge(const Edge& edge ) {
    m_edges.push_back( edge );
}
