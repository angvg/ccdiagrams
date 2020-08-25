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
	if ( (*vertex_pointer).get_connection(0,0) != 0 ) {
	    creation_space_in.push_back( { &(*vertex_pointer) } );
	}
	if ( (*vertex_pointer).get_connection(0,1) != 0 ) {
	    creation_space_out.push_back( { &(*vertex_pointer) } );
	}
	if ( (*vertex_pointer).get_connection(1,0) != 0 ) {
	    annihilation_space_in.push_back( { &(*vertex_pointer) } );
	}
	if ( (*vertex_pointer).get_connection(1,1) != 0 ) {
	    annihilation_space_out.push_back( { &(*vertex_pointer) } );
	}
    }

    // Create all possible combinations of edges.
    // This is done by combining outgoing connections of creation with incoming of annihilation space and
    // outgoing connections of annihilation with incoming of creation space.

    std::deque<Edge> edgelist;

    for ( auto& c_out : creation_space_out ) {
	for ( auto& a_in : annihilation_space_in ) {
	    if ( *c_out != *a_in ) {
		Vertex v_from{ *c_out };
		Vertex v_to{ *a_in };
		v_from.clear_table();
		v_from.set_connection( 0, 1, 1 );
		v_to.clear_table();	
		v_to.set_connection( 1 , 0, 1 );
		edgelist.push_back( Edge( v_from, v_to ) );
	    }
	}
    }
    for (auto & a_out : annihilation_space_out ) {
	for ( auto& c_in : creation_space_in ) {
	    if ( *c_in != *a_out ) {
		Vertex v_from{ *a_out};
		Vertex v_to{ *c_in };
		v_from.clear_table();
		v_from.set_connection( 1, 1, 1 );
		v_to.clear_table();	
		v_to.set_connection( 0 , 0, 1 );
		edgelist.push_back( Edge( v_from, v_to ) );
	    }
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


std::tuple<Graph, std::set<Vertex>,bool> join_graphs( 
	const Graph &lhs_graph, const Graph &rhs_graph, const std::set<Vertex> &remaining_vertices )
{
	    // A list of vertices, whose edge_table resembles the connections that can still be formed.
	    std::set<Vertex> new_remaining_vertices{ remaining_vertices };
	    // From the above list, subtract the connections of lhs.
	    for ( auto& lhs_vertex : lhs_graph.get_vertices() ) {
		auto lhs_node = new_remaining_vertices.extract( lhs_vertex );
		if ( !lhs_node.value().decrease_connection( lhs_vertex ) )
		    return {lhs_graph,remaining_vertices,false};
		new_remaining_vertices.insert( std::move( lhs_node ) );
	    }
	    // Moving on to the rhs. We need to check if we have open connections from the lhs left,
	    // so we can join lhs and rhs.
	    for ( auto& rhs_vertex : rhs_graph.get_vertices() ) {
		auto rhs_node = new_remaining_vertices.extract( rhs_vertex );
		if ( !rhs_node.value().decrease_connection( rhs_vertex ) )
		    return {lhs_graph,remaining_vertices,false};
		new_remaining_vertices.insert( std::move( rhs_node ) );	
	    }
	    // If we haven't returned by now, we're good to join the graphs.		
	    Graph new_lhs_graph{ lhs_graph + rhs_graph };
	    return { new_lhs_graph, new_remaining_vertices, true };
}
    
std::deque<Graph> compound_cluster_fragments( std::deque<Graph> basic_fragments, const std::set<Vertex> &all_vertices ) {
    std::deque<Graph> outgraphs;
    auto frag_it{ basic_fragments.cbegin() };
    for ( auto& lhs_fragment : basic_fragments ) {
	std::deque<Graph> lhs_graphs{ {lhs_fragment} };
	for ( ; frag_it != basic_fragments.cend() ; ++frag_it ) {
	}
    }
    return outgraphs;
}

	    


//Graph join_graphs(
//
//void create_graphs(std::deque<Graph> &final_graphs,std::deque<Graph> &current_graphs, std::list<Vertex*> vertices, std::list<Edge*> edges) {
//
//};
