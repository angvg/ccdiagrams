#include "bch.h"
#include <string>


Sum<Vertex> merge_operators( const Sum<Vertex>& sum, const std::deque<Vertex>& cluster_operators ) {

    Sum<Vertex> s_merged;
    std::vector<int> op_ctr( cluster_operators.size(), 0 );
    
    for ( auto it_sum{sum.cbegin()} ; it_sum != sum.cend() ; ++it_sum ) {
	   Product<Vertex> p_merged;
	   p_merged.push_front( it_sum->front() );
	   
	   for ( auto& op : cluster_operators ) {
		int ctr = it_sum->count( op ) ;
		if ( ctr == 0 )
		    continue;
		else if ( ctr == 1 ) {
		    p_merged.push_back( op );
		    continue;
		}
		std::string name{ op.get_name() + "^" + std::to_string(ctr) };
		int degree = ctr + static_cast<int>( name.at(1) ) - 48 ;

		p_merged.push_back( { name, OperatorType::cluster, degree } );
	   }
	   s_merged.push_back( p_merged );
    }

    return s_merged;
}


Sum<Vertex> bch_expansion( std::vector<CCTruncation> trunc ) {

    Vertex kin{ "Fn", OperatorType::physical } ;
    Vertex pot{ "Vn", OperatorType::physical } ;

    Sum<Vertex> expanded;

    std::deque<Vertex> cluster_operators;

    for ( auto& t : trunc ) {
	switch (t) {
	    case 0 :
		cluster_operators.push_back( { "T1" , OperatorType::cluster, 1 } );
		break;
	    case 1 :
		cluster_operators.push_back( { "T2" , OperatorType::cluster, 2 } );
		break;
	    case 2 :
		cluster_operators.push_back( { "T3" , OperatorType::cluster, 3 } );
		break;
	    case 3 :
		cluster_operators.push_back( { "T4" , OperatorType::cluster, 4 } );
		break;
	}
    }

    Product<Vertex> p_kin;
    p_kin.push_back( kin );

    Product<Vertex> p_pot;
    p_pot.push_back( pot );

    expanded.push_back( p_kin );
    expanded.push_back( p_pot );

    Sum<Vertex> s_prev; 
    s_prev.push_back( p_kin );
    s_prev.push_back( p_pot );
    Sum<Vertex> s_next;

    for ( int trunc = 1 ; trunc < 5 ; ++trunc ) {
	for ( auto& product : s_prev ) {
	    // Fn and Vn can interrelate 2 and 4 cluster operators respectively at max
	    if ( (product.front().get_name() == "Fn" && product.size() > 2) ||
		    (product.front().get_name() == "Vn" && product.size() > 4) )
		continue;

	    for ( auto& op : cluster_operators ) {
		Product<Vertex> tmp_product{ product };
		tmp_product.push_back( op );
		expanded.push_back( tmp_product );
		s_next.push_back( tmp_product );
	    }
	}
	s_prev.clear();
	s_prev = s_next;
	s_next.clear();
	//expanded.append( s_kin_prev ); 
    //std::cout << "Expanded: " << expanded << '\n';
    }
    
    Sum<Vertex> expanded_merged{ merge_operators( expanded, cluster_operators ) };

    return expanded_merged;
}
