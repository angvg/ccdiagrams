#include "creategraph.h"

std::deque<Graph> create_graph(std::deque<Vertex> vertices) {
    std::deque<Graph> graphlist;
    std::deque<Vertex*> creation_space_in;
    std::deque<Vertex*> creation_space_out;
    std::deque<Vertex*> annihilation_space_in;
    std::deque<Vertex*> annihilation_space_out;

    // Populate lists
    for ( auto vertex_pointer = vertices.begin() ; vertex_pointer != vertices.end() ; ++vertex_pointer ) {
	// If in- or outgoing connections exist, append pointer to list.
	if ( (*vertex_pointer).get_creation().first != 0 ) {
	    creation_space_in.push_back( { &(*vertex_pointer) } );
	}
	if ( (*vertex_pointer).get_creation().second != 0 ) {
	    creation_space_out.push_back( { &(*vertex_pointer) } );
	}
	if ( (*vertex_pointer).get_annihilation().first != 0 ) {
	    annihilation_space_in.push_back( { &(*vertex_pointer) } );
	}
	if ( (*vertex_pointer).get_annihilation().second != 0 ) {
	    annihilation_space_out.push_back( { &(*vertex_pointer) } );
	}
    }

    // Create all possible combinations of edges.
    // This is done by combining outgoing connections of creation with incoming of annihilation space and
    // outgoing connections of annihilation with incoming of creation space.

    std::deque<Edge> edgelist;

    for ( auto& c_out : creation_space_out ) {
	for ( auto& a_in : annihilation_space_in ) {
	    edgelist.push_back( Edge( c_out, a_in ) );
	}
    }
    for (auto & a_out : annihilation_space_out ) {
	for ( auto& c_in : creation_space_in ) {
	    edgelist.push_back( Edge( a_out, c_in ) );
	}
    }

    std::cout << "Printing edgelist for: "  ;
    for ( auto& vertex : vertices )
	std::cout << vertex << " ";
    std::cout << '\n';
    for ( auto& edge : edgelist )
	std::cout << edge << '\n';


    return graphlist;
}
