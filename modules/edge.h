#pragma once

#include "vertex.h"


enum class LineType {
    particle,
    hole
};

class Edge{

    private:

	Vertex m_from;
	Vertex m_to;
	LineType m_type;


    public:


	Edge(const Vertex &vertex_from, const Vertex &vertex_to);
	Edge(const Vertex &vertex_from, const Vertex &vertex_to, LineType type);

	Vertex get_from() const;
	Vertex get_to() const;
	LineType get_type() const;
	
	friend std::ostream& operator<< (std::ostream& out, const Edge &edge);

};
