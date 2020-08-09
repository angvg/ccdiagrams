#include "parseinput.h"

using connectiontable_t = std::array<std::array<int, 2>,2>;

// Slice input string at blank characters to reveive a list of operators.
std::deque<std::string> slice_input(std::string inputstr){
    std::deque<std::string> outlist;
    std::string current;
    inputstr += " ";
    for (auto& i : inputstr){
	if ( std::isspace(i) != 0 ) {
	    outlist.push_back(current);
	    current.clear();
	    continue;
	}
	    current += i;
    }
    return outlist;
}


// Create the tables for in- and outgoing connections for the respective operator type
std::deque<connectiontable_t> create_table(OperatorType type, int degree){
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
	       tablelist.push_back(edgetable);
	   }
       }
   }
   return tablelist;
}

void append_operator(std::deque<Vertex>& operatorlist, std::string name, OperatorType type, int degree, int substlevel) {
    std::deque<connectiontable_t> tablelist{ create_table( type, substlevel) };
    for ( auto& entry : tablelist ) {
	operatorlist.push_back( Vertex(name, type, substlevel, entry ) );
    }
}



std::deque<Vertex> parse_input(std::string inputstr) {

    std::deque<Vertex> operatorlist;
    connectiontable_t exttable = { { {0,0},{0,0} } };
    std::deque<std::string> inputlist{ slice_input(inputstr) };
    
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
	exttable = { { {0,0},{substlevel,0} } };
	operatorlist.push_back( Vertex("A", OperatorType::external, 1, exttable ) );
	// Create hole annihilators
	exttable = { { {0,0},{0,substlevel} } };
	operatorlist.push_back( Vertex("I", OperatorType::external, 1, exttable ) );
    }
    
    // Create vertices for physical & cluster operators
    //
    for (auto& entry : inputlist){
	if ( entry == "Fn" ) {
		append_operator( operatorlist, entry, OperatorType::physical,1 ,1 );
	}
	else if ( entry == "Vn" ) {
		append_operator( operatorlist, entry, OperatorType::physical,2, 2 );
	}
	else if ( entry[0] == 'T' ) {
	    int degree{ static_cast<int>(entry[1]) - 48 };
	    if ( entry.size() >= 4 ) {
		substlevel = degree * (static_cast<int>(entry[3]) - 48) ;
	    }
	    append_operator (operatorlist, entry, OperatorType::cluster, degree, substlevel );
	}
    }
    
//    for (auto& i : operatorlist)
//	std::cout << i ;
//	std::cout << '\n';
    return operatorlist;
}
