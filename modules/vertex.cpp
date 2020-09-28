#include "vertex.h"

//Vertex::Vertex(std::string name ) :
//    m_name{ name } {}
Vertex::Vertex(std::string name, OperatorType type ) :
    m_name{ name }, m_operatortype{ type } {}
Vertex::Vertex(std::string name, OperatorType type, int degree ) :
    m_name { name }, m_operatortype{ type }, m_degree{ degree } {}
Vertex::Vertex(std::string name, OperatorType type, int degree, std::array<std::array<int,2>,2> edgetable) :
    m_name{ name }, m_operatortype{ type }, m_degree{ degree }, m_connectiontable{ edgetable } {}


std::string Vertex::get_name() const {
    return m_name;
}


std::string Vertex::get_name_html() const {
    std::string str_out{ m_name.substr(0,1) };
    if ( m_name.size() > 1 )
	str_out.append( "<SUB>" + m_name.substr(1,1) + "</SUB>" );
    if ( m_name.size() > 3 )
	str_out.append( "<SUP>" + m_name.substr(3,1) + "</SUP>" );
    return str_out;
}

int Vertex::degree() const {
    return m_degree;
}


OperatorType Vertex::get_operatortype() const {
    return m_operatortype;
}


int Vertex::rank() const{
    return ( ( m_connectiontable[0][0] + m_connectiontable[0][1] ) - ( m_connectiontable[1][0] + m_connectiontable[1][1] ) );
}


int Vertex::get_connection( bool space, bool direction ) const {
    return m_connectiontable[space][direction];
}

bool Vertex::decrease_connection( bool space, bool direction ) {
    int& connection{ m_connectiontable[space][direction] };
    if ( connection - 1 < 0 )
	return false;
    --connection;
    return true;
}
bool Vertex::decrease_connection( bool space, bool direction, int value ) {
    int& connection{ m_connectiontable[space][direction] };
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
    m_connectiontable[space][direction] = value;
}


bool Vertex::connections() const {
    for ( int m = 0 ; m <= 1 ; ++m ) {
	for ( int n = 0 ; n<= 1 ; ++n ) {
	    if ( m_connectiontable[m][n] != 0 )
		return true;
	}
    }
    return false;
}

		    
void Vertex::clear_table() {
    m_connectiontable[0].fill( 0 );
    m_connectiontable[1].fill( 0 );
}


void Vertex::print_table() const {
    std::cout << "[" << m_connectiontable[0][0] << m_connectiontable[0][1] << m_connectiontable[1][0] << m_connectiontable[1][1] << "]" ;
}

Vertex Vertex::operator= ( const Vertex& vertex ) {
    Vertex cp_vertex( vertex.get_name(), vertex.get_operatortype(), vertex.degree(), vertex.m_connectiontable );
    
    return cp_vertex;
}

Vertex& Vertex::operator+= ( const Vertex& rhs ) {
    assert( this->m_operatortype == rhs.get_operatortype()
	    && "Vertex: Operator += can only by applied to vertices of the same type." );
   this->m_connectiontable[0][0] += rhs.get_connection( 0,0 ); 
   this->m_connectiontable[0][1] += rhs.get_connection( 0,1 ); 
   this->m_connectiontable[1][0] += rhs.get_connection( 1,0 ); 
   this->m_connectiontable[1][1] += rhs.get_connection( 1,1 ); 
   return *this;
}


std::ostream& operator<< (std::ostream &out, const Vertex &vertex){
//   out << std::setw(6) << vertex.m_name ;
//    out << std::setw(6) << "| " << vertex.m_connectiontable[0][0] << " "
//	<< vertex.m_connectiontable[0][1] << " |" << '\n';
//    out << std::setw(12) << "| " << vertex.m_connectiontable[1][0] << " "
//	<< vertex.m_connectiontable[1][1] << " |" << '\n';
    out << vertex.m_name ;
    return out;
}


bool operator< ( const Vertex &vertex1, const Vertex &vertex2 ) {
    if ( vertex1.get_operatortype() == vertex2.get_operatortype() ) {
	if ( vertex1.degree() == vertex2.degree() )
	    return ( vertex1.get_name() < vertex2.get_name() );
	else
	    return ( vertex1.degree() < vertex2.degree() );
    }
    else
	return ( vertex1.get_operatortype() < vertex2.get_operatortype() );
}

bool operator== ( const Vertex &vertex1, const Vertex &vertex2 ) {
    return ( vertex1.get_name() == vertex2.get_name() );
}
bool operator!= ( const Vertex &vertex1, const Vertex &vertex2 ) {
    return !( vertex1 == vertex2 );
}
