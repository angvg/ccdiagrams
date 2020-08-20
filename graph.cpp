#include "graph.h"

Graph::Graph( Vertex vertex, std::deque<Edge> edges) :
    m_vertices{ {vertex} }, m_edges{ edges } {};
Graph::Graph(std::deque<Vertex> vertices, std::deque<Edge> edges) :
    m_vertices{ vertices }, m_edges{ edges } {};


void Graph::append_vertex(const Vertex& vertex ) {
    m_vertices.push_back( vertex );
}

void Graph::append_edge(const Edge& edge ) {
    m_edges.push_back( edge );
}

void Graph::print_edges() const {
    for ( auto& edge : m_edges )
	std::cout << edge;
}

std::ostream& operator<< ( std::ostream &out, const Graph &graph ) {
    out << "Graph ";
    for ( auto& vertex : graph.m_vertices ) {
	out << vertex << "{" << vertex.rank() << "} ";
    }
    out << '\n';
    for ( auto& edge : graph.m_edges ) {
	out << edge << '\n';
    }
    return out;
};
