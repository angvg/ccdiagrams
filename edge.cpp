#include "edge.h"


Edge::Edge(const Vertex &vertex_from, const Vertex &vertex_to) :
    m_from{ vertex_from }, m_to{ vertex_to } {}
Edge::Edge(const Vertex &vertex_from, const Vertex &vertex_to, LineType type) :
    m_from{ vertex_from }, m_to{ vertex_to }, m_type{type} {
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
