#pragma once

#include "vertex.h"
#include <deque>
#include <map>

std::deque<std::deque<Vertex>> rank_filter( std::map<OperatorType,std::deque<Vertex>> vertexmap );
