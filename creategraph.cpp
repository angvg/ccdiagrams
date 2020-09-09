#include "creategraph.h"
#include <deque>
#include <iomanip>
#include <utility>




std::deque<Edge> create_edges(const std::set<Vertex> &vertices) {
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
		edgelist.push_back( Edge( v_from, v_to, LineType::particle ) );
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
		edgelist.push_back( Edge( v_from, v_to, LineType::hole ) );
	    }
	}
    }
    return edgelist;
}


std::map<Vertex,std::deque<Edge>> basic_cluster_edges( const std::set<Vertex> &vertices, const std::deque<Edge> &edges ) {
    std::map<Vertex,std::deque<Edge>> cluster_edges;
    //std::deque<Edge> residual_edges;
    for ( auto& vertex : vertices ) {
	if ( vertex.get_operatortype() == OperatorType::cluster ) {
	    // Collect all in- and outgoing edges
	    std::deque<Edge> current_edges;
	    for ( auto& edge : edges ) {
		if ( edge.get_from() == vertex || edge.get_to() == vertex )
		    current_edges.push_back( edge );
		//else if ( edge.get_from().get_operatortype() != OperatorType::cluster && edge.get_to().get_operatortype() != OperatorType::cluster ) {
		//   residual_edges.push_back( edge );
		//}
	    }
	    // Append to return map
	    cluster_edges.insert( { vertex , current_edges } );
	}
	else if ( vertex.get_operatortype() == OperatorType::physical ) {
	    std::deque<Edge> current_edges;
	    for ( auto& edge : edges ) {
		Vertex from{ edge.get_from() };
		Vertex to{ edge.get_to() };
		if ( ( from == vertex || to == vertex ) &&
			( from.get_operatortype() != cluster && to.get_operatortype() != cluster ) )
		    current_edges.push_back( edge );
	    }
	    cluster_edges.insert( { vertex, current_edges } );
	}
    }
    // Append residuals
    //if ( !residual_edges.empty() )
    //	cluster_edges.insert( { {"R", OperatorType::residual} , residual_edges } ); 
    return cluster_edges;
}


std::pair<std::set<Vertex>, bool> subtract_connections(
	const std::set<Vertex> &lhs_vertices, const std::set<Vertex> &rhs_vertices )
{
    std::set<Vertex> subtracted{ lhs_vertices };
    for ( auto& rhs_vertex : rhs_vertices ) {
	assert( lhs_vertices.contains( rhs_vertex ) && "In subtract_connections(..): rhs_vertex not found in lhs_vertices." );
	auto lhs_node = subtracted.extract( rhs_vertex );
	if ( !lhs_node.value().decrease_connection( rhs_vertex ) )
	    return {lhs_vertices,false}; 
	subtracted.insert( std::move(lhs_node) );
    }
    return {subtracted,true};
}
    

std::map<Vertex,std::deque<std::pair<Graph, std::set<Vertex>>>> basic_cluster_fragments(
	const std::map<Vertex,std::deque<Edge>> &vertexedges, const std::set<Vertex> &all_vertices )
{
    std::map<Vertex,std::deque<std::pair<Graph, std::set<Vertex>>>> fragment_map;
    // Iterate over all vertices
    for ( auto& mapentry : vertexedges ) {
	if ( mapentry.first.get_operatortype() == OperatorType::cluster ) {
	    // Sort edges in incoming and outgoing
	    std::deque<Edge> incoming;
	    std::deque<Edge> outgoing;
	    for ( auto& edge : mapentry.second ) {
		if ( edge.get_from() == mapentry.first )
		    outgoing.push_back( edge );
		else if ( edge.get_to() == mapentry.first )
		    incoming.push_back( edge );
	    }
	    // Create all combinations of incoming and outgoing edges
	    std::deque<std::pair<Graph, std::set<Vertex>>> graphlist;
	    for ( auto& inedge : incoming ) {
		for ( auto& outedge : outgoing ) {
		    Graph new_fragment{ { inedge, outedge } };
		    auto subtracted{ subtract_connections( all_vertices, new_fragment.get_vertices() ) };
		    assert( subtracted.second && "Basic_cluster_fragments(..) : failed to subtract connections." );
		    graphlist.push_back( { { {inedge,outedge} } , subtracted.first } );
		}
	    }
	    fragment_map.insert( {mapentry.first,graphlist} );
	}
	// For phsical operators just create a graph for each remaining edge
	else if ( mapentry.first.get_operatortype() == OperatorType::physical ) {
	    if ( mapentry.second.empty() )
		continue;
	    std::deque<std::pair<Graph, std::set<Vertex>>> graphlist;
	    for ( auto& edge : mapentry.second ) {
		Graph new_fragment{ { edge } };
		auto subtracted{ subtract_connections( all_vertices, new_fragment.get_vertices() ) };
		assert( subtracted.second && "Basic_cluster_fragments(..) : failed to subtract connections." );
		graphlist.push_back( { { new_fragment } , subtracted.first } );
	    }
	    fragment_map.insert( {mapentry.first, graphlist} );
	}
    }
    return fragment_map;
}


    

