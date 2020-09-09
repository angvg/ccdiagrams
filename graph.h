#pragma once

#include "vertex.h"
#include "edge.h"
#include <deque>
#include <set>
#include <map>
#include <iostream>


/** 
 *
 * A graph can be desribed by a set of vertices and edges.
 * The constructor takes edges and inserts vertices contained in them
 * into its member set. (Directed) Edges contain the information which vertex is origin
 * and which is destination and if the edge represents a particle or hole line.
 * With this information, the contructor sets the connectiontables of the inserted
 * vertices to the apporpriate values.
 *
*/
class Graph {
    private:
	
	std::set<Vertex> m_vertices;
	std::deque<Edge> m_edges;

    public:

	Graph( );
	/// Inserts vertices in m_vertices with appropriate connectiontables according to the edges.
	Graph( std::deque<Edge> edges );

	/// For appended edges, the same insertion as for the constructor takes place.
	void append_edge(const Edge& edge);
	void append_edge(const std::deque<Edge>& edges );

	std::set<Vertex> get_vertices() const;
	std::deque<Edge> get_edges() const;
	
	void print_vertices() const;
	void print_edges() const;

	friend std::ostream& operator<< ( std::ostream &out, const Graph &graph );

};

Graph operator+ ( const Graph &lhs_graph, const Graph &rhs_graph);


/** 
 *
 * Constructs an adjacency matrix of a graph. It is a 2D-map, with rows resembling
 * the origin, and columns the destination vertex. 
 *
*/
class AdjacencyMatrix {

    public:

	std::map<Vertex, std::map<Vertex,int>> m_matrix;


	AdjacencyMatrix( const Graph &graph );

};

bool operator== ( const AdjacencyMatrix &lhs, const AdjacencyMatrix &rhs );
