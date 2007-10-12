// this header defines template object matrix, and also necessary row
#ifndef IMS_MATRIX_H
#define IMS_MATRIX_H

#include <vector>

namespace ims {

////////////////////////////////////////////////////////////////////////////////
// row type necessary for matrix

template < typename T , typename A = std::allocator<T> >
class row {
public:
	//some typedefs (see Stroustrup pp 471)
	typedef T value_type;
	typedef typename A::size_type size_type;
	typedef std::vector<value_type, A> data_type;

	//iterators
	//typedef data_type::iterator iterator;
	typedef typename data_type::const_iterator const_iterator;
	
	//references
	typedef typename A::reference reference;
	typedef typename A::const_reference const_reference;

	//constructors
	row (size_type m, value_type v = value_type()) { data = std::vector<value_type> (m,v); } 

	//index access
	reference			operator[] (size_type j) { return data[j]; }
	const_reference		operator[] (size_type j) const { return data[j]; }

	//get iterators
	const_iterator	begin() const { return data.begin(); }
	const_iterator	end() const { return data.end(); }

	//misc
	size_type		size() const { return data.size(); }
private:
	//data field
	data_type data;
};


template < typename T, typename A = std::allocator<T> >
class matrix {
public:
	//some typedefs (see Stroustrup pp 471)
	typedef T value_type;
	typedef typename A::size_type size_type;
	typedef row<value_type> row_type;
	typedef std::vector<row_type> data_type;

	//row iteratorsessene
//	typedef typename data_type::iterator row_iterator;
	typedef typename data_type::const_iterator const_row_iterator;

	//row reference types
	typedef row_type& row_reference;
	typedef const row_reference const_row_reference;

	//constructors
	matrix (size_type n, size_type m, value_type v = value_type()) { data = data_type (n, row_type (m, v)); }

	//index access
	//TODO: not good to give a reference to row, cause so, user is abled to change it
	row_reference		operator[] (size_type i) { return data[i]; }
	const_row_reference	operator[] (size_type i) const { return data[i]; }

	//get row iterator (if it works, we only take const_iterator)
	const_row_iterator		rows_begin() const { return data.begin(); }
	const_row_iterator		rows_end() const { return data.end(); }
	
	//misc
	size_type		rows () const { return size_type (data.size()); }
	size_type		cols () const;
private:
	//data field
	data_type data;
};

////////////////////////////////////////////////////////////////////////////////
//functions
template < typename T, typename A >
typename matrix<T,A>::size_type matrix<T,A>::cols() const
{
	//TODO: somehow try to maintain original row_size for each row
	return size_type ((data.front()).size());
}

////////////////////////////////////////////////////////////////////////////////
//IO functions

//row
//I
/*
template <typename T, typename A>
std::istream& operator>> (std::istream& s, const row<T,A>& r)
{
	T x;
	char t;

	//read in until not more
	while (s >> x >> t)
		r.push_back(x);

	s.clear();
	//ATTENTION: for the moment only unix compatible
	if (t  = '\n') {
		//all ok, pushback last element
		r.push_back(x);
	}
	else {
		//nothing ok
		s.clear(ios_base::badbit);
	}

	return s;		
}
*/

//O
template <typename T, typename A>
std::ostream& operator<< (std::ostream& s, const row<T,A>& r)
{
	typename row<T,A>::const_iterator it = r.begin();

	//for now, we just write a tab as separator
	for ( ; it < r.end(); ++it) {
		s << *it;
		if (it != (r.end()-1)) s << '\t';
	}

	return s;		
}

//matrix
//I
/*
template <typename T, typename A>
std::istream& operator>> (std::istream& s, const matrix<T,A>& m)
{
	typename matrix<T,A>::const_row_iterator it = m.rows_begin();

	for ( ; it < m.rows_end(); ++it)
		s << *it << '\n';

	return s;
}
*/

//O
template <typename T, typename A>
std::ostream& operator<< (std::ostream& s, const matrix<T,A>& m)
{
	typename matrix<T,A>::const_row_iterator it = m.rows_begin();

	for ( ; it < m.rows_end(); ++it)
		s << *it << '\n';

	return s;
}

}; // namespace ims

#endif
