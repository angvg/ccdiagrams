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

    std::string inputstr;
    std::cout << '\n' << "Enter reference determinant substitution level followed by operator sequence:" << '\n';
    std::cout << '\t' << "Substitution level: positive integer" << '\n';
    std::cout << '\t' << "Physical Operators: Fn Vn " << '\n';
    std::cout << '\t' << "Cluster Operators: T1 T2 T1^2 ... " << '\n';
    std::cout << "Sequence: " ;
    std::getline(std::cin, inputstr);

    // Parse input, create a list for each operator and each type of physical operator fragment.
    std::map<OperatorType,std::deque<Vertex>> parsed_map{ parse_input( inputstr ) };
    
    // Filter physical operator vertices out according to rank.
    std::deque<std::set<Vertex>> filtered_by_rank{ rank_filter( parsed_map ) };
    if ( filtered_by_rank.empty() ) {
	std::cout << "No nonzero expression possible according to rank!";
	return 0;
    }

    // Create graphs for each different physical operator fragment.
    std::deque<std::deque<Graph>> final_graphs;
    for ( auto& graphs : filtered_by_rank )
	final_graphs.push_back( create_graphs( graphs ) );


    std::cout << '\n' << "Coupled Cluster Diagrams for: " << inputstr << '\n';
    int fragm_ctr = 1;
    for ( auto graphs : final_graphs ) {
	std::cout << '\t' << "Diagrams for " << fragm_ctr << ". combination of physical operator fragments:" << '\n';
	int dgrm_ctr{1};
	for ( auto& graph : graphs ) {
	    std::cout << '\t' << dgrm_ctr << " - " ;
	    graph.print_edges();
	    ++dgrm_ctr;
	}
	std::cout << '\n';
	++fragm_ctr;
    }
    std::cout << '\n';

    // Create .dot files
    std::string filename;
    fragm_ctr = 1;
    for ( auto& graphs : final_graphs ) {
	filename = "dot/diagrams_" + std::to_string(fragm_ctr) + ".dot";
	create_dot( graphs, filename );
	std::cout << "Graph written to: " << filename << '\n';
	++fragm_ctr;
    }

   return 0;
}
