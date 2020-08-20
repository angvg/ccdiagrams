#include "creategraph.h"
#include <deque>

std::deque<Edge> create_edges(const std::deque<Vertex> &vertices) {
    std::deque<const Vertex*> creation_space_in;
    std::deque<const Vertex*> creation_space_out;
    std::deque<const Vertex*> annihilation_space_in;
    std::deque<const Vertex*> annihilation_space_out;

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
	    if ( *c_out != *a_in )
	    edgelist.push_back( Edge( *c_out, *a_in ) );
	}
    }
    for (auto & a_out : annihilation_space_out ) {
	for ( auto& c_in : creation_space_in ) {
	    if ( *c_in != *a_out )
	    edgelist.push_back( Edge( *a_out, *c_in ) );
	}
    }
    return edgelist;
}


std::map<Vertex,std::deque<Edge>> basic_cluster_edges( const std::deque<Vertex> &vertices, const std::deque<Edge> &edges ) {
    std::map<Vertex,std::deque<Edge>> cluster_edges;
    for ( auto& vertex : vertices ) {
	if ( vertex.get_operatortype() == OperatorType::cluster ) {
	    // Collect all in- and outgoing edges
	    std::deque<Edge> current_edges;
	    for ( auto& edge : edges ) {
		if ( edge.from == vertex || edge.to == vertex )
		    current_edges.push_back( edge );
	    }
	    // Append to return map
	    cluster_edges.insert( { vertex , current_edges } );
	}
    }
    return cluster_edges;
}


std::map<Vertex,std::deque<Graph>> basic_cluster_fragments( const std::map<Vertex,std::deque<Edge>> &vertexedges ) {
    std::map<Vertex,std::deque<Graph>> fragment_map;
    // Iterate over all vertices
    for ( auto& mapentry : vertexedges ) {
	// Sort edges in incoming and outgoing
	std::deque<Edge> incoming;
	std::deque<Edge> outgoing;
	for ( auto& edge : mapentry.second ) {
	    if ( edge.from == mapentry.first )
		outgoing.push_back( edge );
	    else if ( edge.to == mapentry.first )
		incoming.push_back( edge );
	}
	// Create all combinations of incoming and outgoing edges
	std::deque<Graph> graphlist;
	for ( auto& inedge : incoming ) {
	    for ( auto& outedge : outgoing ) {
	graphlist.push_back( { mapentry.first, {inedge,outedge} } );
	    }
	}
	fragment_map.insert( {mapentry.first,graphlist} );
    }
    return fragment_map;
}


//Graph join_graphs(
//
//void create_graphs(std::deque<Graph> &final_graphs,std::deque<Graph> &current_graphs, std::list<Vertex*> vertices, std::list<Edge*> edges) {
//
//};
