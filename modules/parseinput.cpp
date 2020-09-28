#include "parseinput.h"

using connectiontable_t = std::array<std::array<int, 2>,2>;

// Slice input string at blank characters to receive a list of operators.
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

// Create sum from sliced input
Product<Vertex> create_product( std::deque<std::string> sliced_input )
{
    Product<Vertex> p_out;
    // Create vertices for external lines
    //
    // Assert that the first entry is a number
    connectiontable_t exttable = { { {0,0},{0,0} } };
    assert(std::regex_match(sliced_input[0], std::regex("(\\+|-)?[0-9]*(\\.?([0-9]+))$"))
		&& "Invalid Input: first entry not a number");
    int substlevel{ std::stoi( sliced_input[0] ) };
    if ( substlevel > 0 ){
	// Create particle annihilators
	exttable = { { {0,0},{substlevel,0} } };
	p_out.push_back( Vertex("A", OperatorType::external, 1, exttable ) );
	// Create hole annihilators
	exttable = { { {0,0},{0,substlevel} } };
	p_out.push_back( Vertex("I", OperatorType::external, 1, exttable ) );
    }
    
    // Append physical and cluster operators
    auto it_queue{ sliced_input.begin() };
    ++it_queue;
    for ( ; it_queue != sliced_input.end() ; ++it_queue ) {
	p_out.push_back( { *it_queue, OperatorType::undefined } );
    }
    return p_out;
}


Product<Vertex> create_product( int substlevel )
{
    Product<Vertex> p_out;
    connectiontable_t exttable = { { {0,0},{0,0} } };
    if ( substlevel > 0 ){
	// Create particle annihilators
	exttable = { { {0,0},{substlevel,0} } };
	p_out.push_back( Vertex("A", OperatorType::external, 1, exttable ) );
	// Create hole annihilators
	exttable = { { {0,0},{0,substlevel} } };
	p_out.push_back( Vertex("I", OperatorType::external, 1, exttable ) );
    }
    return p_out;
}


// Create the tables for in- and outgoing connections for the respective operator type
std::deque<connectiontable_t> create_table(OperatorType type, int degree)
{
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

// Instantiates and appends an operator to the supplied operatorlist.
// For physical operators, multiple instances for the differing fragemts are created
// by calling create_table.
void append_operator(std::deque<Vertex>& operatorlist, std::string name, OperatorType type, int degree, int substlevel)
{
    std::deque<connectiontable_t> tablelist{ create_table( type, substlevel) };
    for ( auto& entry : tablelist ) {
	operatorlist.push_back( Vertex(name, type, substlevel, entry ) );
    }
}



std::map<OperatorType,std::deque<Vertex>> parse_input( Product<Vertex>& input_product )
{
    std::map<OperatorType,std::deque<Vertex>> outmap;
    std::deque<Vertex> operatorlist;

    // Create vertices for external lines
    for ( auto& vertex : input_product ) {
	if ( vertex.get_operatortype() == OperatorType::external ) {
	    operatorlist.push_back( vertex );
	}
    }
    outmap.insert( { OperatorType::external , operatorlist } );
    operatorlist.clear();

    
    // Create vertices for physical  operators
    // Each physical operator receives its own deque, that contains all the
    // possible fragments. The deque then is stored in the outmap.
    // Should there ever be need for additional physical operators, here is the place to
    // expand the input parsing function.
    for (auto& vertex : input_product ){
	if ( vertex.get_name() == "Fn" ) {
		append_operator( operatorlist, vertex.get_name(), OperatorType::physical,1 ,1 );
		outmap.insert( { OperatorType::physical, operatorlist } );
		operatorlist.clear();
	}
	else if ( vertex.get_name() == "Vn" ) {
		append_operator( operatorlist, vertex.get_name(), OperatorType::physical,2, 2 );
		outmap.insert( { OperatorType::physical, operatorlist } );
		operatorlist.clear();
	}
    }

    // Create vertices for cluster operators. Since the fragments don't vary, we can just
    // put them all in one deque and append it to the outmap.
    int substlevel{0};
    for (auto& vertex : input_product ) {
	std::string name{ vertex.get_name() };
	if ( name[0] == 'T' ) {
	    int degree{ static_cast<int>(name[1]) - 48 };
	    if ( name.size() >= 4 )
		substlevel = degree * (static_cast<int>(name[3]) - 48) ;
	    else
		substlevel = degree;
	    append_operator (operatorlist, name, OperatorType::cluster, degree, substlevel );
	}
    }
    outmap.insert( { OperatorType::cluster, operatorlist } );
    
//    for (auto& i : operatorlist)
//	std::cout << i ;
//	std::cout << '\n';
    return outmap;
}
