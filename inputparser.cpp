#include "inputparser.h"

using connectiontable_t = std::array<std::array<int, 2>,2>;

// Slice input string at blank characters to reveive a list of operators.
std::deque<std::string> sliceinput(std::string inputstr){
    std::deque<std::string> outlist;
    std::string current;
    inputstr += " ";
    // std::cout << "Input string: " << inputstr << '\n';
    for (auto& i : inputstr){
	// std::cout << "Current string iterator: " << i << '\n';
	if ( std::isspace(i) != 0 ) {
	    // std::cout << "Blank found." << '\n';
	    outlist.push_back(current);
	    current.clear();
	    continue;
	}
	    // std:: cout << "Non-blank character found: " << i << '\n';
	    current += i;
    }
    // std::cout << "Printing outlist: " << '\n';
    // for (auto& i : outlist)
    //     std::cout << i << " ";
    // std::cout << '\n';
    return outlist;
}


// Create the tables for in- and outgoing connections for the respective operator type
std::deque<connectiontable_t> createtable(OperatorType type, int degree){
    std::deque<connectiontable_t> tablelist;
    connectiontable_t edgetable{ { {0,0},{0,0} } };  
   if ( type == OperatorType::cluster ){
	    edgetable[0][0] = degree;
	    edgetable[0][1] = degree;
	    tablelist.push_back(edgetable);
   }
   if ( type == OperatorType::physical ) {
       for ( int i = 0 ; i <= degree ; ++i ) {
	   for ( int j = 0 ; j <= degree ; ++j ) {
	       edgetable[0][0] = i;
	       edgetable[0][1] = j;
	       edgetable[1][0] = degree-i;
	       edgetable[1][1] = degree-j;
	   }
       }
   }
   return tablelist;
}


std::deque<Vertex> inputparser(std::string inputstr) {

    std::deque<Vertex> operatorlist;
    connectiontable_t exttable = { { {0,0},{0,0} } };
    std::deque<std::string> inputlist{ sliceinput(inputstr) };
    
    std::cout << "Inputlist: " << inputstr << '\n';
    std::cout << "Sliced list: ";
    for (auto& i : inputlist)
	std::cout << i << " ";
    std::cout << '\n';

    // Create vertices for external lines
    //
    // Assert that the first entry is a number
    assert(std::regex_match(inputlist[0], std::regex("(\\+|-)?[0-9]*(\\.?([0-9]+))$"))
		&& "Invalid Input: first entry not a number");
    int substlevel{ std::stoi( inputlist[0] ) };
    if ( substlevel > 0 ){
	// Create particle annihilators
	exttable = { { {substlevel,0},{0,0} } };
	operatorlist.push_back( Vertex("A", OperatorType::external, 1, exttable ) );
	// Create hole annihilators
	exttable = { { {0,substlevel},{0,0} } };
	operatorlist.push_back( Vertex("I", OperatorType::external, 1, exttable ) );
    }
    
    // Create vertices for physical operators
    //
    for (auto& entry : inputlist){
	if ( entry == "Fn" ) {
	    std::cout << "Found Fn!" << '\n';
	    std::deque<connectiontable_t> tablelist{ createtable( OperatorType::physical, 1 ) };
	    for ( auto& entry : tablelist ) 
		operatorlist.push_back( Vertex("Fn", OperatorType::physical, 1, entry ) );
	}
    }
    
    for (auto& i : operatorlist)
	std::cout << i ;
    std::cout << '\n';
    return operatorlist;
}
