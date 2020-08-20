#include "edge.h"


//Edge::Edge(Vertex &vertex_from, Vertex &vertex_to) :
//    from{ vertex_from }, to{ vertex_to } {}

Edge::Edge(const Vertex &vertex_from, const Vertex &vertex_to) :
    from{ vertex_from }, to{ vertex_to } {}

std::ostream& operator<< (std::ostream& out, const Edge &edge) {
	out << "(" << edge.from << "," << edge.to << ")" ;
	return out;
}
