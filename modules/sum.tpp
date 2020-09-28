template<class T>
void Sum<T>::push_front( const Product<T>& product ) {
    m_sum.push_front( product );
}

template<class T>
void Sum<T>::push_back( const Product<T>& product ) {
    m_sum.push_back( product );
}


template<class T>
void Sum<T>::mulitply_left( const Product<T>& lhs ) {
    for ( auto& products : m_sum ) {
	products.mulitply_left( lhs );
    }
}

template<class T>
void Sum<T>::mulitply_right( const Product<T>& rhs ) {
    for ( auto& products : m_sum ) {
	products.mulitply_right( rhs );
    }
}

template<class T>
void Sum<T>::append( const Sum<T>& sum ) {
    for ( auto it_sum = sum.cbegin() ; it_sum != sum.cend() ; ++it_sum )
	m_sum.push_back( *it_sum );
}

template<class T>
void Sum<T>::clear() {
    m_sum.clear();
}

template<class T>
typename std::deque<Product<T>>::iterator Sum<T>::begin() {
    return m_sum.begin();
}
template<class T>
typename std::deque<Product<T>>::iterator Sum<T>::end() {
    return m_sum.end();
}

template<class T>
typename std::deque<Product<T>>::const_iterator Sum<T>::cbegin() const {
    return m_sum.cbegin();
}
template<class T>
typename std::deque<Product<T>>::const_iterator Sum<T>::cend() const {
    return m_sum.cend();
}

template<class T>
Sum<T>& Sum<T>::operator= ( const Sum& sum ) {
    m_sum = sum.m_sum;
    return *this;
}

template<class T>
std::ostream& operator<< ( std::ostream& out, const Sum<T>& sum ) {
    for ( auto it_sum{ sum.m_sum.cbegin() } ; it_sum != sum.m_sum.cend() ; ++it_sum ) {
	out << *it_sum;
	if ( it_sum != ( sum.m_sum.cend() - 1 ) )
	    out << " + ";
    }
    return out;
}

