#include "rankfilter.h"
#include <deque>
#include <iostream>

// Iterative helper function.
// From a given iterator over the vertex list, check if the current vertex is physical,
// if yes, append it to the current list and keep track of rank.
//void iterative_append( std::deque<Vertex>::const_iterator iter, const std::deque<Vertex> &vertexmap,
//	std::deque<Vertex> &currentlist, int &currentrank, const int &totalrank,
//	std::deque<std::deque<Vertex>> &graphlist ) {
//    if ( ( (*iter).get_operatortype() == OperatorType::physical ) &&
//	    ( (*iter).get_name() != (*(iter-1)).get_name()) ) {
//	currentlist.push_back( *iter );
//	currentrank += (*iter).rank();
//    }
//	if ( iter != vertexmap.cend() )
//	    iterative_append( ++iter, vertexmap, currentlist, currentrank, totalrank, graphlist );
//	else if ( totalrank == currentrank ) {
//	    graphlist.push_back( currentlist );
//	}
//}
	    
 // The idea here is that for each fragment of a physical operator in a queue,
 // the next physical operator creates a copy of that queue for each of its own
 // fragments and appends the repective fragment to that queue. This results in queue that contains
 // lists (or again queues) of unique fragment combinations.
std::deque<std::deque<Vertex*>> physical_fragment_combinations(
	std::map<OperatorType,std::deque<Vertex>>& vertexmap ) {
    // We need this dummy in order to have a first entry.
    std::deque<Vertex*> dummy;
    std::deque<std::deque<Vertex*>> outqueue{ dummy };

    for ( auto& mapentry : vertexmap ) {
	if ( mapentry.first == OperatorType::physical ) {
	    // Save how many elements there were in the previous iteration.
	    int old_fragmentcount = outqueue.size();
	    // For each existing fragment in the queue, create new queues from it with a new 
	    // fragment from another operator appended to it.
	    for ( auto& fragmentlist : outqueue ) {
		for ( auto& fragment : mapentry.second ) {
		    std::deque<Vertex*> fragments_up_to_now{ fragmentlist };
		    fragments_up_to_now.push_back( &fragment );
		    outqueue.push_back( fragments_up_to_now );
		}
	    }
	    // Remove the elements from the previous iteration.
	    for ( int old_fragment = 0 ; old_fragment < old_fragmentcount ; ++old_fragment )
		outqueue.pop_front();
	}
    }
    return outqueue;
}
	    

std::deque<std::deque<Vertex>> rank_filter( std::map<OperatorType,std::deque<Vertex>> vertexmap ) {
    // Remember: Rank here is double the excitation level
    int externalrank{ 0 };
    int clusterrank { 0 };
    std::deque<std::deque<Vertex>> graphlist;
    std::deque<Vertex> externallist;
    std::deque<Vertex> clusterlist;

    // Create all possible combinations of physical operator fragments.
    // Populate external & cluster list and keep track of rank
    //
    // Find map entries for external indices
    for ( auto& mapentry : vertexmap ) {
	if ( mapentry.first == OperatorType::external ) {
	    for ( auto& vertex : mapentry.second ) {
		externalrank += vertex.rank();
		externallist.push_back( vertex );
	    }
	}
	else if ( mapentry.first == OperatorType::cluster ) {
	    for ( auto& vertex : mapentry.second ) {
	    clusterrank += vertex.rank();
	    clusterlist.push_back( vertex );
	    }
	}
    }

    int totalrank{ (-externalrank) - clusterrank };

    std::deque<std::deque<Vertex*>> fragment_combinations( physical_fragment_combinations( vertexmap ) ); 

    for ( auto& fragmentlist : fragment_combinations ) {
	// Calculate rank
	int tmprank{0};
	for ( auto& fragment : fragmentlist ) {
	    tmprank += fragment->rank();
	}
	if ( tmprank == totalrank ) {
	    std::deque<Vertex> tmpqueue{ externallist };
	    for ( auto& vertex : fragmentlist )
		tmpqueue.push_back( *vertex );
	    for ( auto& vertex : clusterlist )
		tmpqueue.push_back( vertex );
	    graphlist.push_back( tmpqueue );
	}
    }

    return graphlist;

}
