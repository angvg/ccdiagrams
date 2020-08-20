#pragma once

#include "vertex.h"

class Edge{

    private:


    public:

	const Vertex from;
	const Vertex to;

	//Edge(Vertex &vertex_from, Vertex &vertex_to);
	Edge(const Vertex &vertex_from, const Vertex &vertex_to);

	
	friend std::ostream& operator<< (std::ostream& out, const Edge &edge);

};
