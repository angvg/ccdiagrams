
template<class T>
Product<T>::Product() {};
template<class T>
Product<T>::Product( double prefactor ) :
    m_prefactor{ prefactor } {};

template<class T>
T& Product<T>::front() {
    return m_multiplicands.front();
}
template<class T>
const T& Product<T>::front() const {
    return m_multiplicands.front();
}

template<class T>
typename std::deque<T>::size_type Product<T>::size() const {
    return m_multiplicands.size();
}

template<class T>
int Product<T>::count( const T& multiplicand ) const {
    return std::count( m_multiplicands.begin(), m_multiplicands.end(), multiplicand );
}

template<class T>
void Product<T>::set_prefactor( double prefactor ) {
    m_prefactor = prefactor;
}

template<class T>
void Product<T>::push_front( const T& multiplicand ) {
    m_multiplicands.push_front( multiplicand );
}

template<class T>
void Product<T>::push_back( const T& multiplicand ) {
    m_multiplicands.push_back( multiplicand );
}

template<class T>
void Product<T>::mulitply_left( const Product<T>& lhs ) {
    for ( auto it_lhs{ lhs.cbegin() } ; it_lhs != lhs.cend() ; ++it_lhs) {
	m_multiplicands.push_front( *it_lhs );
    }
}

template<class T>
void Product<T>::mulitply_right( const Product<T>& rhs ) {
    for ( auto it_rhs{ rhs.cbegin() } ; it_rhs != rhs.cend() ; ++it_rhs ) {
	m_multiplicands.push_back( *it_rhs );
    }
}


template<class T>
typename std::deque<T>::iterator Product<T>::begin() {
    return m_multiplicands.begin();
}
template<class T>
typename std::deque<T>::iterator Product<T>::end() {
    return m_multiplicands.end();
}

template<class T>
typename std::deque<T>::const_iterator Product<T>::cbegin() const {
    return m_multiplicands.cbegin();
}
template<class T>
typename std::deque<T>::const_iterator Product<T>::cend() const {
    return m_multiplicands.cend();
}

template<class T>
Product<T>& Product<T>::operator= ( const Product<T>& product ) {
    m_prefactor = product.m_prefactor;
    m_multiplicands = product.m_multiplicands;
    return *this;
}

template<class T>
std::ostream& operator<< ( std::ostream &out, const Product<T> &product ) {
    //out << product.m_prefactor;
    //out << "*";
    for ( auto& multiplicand : product.m_multiplicands ) {
	out << multiplicand;
    }
    return out;
}

