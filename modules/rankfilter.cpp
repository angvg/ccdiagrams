#include "rankfilter.h"
#include <deque>
#include <iostream>

	    
 // The idea here is that for each fragment of a physical operator in a queue,
 // the next physical operator creates a copy of that queue for each of its own
 // fragments and appends the repective fragment to that queue. This results in queue that contains
 // lists (or again queues) of unique fragment combinations.
std::deque<std::set<Vertex*>> physical_fragment_combinations(
	std::map<OperatorType,std::deque<Vertex>>& vertexmap ) {
    // We need this dummy in order to have a first entry.
    std::set<Vertex*> dummy;
    std::deque<std::set<Vertex*>> outqueue{ dummy };

    for ( auto& mapentry : vertexmap ) {
	if ( mapentry.first == OperatorType::physical ) {
	    // Save how many elements there were in the previous iteration.
	    int old_fragmentcount = outqueue.size();
	    // For each existing fragment in the queue, create new queues from it with a new 
	    // fragment from another operator appended to it.
	    for ( auto& fragmentset : outqueue ) {
		for ( auto& fragment : mapentry.second ) {
		    std::set<Vertex*> fragments_up_to_now{ fragmentset };
		    fragments_up_to_now.insert( &fragment );
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
	    

std::deque<std::set<Vertex>> rank_filter( std::map<OperatorType,std::deque<Vertex>> vertexmap ) {
    // Remember: Rank here is double the excitation level
    int externalrank{ 0 };
    int clusterrank { 0 };
    std::deque<std::set<Vertex>> unique_combinations;
    std::set<Vertex> externallist;
    std::set<Vertex> clusterlist;

    // Create all possible combinations of physical operator fragments.
    // Populate external & cluster list and keep track of rank
    //
    // Find map entries for external indices
    for ( auto& mapentry : vertexmap ) {
	if ( mapentry.first == OperatorType::external ) {
	    for ( auto& vertex : mapentry.second ) {
		externalrank += vertex.rank();
		externallist.insert( vertex );
	    }
	}
	else if ( mapentry.first == OperatorType::cluster ) {
	    for ( auto& vertex : mapentry.second ) {
	    clusterrank += vertex.rank();
	    clusterlist.insert( vertex );
	    }
	}
    }

    int totalrank{ (-externalrank) - clusterrank };

    // Create a queue of all possible physical operator fragment combinations
    std::deque<std::set<Vertex*>> fragment_combinations( physical_fragment_combinations( vertexmap ) ); 

    // Now we need to weed out the combinations with a wrong rank.
    for ( auto& fragmentset : fragment_combinations ) {
	// Calculate rank
	int tmprank{0};
	for ( auto& fragment : fragmentset ) {
	    tmprank += fragment->rank();
	}
	if ( tmprank == totalrank ) {
	    // Ranks fits. Now concatenate queues of external, physical and cluster operators.
	    std::set<Vertex> tmpset{ externallist };
	    for ( auto& vertex : fragmentset )
		tmpset.insert( *vertex );
	    for ( auto& vertex : clusterlist )
		tmpset.insert( vertex );
	    unique_combinations.push_back( tmpset );
	}
    }

    return unique_combinations;

}
