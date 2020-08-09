#pragma once

#include "vertex.h"

class Edge{

    private:

	const Vertex* m_from;
	const Vertex* m_to;

    public:

	Edge(Vertex* from, Vertex* to);

};