std::tuple<Graph, std::set<Vertex>,bool> merge_graphs( 
	const std::pair<Graph, std::set<Vertex>> &lhs_graph, const std::pair<Graph, std::set<Vertex>> &rhs_graph )
{
    //std::cout << "Attempting to join: " ;
    //(std::get<0>(lhs_graph)).print_edges(); std::cout <<  " & "; rhs_graph.first.print_edges();
	    // A set of vertices, whose edge_table resembles the connections that can still be formed.
	    auto subtracted{ subtract_connections( lhs_graph.second, rhs_graph.first.get_vertices() ) };
	    if ( !subtracted.second ) 
		return { lhs_graph.first, lhs_graph.second, subtracted.second };
	    // If we haven't returned by now, we're good to join the graphs.		
	    Graph new_lhs_graph{ lhs_graph.first + rhs_graph.first };
	    return { new_lhs_graph, subtracted.first, subtracted.second };
}

std::deque<std::pair<Graph, std::set<Vertex>>> asymmetric_combinations(
	const std::deque<std::pair<Graph, std::set<Vertex>>> &lhs_graphs, const std::deque<std::pair <Graph, std::set<Vertex>>> &rhs_graphs )
{
    //std::cout << "Performing asymmetric combination: " << '\n';
    std::deque<std::pair<Graph, std::set<Vertex>>> combined_graphs;
    for ( auto& lhs_graph : lhs_graphs ) {
	for ( auto& rhs_graph : rhs_graphs ) {
	    auto merged{ merge_graphs( lhs_graph, rhs_graph ) };
	    //std::cout << '\t' << "Attempting to merge "; lhs_graph.first.print_edges() ; std:: cout << " & "; rhs_graph.first.print_edges() ; std::cout << " , Result: " << std::get<2>(merged) << '\n';
	    if ( std::get<2>(merged) )
		combined_graphs.push_back( { std::get<0>(merged) , std::get<1>(merged) } );
	}
    }
    return combined_graphs;
}
    

std::deque<std::pair<Graph, std::set<Vertex>>> symmetric_combinations(
	const std::deque<std::pair<Graph, std::set<Vertex>>> &lhs_graphs )
{
    //std::cout << "Performing symmetric combination: " << '\n';
    std::deque<std::pair<Graph, std::set<Vertex>>> combined_graphs;
    auto it_rhs{ lhs_graphs.cbegin() };
    for ( auto& lhs_graph : lhs_graphs ) {
	for ( auto it_rhs_inner = it_rhs; it_rhs_inner != lhs_graphs.cend() ; ++it_rhs_inner ) {
	    auto merged{ merge_graphs( lhs_graph, *it_rhs_inner ) };
	    //std::cout << '\t' << "Attempting to merge "; lhs_graph.first.print_edges() ; std:: cout << " & "; it_rhs_inner->first.print_edges() ; std::cout << " , Result: " << std::get<2>(merged) << '\n';
	    if ( std::get<2>(merged) )
		combined_graphs.push_back( { std::get<0>(merged), std::get<1>(merged) } );
	}
	++it_rhs;
    }
    return combined_graphs;
}

std::deque<std::pair<Graph, std::set<Vertex>>> complete_cluster_combinations(
	const Vertex &cluster, const std::deque<std::pair<Graph, std::set<Vertex>>> &basic_fragments ) 
{
    // Splits the degree in even (combine fragment with itself) or odd (add a basic fragment to the combined) combinations.
    std::stack<bool> divisible_by_two;
    int degree{ cluster.degree() };
    if ( (cluster.get_operatortype() == OperatorType::physical) && (basic_fragments.size() < 2) )
	degree = 1;
    if ( degree == 1 )
	return basic_fragments;
    while ( degree > 2 ) {	
	if ( degree % 2 ) {	// if odd
	    degree -= 1;
	    divisible_by_two.push( false );
	} else {			// if even 
	    degree /= 2;
	    divisible_by_two.push( true );
	}
    }
    
    // Populate first time, case for degree == 2.
    std::deque<std::pair<Graph, std::set<Vertex>>> complete_fragments{ symmetric_combinations( basic_fragments ) } ;

    while ( !divisible_by_two.empty() ) {
	if ( divisible_by_two.top() ) 
	    complete_fragments = symmetric_combinations( complete_fragments );
	else
	    complete_fragments = asymmetric_combinations( complete_fragments, basic_fragments );
	divisible_by_two.pop();
    }

    return complete_fragments;
}


std::deque<Graph> remove_duplicates( const std::deque<Graph> &all_graphs ) {
    std::list<std::pair<Graph,AdjacencyMatrix>> compare_list;
    for ( auto& graph : all_graphs )
	compare_list.push_back( std::make_pair( graph, AdjacencyMatrix( graph ) ) );
    // Graphs are identical if adjacency matrices are equal.
    for ( auto it_list = compare_list.begin() ; it_list != compare_list.end() ; ++it_list ) {
	auto it_seek = it_list;
	++it_seek;
	for ( ; it_seek != compare_list.end() ; ) {
	    if ( it_list->second == it_seek->second )
		it_seek = compare_list.erase( it_seek );
	    else
		++it_seek;
	}
 
    }
    std::deque<Graph> outqueue;
    for ( auto& element : compare_list )
	outqueue.push_back( element.first );

    return outqueue;
}


