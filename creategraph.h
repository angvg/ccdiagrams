#pragma once

#include "vertex.h"
#include "graph.h"
#include <map>
#include <list>
#include <iostream>

/** \defgroup creategraph Graph Creation
 * @{
 */

/// Creates a queue of all possible edges from a given queue of vertices.
/** 
 * From the input vertices, all possible combinations of outgoing connections in the q-creation 
 * and incoming connections in the q-annihilation space, and outgoing connections in the
 * q-annihilation and incoming connections in the q-creation space, are created.
 */
std::deque<Edge> create_edges(const std::deque<Vertex> &vertices);


/// Creates a map with cluster operators as keys and queues of edges from or to that operator as values.
std::map<Vertex,std::deque<Edge>> basic_cluster_edges( const std::deque<Vertex> &vertices, const std::deque<Edge> &edges );

/// Creates all possible operator contractions from a given set of edges.
/**
 * Input:
 * 	Map	Key: Vertex
 * 		Value: Queue of edges, that start or end at key vertex
 *
 * Output:
 * 	Queue	All fragments of the key vertex, that can be created with
 * 		value edges
 */
std::map<Vertex,std::deque<Graph>> basic_cluster_fragments( const std::map<Vertex,std::deque<Edge>> &vertexedges );


//std::deque<Graph> create_graphs(std::deque<Vertex> vertices);

/** @} */
