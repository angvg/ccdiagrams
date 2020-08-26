#include "edge.h"


//Edge::Edge(Vertex &vertex_from, Vertex &vertex_to) :
//    from{ vertex_from }, to{ vertex_to } {}

Edge::Edge(const Vertex &vertex_from, const Vertex &vertex_to) :
    m_from{ vertex_from }, m_to{ vertex_to } {}
Edge::Edge(const Vertex &vertex_from, const Vertex &vertex_to, LineType type) :
    m_from{ vertex_from }, m_to{ vertex_to }, m_type{type} {
	m_from.clear_table();
	m_to.clear_table();
	if ( m_type == LineType::particle ) {
	    m_from.set_connection( 0,1, 1 );
	    m_to.set_connection( 1,0, 1 );
	}
	if ( m_type == LineType::hole ) {
	    m_from.set_connection( 1,1, 1 );
	    m_to.set_connection( 0,0, 1 );
	}
    }


Vertex Edge::get_from() const {
    return m_from;
}
Vertex Edge::get_to() const {
    return m_to;
}
LineType Edge::get_type() const {
    return m_type;
}


std::ostream& operator<< (std::ostream& out, const Edge &edge) {
	out << "(" << edge.m_from << "," << edge.m_to << ")" ;
	return out;
}
