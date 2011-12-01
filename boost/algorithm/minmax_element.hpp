/* 
   Copyright (c) Marshall Clow 2011.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/// \file  minmax_element.hpp
/// \brief Find the minimum and maximum of a series of values.
/// \author Marshall Clow

#ifndef BOOST_ALGORITHM_MINMAX_ELEMENT_HPP
#define BOOST_ALGORITHM_MINMAX_ELEMENT_HPP

#include <algorithm>	// for std::less and minmax (if available)
#include <utility>		// for std::make_pair
#include <iterator>	
#include <iostream>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm {

#if __cplusplus >= 201103L
//  Use the C++11 versions of minmax_element if it is available
using std::minmax_element;      // Section 25.4.7
#else
template<class ForwardIterator, class Compare>
std::pair<ForwardIterator, ForwardIterator>
minmax_element ( ForwardIterator first, ForwardIterator last, Compare comp )
{
	ForwardIterator min_element = first;
	ForwardIterator max_element = first;
	
    if ( first != last ) {	// zero-element list: We're done
    	if ( ++first != last ) {	// One element list: We're also done
			if ( comp ( *first, *min_element ))
				min_element = first;
			else
				max_element = first;

    		while ( ++first != last ) {
    		//	std::cout << "minmax: " << *min_element << ", " << *max_element 
    		//		<< ", next = " << *first << std::endl;
    			ForwardIterator one = first;
    			if ( ++first == last ) { // one item left
	    	//		std::cout << "One element = " << *one << std::endl;
  					if      (  comp ( *one, *min_element )) min_element = one;
    				else if ( !comp ( *one, *max_element )) max_element = one;
    				break;
    				}
    			else {	// More than one item left, grab two
    				ForwardIterator two = first;
	    	//		std::cout << "Two elements = " << *one << ", " << *two << std::endl;
    				if ( !comp ( *two, *one )) {
    					if (  comp ( *one, *min_element )) min_element = one;
    					if ( !comp ( *two, *max_element )) max_element = two;
    					}
    				else {
    					if (  comp ( *two, *min_element )) min_element = two;
    					if ( !comp ( *one, *max_element )) max_element = one;
    					}
					}
				}
    		}
    	}
//	std::cout << "Returning: " << *min_element << ", " << *max_element << std::endl;
	return std::make_pair<ForwardIterator, ForwardIterator> (min_element, max_element);
}
    
template<class ForwardIterator>
std::pair<ForwardIterator, ForwardIterator>
minmax_element ( ForwardIterator first, ForwardIterator last )
{
	return minmax_element ( first, last, 
		std::less<typename std::iterator_traits<ForwardIterator>::value_type> ());
}
    
#endif
}}

#endif  // BOOST_ALGORITHM_MINMAX_ELEMENT_HPP