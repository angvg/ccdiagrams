#include "vertex.h"
#include "edge.h"
#include "parseinput.h"
#include "creategraph.h"
#include "rankfilter.h"
#include "createdot.h"
#include <iostream>
#include <string>
#include <deque>
#include <map>

int main(){

    
std::deque<std::string> inputstrings{
	"1 Fn",
	"2 Vn",
	"Fn T1",
	"1 T1 Fn T1"
	"1 Fn T1^2",
	"1 Vn T1 T2",
	"2 Vn T1^2 T2",
	"1 Vn T1",
	"2 Vn T2 T1"
	};

int input_ctr{1};    

    // For every inputstring
for ( auto& inputstring : inputstrings ) {

    std::deque<std::string> sliced_input{ slice_input( inputstring ) };

    Product<Vertex> input_product{ create_product( sliced_input ) };

    // Parse input, create a list for each operator and each type of physical operator fragment.
    std::map<OperatorType,std::deque<Vertex>> parsed_map{ parse_input( input_product ) };
    
    // Filter physical operator vertices out according to rank.
    std::deque<std::set<Vertex>> filtered_by_rank{ rank_filter( parsed_map ) };

    // Create all edges for each vertexlist.
    std::deque<std::deque<Edge>> all_edges;
	for ( auto& vertexlist : filtered_by_rank )
	all_edges.push_back( create_edges( vertexlist ) );
    

    // Collect edges from or to cluster operators
    std::deque<std::map<Vertex,std::deque<Edge>>> cluster_edges;
    auto it_filtered_by_rank = filtered_by_rank.cbegin();
    auto it_all_edges = all_edges.cbegin();
    // iterate over different physical operator fragments
    for ( ; it_filtered_by_rank != filtered_by_rank.cend() && it_all_edges != all_edges.cend() ; 
	    ++it_filtered_by_rank, ++it_all_edges ) {
	cluster_edges.push_back( basic_cluster_edges( *it_filtered_by_rank, *it_all_edges ) );
    }

    // Generate basic cluster fragments
    std::deque<std::map<Vertex,std::deque<std::pair<Graph, std::set<Vertex>>>>> basic_cluster_fragments_queue;
    it_filtered_by_rank = filtered_by_rank.cbegin();
    auto it_edges_map = cluster_edges.cbegin();
    for ( ; it_edges_map != cluster_edges.cend() ; ++it_filtered_by_rank, ++ it_edges_map )
	basic_cluster_fragments_queue.push_back( basic_cluster_fragments( *it_edges_map, *it_filtered_by_rank  ) );

    // Generate complete cluster fragments
    std::deque<std::map<Vertex, std::deque<std::pair<Graph, std::set<Vertex>>>>> complete_cluster_combinations_queue;
    it_filtered_by_rank = filtered_by_rank.cbegin();
    for ( auto& physical_comb : basic_cluster_fragments_queue ) {
	std::map<Vertex, std::deque<std::pair<Graph, std::set<Vertex>>>> phys_compl;
	for ( auto& mapentry : physical_comb ) {
	    // For each cluster operator generate a complete set of fragment combinations.
//	    if ( mapentry.first.get_operatortype() != OperatorType::cluster )
//		continue;
	    std::deque<std::pair<Graph, std::set<Vertex>>> op_combinations{ complete_cluster_combinations( mapentry.first, mapentry.second ) };
	    phys_compl.insert( { mapentry.first, op_combinations } );  
	}
	complete_cluster_combinations_queue.push_back( phys_compl );
    }

    // Generate complete graphs
    std::deque<std::deque<Graph>> complete_graphs;
    for ( auto& physical_comb : filtered_by_rank ) {
	complete_graphs.push_back( create_graphs( physical_comb ) );
    }
    
    // Combine different physical op. fragments
    std::deque<Graph> combined_graphs;
    for ( auto& physical_comb : complete_graphs ) {
	for ( auto& graph : physical_comb )
	    combined_graphs.push_back( graph );
    }


    // Remove identical
    std::deque<Graph> identical_removed{ remove_duplicates( combined_graphs ) };

    // Remove identical
    std::deque<Graph> disconnected_removed{ remove_disconnected( identical_removed ) }; 

    
    // Print to files
    std::string filename_all = "dot/" + std::to_string(input_ctr)  + "_all_demo.dot" ;
    std::string filename_unique = "dot/" + std::to_string(input_ctr)  + "_unique_demo.dot" ;
    std::string filename_connected = "dot/" + std::to_string(input_ctr)  + "_unique_connected_demo.dot" ;
    ++input_ctr;

    create_dot( combined_graphs, filename_all );
    create_dot( identical_removed, filename_unique );
    create_dot( disconnected_removed, filename_connected );

    // Print to console
    std::cout << '\n';
	std::cout << "Input string: " << '\n' << '\t' << inputstring << '\n';

	std::cout << "Sliced Input: " << '\n' << '\t';
	for ( auto& sliced : sliced_input ) {
	    std::cout << sliced << " ";
	}
	std::cout << '\n';

	std::cout << "Parsed Input: " << '\n' << '\t'; 
	for ( auto& mapentry : parsed_map ) {
	    for ( auto& parse_vertex : mapentry.second ) {
		std::cout << parse_vertex << "{" << parse_vertex.rank() << "} ";
	    }
	}

	int frgmt_ctr{1};
	std::cout << '\n';
	std::cout << "Input filtered by rank: " << '\n';
	for ( auto& rank_vertices : filtered_by_rank ) {
	    std::cout << '\t' << frgmt_ctr << " - ";
	    for ( auto& rank_vertex : rank_vertices ) {
		std::cout << rank_vertex << "{" << rank_vertex.rank() << "} ";
		}
		std::cout << '\n';
		++frgmt_ctr;
	}
	frgmt_ctr = 1;
	std::cout << "All possible edges:" << '\n';
	for ( auto& edgeslist : all_edges ) {
	    std::cout << '\t' << frgmt_ctr << " - ";
	    for ( auto& edge : edgeslist )
		std::cout << edge;
	    std::cout << '\n';
	    ++frgmt_ctr;
	}
	frgmt_ctr = 1;
	std::cout << "Egdes from- or to cluster operators:" << '\n';
	for ( auto& map_cluster_edges : cluster_edges ) { // iterate over maps 
	    for ( auto& mapentry : map_cluster_edges ) { // iterate over map elements
		std::cout << '\t' << frgmt_ctr << " - " << mapentry.first << ": " ;
		for ( auto& cluster_edge : mapentry.second ) { // iterate over edges in map value 
		    std::cout << cluster_edge;
		}
		std::cout << '\n';
	    }
	    //std::cout << '\n';
	    ++frgmt_ctr;
	}
	frgmt_ctr = 1;
	std::cout << "Basic cluster operator fragments:" << '\n';
	for ( auto& fragment_map : basic_cluster_fragments_queue ) {		// iterate over diff. phys. op.
	    for ( auto& mapentry : fragment_map ) {				// iterate map entries 
		std::cout << '\t' << frgmt_ctr << " - ";
		for ( auto& graph : mapentry.second ) {			// iterate over queue in map
		    std::cout << mapentry.first << ": ";
		    graph.first.print_edges();
		    std::cout << " | ";
		}
		std::cout << '\n';
	    }
	    ++frgmt_ctr;
	}
	frgmt_ctr = 1;
	std::cout << "Complete cluster fragments:" << '\n';
	for ( auto& cluster_map : complete_cluster_combinations_queue ) {
	    for ( auto& mapentry : cluster_map )  {
		std::cout << '\t' << frgmt_ctr << " - " << mapentry.first << ":" << '\n' << '\t';
		for ( auto& graph : mapentry.second ) {
		    //std::cout << '\t';
		    graph.first.print_edges() ; std::cout << " | ";
		}
		std::cout << '\n';
	    }
	    std::cout << '\n';
	    ++frgmt_ctr;
	}
	std::cout << '\n';

	frgmt_ctr = 1;
	std::cout << "Complete graphs: " << '\n';
	for ( auto& graphs : complete_graphs ) {
	    if ( graphs.empty() )
		continue;
	    //std::cout << '\t' << "#Graphs for current physical operator fragment: " << graphs.size() << '\n';
	    for ( auto& graph : graphs ) {
		std::cout << '\t' << frgmt_ctr << " - " ; graph.print_vertices() ;
		std::cout << ": " ; graph.print_edges();
		std::cout << '\n';
	    }
	    std::cout << '\n' ;
	    ++frgmt_ctr;
	    }
	std::cout << '\n';
    

	frgmt_ctr = 1;
	std::cout << "Combined Graphs:" << '\n' ;
	for ( auto& graph: combined_graphs ) {
		std::cout << '\t'  ; graph.print_vertices();
		std::cout << ": " ; graph.print_edges();
		std::cout << '\n';

		++frgmt_ctr;
	}	
	std::cout << '\n' ;

	frgmt_ctr = 1;
	std::cout << "Identical Graphs removed:" << '\n' ;
	for ( auto& graph: identical_removed ) {
		std::cout << '\t'  ; graph.print_vertices();
		std::cout << ": " ; graph.print_edges();
		std::cout << '\n';

		++frgmt_ctr;
	}	
	std::cout << '\n' ;

	frgmt_ctr = 1;
	std::cout << "Disconnected Graphs removed:" << '\n' ;
	for ( auto& graph: disconnected_removed ) {
		std::cout << '\t'  ; graph.print_vertices();
		std::cout << ": " ; graph.print_edges();
		std::cout << '\n';

		++frgmt_ctr;
	}	
	std::cout << '\n' ;
}

    return 0;
}
