#pragma once

#include "graph.h"
#include "creategraph.h"
#include <string>
#include <deque>
#include <fstream>


/** \defgroup createdot Create Dot Files
 * @{ **/

/// Creates .dot files
/**
 * Creates output files of the graphs in the DOT-language that enables for rendering the graphs.
 *
 */
void create_dot( const std::deque<Graph> &graphs, std::string filename );

/** }@ */
