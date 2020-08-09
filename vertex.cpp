#include "vertex.h"

Vertex::Vertex(std::string name, OperatorType type, int degree, std::array<std::array<int,2>,2> edgetable) :
    m_name{ name }, m_operatortype{ type }, m_degree{ degree }, m_edgetable{ edgetable } {}

std::string Vertex::getname() const {
    return m_name;
}

int Vertex::rank() const{
    return ( ( m_edgetable[0][0] + m_edgetable[0][1] ) - ( m_edgetable[1][0] + m_edgetable[1][1] ) );
}

std::ostream& operator<< (std::ostream &out, const Vertex &vertex){
    out << std::setw(5) << vertex.m_name ;
    out << std::setw(10) << "| " << vertex.m_edgetable[0][0] << '\t' 
	<< vertex.m_edgetable[0][1] << " |" << '\n';
    out << std::setw(15) << "| " << vertex.m_edgetable[1][0] << '\t'
	<< vertex.m_edgetable[1][1] << " |" << '\n';

    return out;
}
