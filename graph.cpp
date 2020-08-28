#include "graph.h"
#include <deque>

void insert_or_increment( std::set<Vertex>& vertices, const Edge& edge  );


Graph::Graph( Vertex vertex, std::deque<Edge> edges) :
    m_vertices{ {vertex} }, m_edges{ edges } {};
Graph::Graph(std::set<Vertex> vertices, std::deque<Edge> edges) :
    m_vertices{ vertices }, m_edges{ edges } {};
Graph::Graph( std::deque<Edge> edges ) : m_edges{ edges } {
    for ( auto& edge : m_edges ) {
	   insert_or_increment( m_vertices, edge ); 
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
    insert_or_increment( m_vertices, edge );
}
void Graph::append_edge(const std::deque<Edge>& edges ) {
    for ( auto& edge : edges ) {
    m_edges.push_back( edge );
    insert_or_increment( m_vertices, edge );
    }
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

Graph operator+ ( const Graph &lhs_graph, const Graph &rhs_graph ) {
    Graph graph( lhs_graph );
    graph.append_edge( rhs_graph.get_edges() );
    return graph;
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


void insert_or_increment( std::set<Vertex>& vertices, const Edge& edge ) {
    Vertex from{ edge.get_from() };
    from.clear_table();
    if ( edge.get_type() == LineType::particle ) 
	from.set_connection( 0,1,1 );
    else 
	from.set_connection( 1,1,1 );
    auto result = vertices.insert( from );
    if ( !result.second ) {
	auto node = vertices.extract( result.first );
	node.value() += from;
	vertices.insert(std::move(node));
    }
    Vertex to{ edge.get_to() };
    to.clear_table();
    if ( edge.get_type() == LineType::particle )
	to.set_connection( 1,0,1 );
    else
	to.set_connection( 0,0,1 );
    result = vertices.insert( to );
    if ( !result.second ) {
	auto node = vertices.extract( result.first );
	node.value() += to;
	vertices.insert(std::move(node));
    }
}