void search_connected( const Vertex vertex, std::set<Vertex> &to_visit, const AdjacencyMatrix &adj_mat ) {
    // Since this function got called, we could reach the current vertex already,
    // so remove it from the set of vertices that are still to be visited.
    to_visit.erase( vertex );
    // To which vertex can we go?
    // Look for forward connected vertices
    for ( auto& v_to : adj_mat.m_matrix.at( vertex ) ) {
	if ( (v_to.second > 0) && to_visit.contains(v_to.first) )
	    search_connected( v_to.first, to_visit, adj_mat );
    }
    // Look for backward connected vertices
    for ( auto& v_from : adj_mat.m_matrix ) {
	if ( (v_from.second.at( vertex ) > 0) && to_visit.contains( v_from.first ) )
	    search_connected( v_from.first, to_visit, adj_mat );
    }
}


std::deque<Graph> remove_disconnected( const std::deque<Graph> &all_graphs ) {
    std::deque<Graph> disc_removed;
    for ( auto& graph : all_graphs ) {
	std::set<Vertex> to_visit{ graph.get_vertices() };
	AdjacencyMatrix adj_mat{ graph };
	search_connected( *(to_visit.begin()), to_visit, adj_mat );
	if ( to_visit.empty() )
	    disc_removed.push_back( graph );
    }

    return disc_removed;
}


std::deque<Graph> create_graphs( const std::set<Vertex> &vertices )
{
    // At first, we create all the possible contractions that can be created from the given vertices.
    std::deque<Edge> all_edges{ create_edges( vertices ) };
    
    // To each cluster- or physical operator are edges, that begin or end at the respective operator, mapped.
    std::map<Vertex, std::deque<Edge>> edge_map{ basic_cluster_edges( vertices, all_edges ) };

    // For each cluster vertex, a queue with all possible, unique fragments, that can be created from the
    // vertex, is created. Each fragment is paired with a set of vertices, that holds the connections which
    // are not consumed yet by contractions.
    std::map<Vertex,std::deque<std::pair<Graph, std::set<Vertex>>>>
	basic_fragments{ basic_cluster_fragments( edge_map, vertices ) };

    // These basic fragments have to be further combined, but first with themselves (e.g. T1^2,
    // two halves of T2). By doing that, we can abuse symmetry to avoid duplicate combinations.
    std::map<Vertex, std::deque<std::pair<Graph, std::set<Vertex>>>> complete_fragments;
    for ( auto& vertex : basic_fragments ) {
	std::deque<std::pair<Graph, std::set<Vertex>>> combination{
	    complete_cluster_combinations( vertex.first, vertex.second ) };
	if ( !combination.empty() )
	    complete_fragments.insert( { vertex.first, complete_cluster_combinations( vertex.first, vertex.second ) } ); 
    }

    // All we have left to do, is to combine the complete cluster fragments and lastly to combine the results
    // with the leftovers from the physical operator fragments. External operators can be ignored, since they
    // either have to contract with cluster- or physical operators or they wont yield a valid graph.
    auto it_complete_fragments{ complete_fragments.crbegin() };
    std::deque<std::pair<Graph, std::set<Vertex>>> complete_pairs{ it_complete_fragments->second };
    ++it_complete_fragments;
    for ( ; it_complete_fragments != complete_fragments.crend() ; ++it_complete_fragments ) {
	complete_pairs = asymmetric_combinations( complete_pairs, it_complete_fragments->second );
    }

    // Lastly, we have to discard incomplete graphs, i.e. those graphs, whose associated vertices (in the pair,
    // not the member) still have open connections ( their edge_table entries != 0).
    std::deque<Graph> complete_graphs;
    for ( auto& complete_pair : complete_pairs ) {
	bool loose_ends{false};
	for ( auto& vertex : complete_pair.second ) {
	   if ( vertex.connections() ) {
	       loose_ends = true;
	       break;
	   }
	}
	if ( !loose_ends )
	    complete_graphs.push_back( complete_pair.first );
    }

    std::deque<Graph> duplicates_removed{ remove_duplicates( complete_graphs ) };

    std::deque<Graph> disconnected_removed{ remove_disconnected ( duplicates_removed ) };

    return complete_graphs;
}




std::deque<Graph> pull_apart( const std::deque<Graph> &complete_graphs ) {
    std::deque<Graph> pulled_apart;
    for (auto& graph : complete_graphs ) {
	Graph new_graph;
	const std::deque<Edge>& current_edges = graph.get_edges();
	   for ( auto it_edge = current_edges.cbegin() ; it_edge != current_edges.cend() ; ) {
	       Edge first_edge{ *it_edge };
	       Edge second_edge{ *(it_edge+1) };
	       assert( first_edge.get_to() == second_edge.get_from() && "Cannot pull apart different cluster op." );
	       std::string new_name{ first_edge.get_to().get_name().substr(0,2) };

	   }
    }
    return pulled_apart;
}
