#include "createdot.h"


void create_dot( const std::deque<Graph> &graphs, std::string filename ) {

    std::ofstream outf{ filename, std::ios::trunc };

    int graph_ctr{0};

    outf << "digraph {" << '\n';
    outf << "graph [splines=true] " << '\n';
    outf << "node [ shape=circle, fixedsize=true, margin=0.5, height=0.3, fontsize=8 ]" << '\n';
    outf << "edge [ arrowhead=normal, arrowsize=0.4 ] " << '\n';

    for ( auto& graph : graphs ) {

	outf << '\t' << "subgraph cluster_" << graph_ctr << " {" << '\n';

	outf << '\t' << "label = <" << graph.vertices_html() << ">;" << '\n';

	outf << '\t' << "{" << '\n';

	// Collect external vertices
	outf << '\t' << "{ rank = source; ";	
	for ( auto& vertex : graph.get_vertices() ) {
	    if ( vertex.get_operatortype() == OperatorType::external ) {
		outf << "\"" << vertex << std::to_string(graph_ctr) << "\"" ;
		outf << " [label=\"" << vertex.get_name() << "\"];" << '\n';
	    }
	}
	outf << " }" << '\n' ;	

	// Collect physical vertices
	outf << '\t' << "{ rank = same; ";	
	for ( auto& vertex : graph.get_vertices() ) {
	    if ( vertex.get_operatortype() == OperatorType::physical ) {
		outf << "\"" << vertex << std::to_string(graph_ctr) << "\"" ;
		outf << " [label=<" << vertex.get_name_html() << ">];" << '\n';
	    }
	}
	outf << " }" << '\n';	

	// Collect cluster vertices
	outf << '\t' << "{ rank = sink; ";	
	for ( auto& vertex : graph.get_vertices() ) {
	    if ( vertex.get_operatortype() == OperatorType::cluster ) {
		std::string str_cluster{ vertex.get_name() };
		outf << "\"" << vertex << std::to_string(graph_ctr) << "\"";
		outf << " [label=<" << vertex.get_name_html() << ">];" << '\n';
		//outf << ">];" << '\n';
	    }
	}
	outf << " }" << '\n';	

	for ( auto& edge : graph.get_edges() ) {
	    outf << '\t' << "\"" << edge.get_from() << std::to_string(graph_ctr) << "\"" 
		<< " -> " << "\"" << edge.get_to() << std::to_string(graph_ctr) << "\"" << ";" << '\n';
	}

	outf << '\t' << "}" << '\n';
	outf << "}" << '\n';

	++graph_ctr;
    }

    outf << "}" << '\n';
} 
