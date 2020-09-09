#pragma once
#include <iostream>
#include <string>
#include <deque>
#include <map>
#include <cassert>
#include <regex>
#include "vertex.h"


/** \defgroup parseinput Input Parser 
 * @{ */

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
std::map<OperatorType,std::deque<Vertex>> parse_input(std::string inputstr);

/** }@ */
