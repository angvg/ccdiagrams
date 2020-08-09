#pragma once

#include <string>
#include <array>
#include <iomanip>


enum class OperatorType{
    external,
    physical,
    cluster
};

class Vertex{

    private:

	const std::string m_name;	
	const OperatorType m_operatortype;
	// The degree of T1 = 1, T2 = 2, Fn = 1, Vn = 2 , ...
	const int m_degree;
	/*
	   This table stores the number of in- and outgoing connections 
	   of the operator and has the form m_edgetable[QSPACE][IN/OUT].

	   Example T1:
			   |  in  out
	   ------------------------------
	   q-creation      |   1   1
	   q-annihilation  |   0   0
	*/ 
	std::array<std::array<int,2>,2> m_edgetable;

    public:

	Vertex(std::string name, OperatorType type, int degree, std::array<std::array<int,2>,2> edgetable);

	std::string getname() const;

	int rank() const;

	friend std::ostream& operator<< (std::ostream &out, const Vertex &vertex);
};

