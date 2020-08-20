#include "vertex.h"
#include "edge.h"
#include "parseinput.h"
#include "creategraph.h"
#include "rankfilter.h"
#include <iostream>
#include <string>
#include <deque>
#include <map>

int main(){

    // Test inputparser function
//    std::string input{ "1 Fn" };
//    std::deque<std::string> inputlist;
//    std::cout << "Please Enter Substitution level followed by operator sequence:" << '\n';
//    std::getline(std::cin, input);
    
    std::deque<std::string> inputstrings{
	"1 Fn T1^2",
//	"2 Vn T1^2 T2",
	"1 Vn T1"
	};
    
    // Parse input, create a list for each type of physical operator fragment.
    std::deque<std::map<OperatorType,std::deque<Vertex>>> inputmaps;
    for ( auto& inputstring : inputstrings )
	inputmaps.push_back( parse_input( inputstring ) );
    
    // Filter physical operator vertices out according to rank.
    std::deque<std::deque<std::deque<Vertex>>> filtered_by_rank;
    for ( auto& inputmap :inputmaps )
	filtered_by_rank.push_back( rank_filter( inputmap ) );

    // Create all edges for each vertexlist.
    std::deque<std::deque<std::deque<Edge>>> all_edges;
    for ( auto& inputs : filtered_by_rank ) {
	std::deque<std::deque<Edge>> input_edges;
	for ( auto& vertexlist : inputs )
	    input_edges.push_back( create_edges( vertexlist ) );
	all_edges.push_back( input_edges );
    }

    // Collect edges from or to cluster operators
    std::deque<std::deque<std::map<Vertex,std::deque<Edge>>>> cluster_edges;
    auto input_filtered_by_rank{ filtered_by_rank.cbegin() };
    auto input_all_edges{ all_edges.cbegin() };
    // iterate over inputs
    for ( ; input_filtered_by_rank != filtered_by_rank.cend() && input_all_edges != all_edges.cend() ; 
	    ++input_filtered_by_rank, ++input_all_edges ) {
	std::deque<std::map<Vertex,std::deque<Edge>>> input_cluster_edges;
	auto vertexlists{ input_filtered_by_rank->cbegin() };
	auto edgelists{ input_all_edges->cbegin() };
	// iterate over operator combinations
	for (  ; vertexlists != input_filtered_by_rank->cend() ; ++vertexlists, ++ edgelists ) {
	    input_cluster_edges.push_back( basic_cluster_edges( *vertexlists, *edgelists ) );
	}
	cluster_edges.push_back( input_cluster_edges );
    }

    // Generate basic cluster fragments
    std::deque<std::deque<std::map<Vertex,std::deque<Graph>>>> cluster_fragments;
    for ( auto& input_cluster_edge : cluster_edges ) {	// iterate over inputs
	std::deque<std::map<Vertex,std::deque<Graph>>> input_cluster_fragments;
	for ( auto& physical_comb : input_cluster_edge ) {	// iterate over differing physical op. fragments
	    input_cluster_fragments.push_back( basic_cluster_fragments( physical_comb ) );
		    }
		cluster_fragments.push_back ( input_cluster_fragments );    
	    }
    

    // Print
    std::cout << '\n';
    for ( std::size_t current{0} ; current < inputstrings.size() ; ++current ) {
	std::cout << "Input string: " << '\n' << '\t' << inputstrings[current] << '\n';
	std::cout << "Parsed Input: " << '\n' << '\t'; 
	for ( auto& inputmap : inputmaps.at( current ) ) {
	    for ( auto& parse_vertex : inputmap.second ) {
		std::cout << parse_vertex << "{" << parse_vertex.rank() << "} ";
	    }
	}
	std::cout << '\n';
	std::cout << "Input filtered by rank: " << '\n';
	for ( auto& rank_vertices : filtered_by_rank.at( current ) ) {
	    std::cout << '\t';
	    for ( auto& rank_vertex : rank_vertices ) {
		std::cout << rank_vertex << "{" << rank_vertex.rank() << "} ";
		}
		std::cout << '\n';
		}
	std::cout << "All possible edges:" << '\n';
	for ( auto& edgeslist : all_edges.at( current ) ) {
	    std::cout << '\t';
	    for ( auto& edge : edgeslist )
		std::cout << edge;
	    std::cout << '\n';
	}
	std::cout << "Egdes from- or to cluster operators:" << '\n';
	for ( auto& input_cluster_edges : cluster_edges.at(current) ) { // iterate over maps 
	    for ( auto& map_cluster_edges : input_cluster_edges ) { // iterate over map elements
		std::cout << '\t' << map_cluster_edges.first << ": " ;
		for ( auto& cluster_edge : map_cluster_edges.second ) { // iterate over edges in map value 
		    std::cout << cluster_edge;
		}
		std::cout << '\n';
	    }
	}
	std::cout << "Basic cluster operator fragments:" << '\n';
	for ( auto& input_cluster_fragments : cluster_fragments.at(current) ) {	// iterate over inputs
	    for ( auto& physical_comb : input_cluster_fragments ) {		// iterate over diff. phys. op.
		std::cout << '\t';
		for ( auto& graph : physical_comb.second ) {			// iterate over graphs map
		    std::cout << physical_comb.first << ": ";
		    graph.print_edges();
		    std::cout << " | ";
		}
		std::cout << '\n';
	    }
	}
	std::cout << '\n';
    }
	

    return 0;
}
