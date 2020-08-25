#include "vertex.h"

Vertex::Vertex(std::string name, OperatorType type, int degree, std::array<std::array<int,2>,2> edgetable) :
    m_name{ name }, m_operatortype{ type }, m_degree{ degree }, m_edgetable{ edgetable } {}


std::string Vertex::get_name() const {
    return m_name;
}


int Vertex::degree() const {
    return m_degree;
}


OperatorType Vertex::get_operatortype() const {
    return m_operatortype;
}


//std::pair<int,int> Vertex::get_creation() const {
//    return std::make_pair( m_edgetable[0][0], m_edgetable[0][1] );
//}
//
//std::pair<int,int> Vertex::get_annihilation() const {
//    return std::make_pair( m_edgetable[1][0], m_edgetable[1][1] );
//}


int Vertex::rank() const{
    return ( ( m_edgetable[0][0] + m_edgetable[0][1] ) - ( m_edgetable[1][0] + m_edgetable[1][1] ) );
}


int Vertex::get_connection( bool space, bool direction ) const {
    return m_edgetable[space][direction];
}

bool Vertex::decrease_connection( bool space, bool direction ) {
    int& connection{ m_edgetable[space][direction] };
    if ( connection - 1 < 0 )
	return false;
    --connection;
    return true;
}
bool Vertex::decrease_connection( bool space, bool direction, int value ) {
    int& connection{ m_edgetable[space][direction] };
    if ( connection - value < 0 )
	return false;
    connection -= value;
    return true;
}
bool Vertex::decrease_connection( const Vertex &vertex ) {
    for ( int m = 0 ; m <= 1 ; ++m ) {
	for ( int n = 0 ; n <= 1 ; ++n ) {
    if ( !(decrease_connection( m, n, vertex.get_connection( m,n ) ) ) )
	return false;
	}
    }
    return true;
}



void Vertex::set_connection( bool space, bool direction, int value ) { 
    m_edgetable[space][direction] = value;
}


void Vertex::clear_table() {
    m_edgetable[0].fill( 0 );
    m_edgetable[1].fill( 0 );
}


Vertex& Vertex::operator+= ( const Vertex& rhs ) {
    assert( this->m_operatortype == rhs.get_operatortype()
	    && "Vertex: Operator += can only by applied to vertices of the same type." );
   this->m_edgetable[0][0] += rhs.get_connection( 0,0 ); 
   this->m_edgetable[0][1] += rhs.get_connection( 0,1 ); 
   this->m_edgetable[1][0] += rhs.get_connection( 1,0 ); 
   this->m_edgetable[1][1] += rhs.get_connection( 1,1 ); 
   return *this;
}


std::ostream& operator<< (std::ostream &out, const Vertex &vertex){
//   out << std::setw(6) << vertex.m_name ;
//    out << std::setw(6) << "| " << vertex.m_edgetable[0][0] << " "
//	<< vertex.m_edgetable[0][1] << " |" << '\n';
//    out << std::setw(12) << "| " << vertex.m_edgetable[1][0] << " "
//	<< vertex.m_edgetable[1][1] << " |" << '\n';
    out << vertex.m_name ;
    return out;
}


bool operator< ( const Vertex &vertex1, const Vertex &vertex2 ) {
    if ( ( vertex1.get_operatortype() == OperatorType::external || vertex1.get_operatortype() == OperatorType::physical ) && vertex2.get_operatortype() == OperatorType::cluster )
	return true;
    else if ( vertex1.get_operatortype() == OperatorType::external && vertex2.get_operatortype() == OperatorType::physical )
	return true;
    else if ( vertex1.get_operatortype() == OperatorType::cluster && vertex2.get_operatortype() == OperatorType::cluster ) 
	return ( vertex1.degree() < vertex2.degree() );
	//return ( vertex1.get_name() < vertex2.get_name() );
    else
	return false;
}

bool operator== ( const Vertex &vertex1, const Vertex &vertex2 ) {
    return ( vertex1.get_name() == vertex2.get_name() );
}
bool operator!= ( const Vertex &vertex1, const Vertex &vertex2 ) {
    return !( vertex1 == vertex2 );
}
