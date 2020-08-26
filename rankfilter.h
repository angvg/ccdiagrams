#pragma once

#include "vertex.h"
#include <deque>
#include <map>

/// Creates a queue for each unique combination of (physical) operators that can survive according to rank.
/**
 * Input
 * 	A map that contains a queue of vertices, that have the type of the key.
 *
 * Output
 * 	The inner set contains all external vertices, a unique combination
 * 	of physical operators, that can survive according to rank, and all cluster vertices.
 * 	The outer queue contains inner sets, that differ in their combination of physical operators.
 *
 */
std::deque<std::set<Vertex>> rank_filter( std::map<OperatorType,std::deque<Vertex>> vertexmap );
