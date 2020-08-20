#pragma once

#include "vertex.h"
#include "edge.h"
#include <deque>
#include <iostream>

class Graph {
    private:
	
	std::deque<Vertex> m_vertices;
	std::deque<Edge> m_edges;

    public:

	Graph( Vertex vertex, std::deque<Edge> edges);
	Graph(std::deque<Vertex> vertices, std::deque<Edge> edges);
	
	void append_vertex(const Vertex& vertex);
	void append_vertices(const std::deque<Vertex>& vertices);

	void append_edge(const Edge& edge);
	void append_edges(const std::deque<Edge>& edges );

	std::deque<Vertex> get_vertices() const;
	std::deque<Edge> get_edges() const;
	
	void print_edges() const;

	friend std::ostream& operator<< ( std::ostream &out, const Graph &graph );

};

Graph operator+ (const Graph &graph1, const Graph &graph2);
