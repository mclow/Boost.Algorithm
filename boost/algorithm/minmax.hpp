/* 
   Copyright (c) Marshall Clow 2011.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/// \file  minmax.hpp
/// \brief Find the minimum and maximum of a pair of values.
/// \author Marshall Clow

#ifndef BOOST_ALGORITHM_MINMAX_HPP
#define BOOST_ALGORITHM_MINMAX_HPP

#include <algorithm>	// for std::less and minmax (if available)
#include <utility>		// for std::make_pair
#include <iterator>	
#include <iostream>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm {

#if __cplusplus >= 201103L
//  Use the C++11 versions of minmax if it is available
using std::minmax;      		// Section 26.7.6
#else
//
template <typename T, typename Compare>
std::pair<const T&, const T&> 
minmax ( const T& a, const T&b, Compare comp )
{
	return comp (a, b) ? 
		std::make_pair<const T&, const T&> ( a, b ) :
		std::make_pair<const T&, const T&> ( b, a ) ;
}

/// \fn minmax ( const T& a, const T& b )
/// \desc Returns the min and max of a pair of values
/// 
/// \param a    	The first value
/// \param b		The second value
/// \note           This function is part of the C++2011 standard library.
///  We will use the standard one if it is available, 
///  otherwise we have our own implementation.
template <typename T>
std::pair<const T&, const T&> 
minmax ( const T& a, const T& b )
{
	return minmax ( a, b, std::less<T> ());
}   
#endif

}}

#endif  // BOOST_ALGORITHM_MINMAX_HPP