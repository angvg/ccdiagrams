#pragma once

#include <deque>
#include <string>
#include <array>
#include <utility>
#include <iostream>
#include <iomanip>
#include <cassert>


enum OperatorType{
    external,
    physical,
    cluster,
    undefined
};

class Vertex{

    private:

	const std::string m_name;	
	const OperatorType m_operatortype;
	// The degree of T1 = 1, T2 = 2, Fn = 1, Vn = 2 , ...
	const int m_degree{0};
	/*
	   This table stores the number of in- and outgoing connections 
	   of the operator and has the form m_connectiontable[QSPACE][IN/OUT].

	   Example T1:
			   |  in  out
	   ------------------------------
	   q-creation      |   1   1
	   q-annihilation  |   0   0
	*/ 
	std::array<std::array<int,2>,2> m_connectiontable;


    public:

	//Vertex(std::string name );
	Vertex(std::string name, OperatorType type );
	Vertex(std::string name, OperatorType type, int degree );
	Vertex(std::string name, OperatorType type, int degree, std::array<std::array<int,2>,2> edgetable);

	
	// space == false 	-> q-creation
	// space == true  	-> q-annihilation
	// direction == true 	-> in
	// direction == false	-> out
	int get_connection( bool space, bool direction ) const;
	// Decreases the values in the edgetable up to 0.
	// If a value would be decreased below 0, nothing is done and false is returned,
	// otherwise true is returned.
	bool decrease_connection( bool space, bool direction );
	bool decrease_connection( bool space, bool direction, int value );
	bool decrease_connection( const Vertex &vertex );
	// Sets a field in the edgetable to a specific value.
	void set_connection( bool space, bool direction, int value );

	// Returns false, if all connections in the table are 0.
	bool connections() const;

	// Sets all entries in connectiontable to 0.
	void clear_table();

	std::string get_name() const;
	std::string get_name_html() const;

	// The degree is determined by the number of creater/annihilator pairs.
	// E.g. T1: 1, T2: 2, T1^2: 2 , ...
	int degree() const;

	OperatorType get_operatortype() const;
	
	// Rank here is double the value of excitation level. E.g. rank(T1) = 2, rank (Vn) = 4.
	// This enables the handling of the rank with integers.
	int rank() const;
	
	void print_table() const;

	Vertex operator= ( const Vertex& vertex );

	Vertex& operator+= ( const Vertex& rhs );

	friend std::ostream& operator<< (std::ostream &out, const Vertex &vertex);
};

bool operator<  ( const Vertex &vertex1, const Vertex &vertex2 );
bool operator== ( const Vertex &vertex1, const Vertex &vertex2 );
bool operator!= ( const Vertex &vertex1, const Vertex &vertex2 );
