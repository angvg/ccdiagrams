#pragma once

#include "vertex.h"
#include "graph.h"
#include <deque>
#include <map>
#include <list>
#include <stack>
#include <tuple>
#include <iostream>
#include <cassert>

/** \defgroup creategraph Graph Creation
 * @{ */

/// Creates a queue of all possible edges from a given set of vertices.
/** 
 * From the input vertices, all possible combinations of outgoing connections in the q-creation 
 * and incoming connections in the q-annihilation space, and outgoing connections in the
 * q-annihilation and incoming connections in the q-creation space, are created.
 */
std::deque<Edge> create_edges(const std::set<Vertex> &vertices);


/// Creates a map with cluster operators as keys and queues of edges from or to that operator as values.
std::map<Vertex,std::deque<Edge>> basic_cluster_edges( const std::set<Vertex> &vertices, const std::deque<Edge> &edges );



/// Creates all possible operator contractions from a given set of edges.
/**
 * Input:
 * 	Map	Key: Vertex
 * 		Value: Queue of edges, that start or end at key vertex
 *
 * Output:
 * 	Queue	All fragments of the key vertex, that can be created with
 * 		value edges. The member vertices of the graphs store the actual connections
 * 		within the graph in their connectiontable, while the associated set in the return pair
 *		contains all vertices that have to be included in the final graph and their connectiontables
 *		resemble the connections that can still be formed when the associated subgraph/fragment is
 *		subtracted.
 *
 */
std::map<Vertex,std::deque<std::pair<Graph, std::set<Vertex>>>> basic_cluster_fragments( const std::map<Vertex,std::deque<Edge>> &vertexedges, const std::set<Vertex> &all_vertices );


/// Tries to merge two graphs.
/**
 * Input:
 * 	Graphs that are to be merged, including the vertex-sets that store the connections that can still be formed,
 * 	when the connections (entries in the connectiontable) are subtracted from all vertices contained in a final graph.
 *
 * Output:
 * 	If successful, returns the merged graph and vertex-set (see above) and true.
 * 	If failed, returns the lhs graph, vertex-set and false.
 * 	
 * Compares the associated vertex-set of lhs (connections that can still be formed) with the member set of rhs (connections
 * that are present in rhs graph). If the connections of rhs are not present in the associated set of lhs, the graphs cannot
 * be merged.
 *
 */
std::tuple<Graph, std::set<Vertex>,bool> merge_graphs( const std::pair<Graph, std::set<Vertex>> &lhs_graph, const std::pair<Graph, std::set<Vertex>> &rhs_graph );
 

/// Creates all combinations of graphs.
/**
 * Creates all possible combinations of lhs and rhs graphs. Each entry of lhs is tried to merge with rhs.
 */
std::deque<std::pair<Graph, std::set<Vertex>>> asymmetric_combinations(
	const std::deque<std::pair<Graph, std::set<Vertex>>> &lhs_graphs, const std::deque<std::pair <Graph, std::set<Vertex>>> &rhs_graphs );


/// Creates unique 2-combinations of graphs.
/**
 * All entries of lhs are merged with entries of lhs. Creates only unique combinations.
 * For example, the combination of entry 1 and 2 results in combination 1-2, that is the same
 * as combination 2-1.
 *
 */
std::deque<std::pair<Graph, std::set<Vertex>>> symmetric_combinations(
	const std::deque<std::pair<Graph, std::set<Vertex>>> &lhs_graphs );


/// Combines basic fragments.
std::deque<std::pair<Graph, std::set<Vertex>>> complete_cluster_combinations(
	const Vertex &cluster, const std::deque<std::pair<Graph, std::set<Vertex>>> &basic_fragments );

/// Creates complete graphs that includes all combinations of vertices.
std::deque<Graph> create_graphs( const std::set<Vertex> &vertices );

/// Removes duplicate identical graphs.
std::deque<Graph> remove_duplicates( const std::deque<Graph> &all_graphs );

/// Removes graphs that contain disconnected fragments.
std::deque<Graph> remove_disconnected( const std::deque<Graph> &all_graphs );

//std::deque<Graph> pull_apart( const std::deque<Graph> &complete_graphs );
/** @} */
