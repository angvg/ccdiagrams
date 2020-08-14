#include "vertex.h"
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
    
    std::deque<std::string> testinputs{
	"1 Fn T1^2",
	"2 Vn T1^2 T2"
	};
    
    std::deque<std::map<OperatorType,std::deque<Vertex>>> testmaps;
    std::deque<std::deque<std::deque<Vertex>>> testgraphs;
    for ( auto& teststrings : testinputs ) {
	std::map<OperatorType,std::deque<Vertex>> tmpmap( parse_input( teststrings ) );
	testmaps.push_back(  tmpmap );
	testgraphs.push_back( rank_filter( tmpmap ) ); 
    }
    std::cout << "-----------------" << '\n' ;
    for ( auto& testmap : testmaps ) {
	std::cout << "Input vertexlist: ";
	for ( auto& vertexqueue : testmap ) {
	    for ( auto& vertex : vertexqueue.second )
		std::cout << vertex << "{" << vertex.rank() << "} " ;
	}
    std::cout << '\n' << "-----------------" << '\n'; 
    }

    std::cout << "Vertex lists filtered by rank:" << '\n' << "-----------------" << '\n' ;
    for ( auto& graph : testgraphs ) {
	for ( auto& vertexlist : graph ) {
	    for ( auto& vertex : vertexlist ) {
	    std::cout << vertex << "{" << vertex.rank() << "} " ;
	    }
	std::cout << '\n' << "-----------------" << '\n';
	}
    }

    std::deque<std::deque<Graph>> realgraphs;
    for ( auto& testgraph : testgraphs ) {
	for ( auto& vertexlist : testgraph ) {
	    realgraphs.push_back( create_graph( vertexlist ) );
	}
    }
	
    // create_graph( parse_input(input) );
    return 0;
}
