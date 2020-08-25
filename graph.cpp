#include "graph.h"
#include <deque>

void insert_or_increment( std::set<Vertex>& vertices, const Vertex& vertex );


Graph::Graph( Vertex vertex, std::deque<Edge> edges) :
    m_vertices{ {vertex} }, m_edges{ edges } {};
Graph::Graph(std::set<Vertex> vertices, std::deque<Edge> edges) :
    m_vertices{ vertices }, m_edges{ edges } {};
Graph::Graph( std::deque<Edge> edges ) : m_edges{ edges } {
    for ( auto& edge : m_edges ) {
	   insert_or_increment( m_vertices, edge.from ); 
	   insert_or_increment( m_vertices, edge.to );
		    //Vertex add_vertex = *result_from.first;	
	    //add_vertex += edge.from;
	    //m_vertices.erase( result_from.first );
	    //m_vertices.insert( add_vertex );
	}
    };


//void Graph::append_vertex(const Vertex& vertex ) {
//    m_vertices.push_back( vertex );
//}

//void Graph::append_vertices(const std::deque<Vertex>& vertices) {
//    for ( auto& vertex : vertices )
//	m_vertices.push_back( vertex );
//}

void Graph::append_edge(const Edge& edge ) {
    m_edges.push_back( edge );
}
void Graph::append_edges(const std::deque<Edge>& edges ) {
    for ( auto& edge : edges )
    m_edges.push_back( edge );
}

std::set<Vertex> Graph::get_vertices() const {
    return m_vertices;
}

std::deque<Edge> Graph::get_edges() const {
    return m_edges;
}

void Graph::print_edges() const {
    for ( auto& edge : m_edges )
	std::cout << edge;
}

//Graph operator+ (const Graph &graph1, const Graph &graph2) {
//    Graph graph( graph1.get_vertices(), graph1.get_edges() );
//    graph.append_vertices( graph2.get_vertices() );
//    graph.append_edges( graph2.get_edges() );
//    return graph;
//}

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


void insert_or_increment( std::set<Vertex>& vertices, const Vertex& vertex ) {
    auto result = vertices.insert( vertex );
    if ( !result.second ) {
	auto node = vertices.extract( result.first );
	node.value() += vertex;
	vertices.insert(std::move(node));
    }
}
