#pragma once

#include "vertex.h"
#include "product.h"
#include "sum.h"
#include <deque>
#include <vector>
#include <string>

enum CCTruncation {
    S,	// singles
    D,	// doubles
    T,	// triples
    Q	// quadruples
};

Sum<Vertex> bch_expansion( std::vector<CCTruncation> trunc );



