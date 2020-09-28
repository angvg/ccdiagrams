#pragma once
#include <iostream>
#include <string>
#include <deque>
#include <map>
#include <cassert>
#include <regex>
#include "vertex.h"
#include "product.h"
#include "sum.h"


/** \defgroup parseinput Input Parser 
 * @{ */

/// Slice input string at blank characters to receive a list of operators.
std::deque<std::string> slice_input( std::string inputstr );

/// Creates a product of vertices that contain complete external operators and vertices with names only for physical and cluster operators from sliced input.
Product<Vertex> create_product( std::deque<std::string> sliced_input );
// For generation of excitation operators.
Product<Vertex> create_product( int substlevel );

/// Parses the user-input string to deques of vertices.
/** 
 * Input:
 * 	A string containing external, physical and cluster operators.
 *
 * Output:
 * 	A map containing deques of vertices, that have the same operator type.
 * 	In addition each pysical operator has its own deque. This is because physical
 * 	operators can manifest in different fragments.
*/
std::map<OperatorType,std::deque<Vertex>> parse_input( Product<Vertex>& input_product );

/** }@ */
