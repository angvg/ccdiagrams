#pragma once

#include "vertex.h"
#include "edge.h"
#include <deque>

class Graph {
    private:
	
	std::deque<Vertex> m_vertices;
	std::deque<Edge> m_edges;

    public:

	Graph(std::deque<Vertex> vertices, std::deque<Edge> edges);
	
	void append_vertex(const Vertex& vertex);

	void append_edge(const Edge& edge);

};
