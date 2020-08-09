#include "edge.h"


Edge::Edge(Vertex* from, Vertex* to) :
    m_from{ from }, m_to{ to } {}


std::ostream& operator<< (std::ostream& out, const Edge &edge) {
	out << "( " << *(edge.m_from) << " , " << *(edge.m_to) << " )" ;
	return out;
}
