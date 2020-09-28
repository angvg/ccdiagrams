#pragma once

#include <deque>
#include <iostream>
#include "product.h"

template <class T>
class Sum {

    private:

	std::deque<Product<T>> m_sum;

    public:

	void push_front( const Product<T>& product );
	void push_back( const Product<T>& product );
	
	void mulitply_left( const Product<T>& lhs );
	void mulitply_right( const Product<T>& rhs );

	void append( const Sum<T>& sum );

	void clear();

	typename std::deque<Product<T>>::iterator begin();
	typename std::deque<Product<T>>::iterator end();

	typename std::deque<Product<T>>::const_iterator cbegin() const;
	typename std::deque<Product<T>>::const_iterator cend() const;

	Sum<T>& operator= ( const Sum& sum );

	template <class U>
	friend std::ostream& operator<< ( std::ostream& out, const Sum<U>& sum );	

};

#include "sum.tpp"
