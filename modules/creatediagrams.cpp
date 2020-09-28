#include "bch.h"
#include "parseinput.h"
#include "rankfilter.h"
#include "graph.h"
#include "creategraph.h"
#include "createdot.h"
#include <deque>
#include <ios>
#include <iostream>
#include <algorithm>
#include <string>


int main() {


    std::cout << "To generate CC energy and amplitude equations please enter truncation levels." << '\n';
    std::cout << "(Available options: \"S\", \"D\", \"T\", \"Q\", \"SD\", \"SDT\" , ... )" << '\n';

    std::string str_input;
    std::getline( std::cin, str_input );
    
    std::vector<CCTruncation> truncation;
    for ( auto& character : str_input ) {
	if ( character == 'S' )
	    truncation.push_back( CCTruncation::S );
	else if ( character == 'D' )
	    truncation.push_back( CCTruncation::D );
	else if ( character == 'T' )
	    truncation.push_back( CCTruncation::T );
	else if ( character == 'Q' )
	    truncation.push_back( CCTruncation::Q );
    }
    std::sort( truncation.begin(), truncation.end(), []( CCTruncation lhs, CCTruncation rhs ){ return lhs < rhs; } );


    
    Sum<Vertex> s_bch( bch_expansion( truncation ) );
    
    std::cout << "..." << '\n' << "Generating BCH-expansion of Hn = Fn + Vn for CC" << str_input << " :" << '\n';
    std::cout << s_bch << '\n' << "..." << '\n';

    std::deque<Sum<Vertex>> cc_equations;
    cc_equations.push_back( s_bch );
    for ( auto& t : truncation ) {
	Sum<Vertex> s_tmp_bch{ s_bch };	
	s_tmp_bch.mulitply_left( create_product( t + 1 ) );
	cc_equations.push_back( s_tmp_bch );
    }
    
    std::deque<std::deque<std::map<OperatorType,std::deque<Vertex>>>> parsed;
    for ( auto& equation : cc_equations ) {
	std::deque<std::map<OperatorType,std::deque<Vertex>>> parsed_equation;
	for ( auto& product : equation ) {
	    parsed_equation.push_back( parse_input( product ) );
	}
	parsed.push_back( parsed_equation );
    }

    std::deque<std::deque<std::deque<std::set<Vertex>>>> filtered_by_rank;
    for ( auto& equation : parsed ) {
	std::deque<std::deque<std::set<Vertex>>> filtered_equation;
	for ( auto& entry : equation ) {
	    filtered_equation.push_back( rank_filter( entry ) );
	}
	filtered_by_rank.push_back( filtered_equation );
    }

    std::deque<std::deque<Graph>> final_graphs;
    // For every cc equation
    for ( auto& equation : filtered_by_rank ) {
	std::deque<Graph> final_equation;
	// For every product in the bch-series
	for ( auto& bch_product : equation ) {
	    // For every combination of physical operator fragments
	    for ( auto& phys_comb : bch_product ) {
		std::deque<Graph> all_graphs{ create_graphs( phys_comb ) };
		//std::deque<Graph> duplicates_removed{ remove_duplicates ( all_graphs ) };
		std::deque<Graph> disconnected_removed{ remove_disconnected( all_graphs ) };
		for ( auto& graph : disconnected_removed )
		    final_equation.push_back( graph );
	    }
	}
	final_graphs.push_back( final_equation );
    }

    // Remove duplicates
    std::deque<std::deque<Graph>> duplicates_removed;
    for ( auto& equation : final_graphs ) {
	duplicates_removed.push_back( remove_duplicates( equation ) );
    }

    
    // Print

    auto it_equation{ duplicates_removed.cbegin() };
    std::cout << "Diagrams for CC" << str_input << "-energy equation (total "
	<< it_equation->size() << "):" << '\n' << '\n';
    for ( auto& graph : *it_equation ) {
		std::cout << '\t' << "Diagram for: "; graph.print_vertices();
		std::cout << '\n' << '\t' << '\t' ;
		graph.print_edges();
		std::cout << '\n';
    }
    std::cout << '\n';
    
    ++it_equation;
    int input_ctr{0};
    
    for ( ; it_equation != duplicates_removed.cend() ; ++it_equation, ++input_ctr ) {
    std::cout << "Diagrams for CC" << str_input << "-amplitude equation: " << str_input.at(input_ctr) 
	<< " - excitation (total " << it_equation->size() << ")" << '\n' << '\n';
	for ( auto& graph : *it_equation ) {
		    std::cout << '\t' << "Diagram for: "; graph.print_vertices();
		    std::cout << '\n' << '\t' << '\t';
		    graph.print_edges();
		    std::cout << '\n';
	}
	std::cout << '\n';
    }
    std::cout << '\n';
    

    auto it_dot{ duplicates_removed.cbegin() };
    create_dot( *it_dot, "dot/0_Energy.dot" );
    std::cout << "Created .dot file: dot/0_Energy.dot" << '\n'; 
    ++it_dot;

    int dotctr{1};
    for ( ; it_dot != duplicates_removed.cend() ; ++it_dot, ++dotctr ) {
	std::string filename = "dot/" + std::to_string( dotctr ) + "_Amplitude_" + str_input.at( dotctr - 1) + ".dot";
	create_dot( *it_dot, filename );
	std::cout << "Created .dot file: " << filename << '\n'; 
    }
    std::cout << "For graphical rendering use \"make render\" (graphviz install required)." << '\n'; 

    return 0;
}
