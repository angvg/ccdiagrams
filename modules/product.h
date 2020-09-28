#pragma once

#include <deque>
#include <algorithm>
#include <iostream>

template <class T>
class Product {

    private:

	double m_prefactor{1};
	std::deque<T> m_multiplicands;

    public:

	Product();
	Product( double prefactor );

	T& front();
	const T& front() const;

	typename std::deque<T>::size_type size() const;

	int count( const T& multiplicand ) const;

	void set_prefactor( double prefactor );

	void push_front( const T& multiplicand );
	void push_back( const T& multiplicand );

	void mulitply_left( const Product<T>& lhs );
	void mulitply_right( const Product<T>& rhs );

	typename std::deque<T>::iterator begin();
	typename std::deque<T>::iterator end();

	typename std::deque<T>::const_iterator cbegin() const;
	typename std::deque<T>::const_iterator cend() const;

	Product<T>& operator= ( const Product<T>& product );

	template <class U>
	friend std::ostream& operator<< ( std::ostream &out, const Product<U> &product );	
};

#include "product.tpp"
