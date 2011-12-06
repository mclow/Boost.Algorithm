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

#include <algorithm>    // for std::less and minmax (if available)
#include <utility>      // for std::make_pair
#include <iterator> 
#include <iostream>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace algorithm {

#if __cplusplus >= 201103L
//  Use the C++11 versions of minmax_element if it is available
using std::minmax_element;      // Section 25.4.7
#else
/// \fn minmax_element ( ForwardIterator first, ForwardIterator last, Compare comp )
/// \brief Returns a pair of iterators denoting the minimum and maximum values of a sequence.
/// 
/// \param first    The start of the input sequence
/// \param last     One past the end of the input sequence
/// \param comp     A predicate used to compare the values.
/// \note           This function is part of the C++2011 standard library.
///  We will use the standard one if it is available, 
///  otherwise we have our own implementation.
template<class ForwardIterator, class Compare>
std::pair<ForwardIterator, ForwardIterator>
minmax_element ( ForwardIterator first, ForwardIterator last, Compare comp )
{
    ForwardIterator min_element = first;
    ForwardIterator max_element = first;
    
    if ( first != last ) {  // zero-element list: We're done
        if ( ++first != last ) {    // One element list: We're also done
            if ( comp ( *first, *min_element ))
                min_element = first;
            else
                max_element = first;

            while ( ++first != last ) {
            //  std::cout << "minmax: " << *min_element << ", " << *max_element 
            //      << ", next = " << *first << std::endl;
                ForwardIterator one = first;
                if ( ++first == last ) { // one item left
            //      std::cout << "One element = " << *one << std::endl;
                    if      (  comp ( *one, *min_element )) min_element = one;
                    else if ( !comp ( *one, *max_element )) max_element = one;
                    break;
                    }
                else {  // More than one item left, grab two
                    ForwardIterator two = first;
            //      std::cout << "Two elements = " << *one << ", " << *two << std::endl;
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
//  std::cout << "Returning: " << *min_element << ", " << *max_element << std::endl;
    return std::make_pair<ForwardIterator, ForwardIterator> (min_element, max_element);
}
    
/// \fn minmax_element ( ForwardIterator first, ForwardIterator last )
/// \brief Returns a pair of iterators denoting the minimum and maximum values of a sequence.
/// 
/// \param first    The start of the input sequence
/// \param last     One past the end of the input sequence
/// \note           This function is part of the C++2011 standard library.
///  We will use the standard one if it is available, 
///  otherwise we have our own implementation.
template<class ForwardIterator>
std::pair<ForwardIterator, ForwardIterator>
minmax_element ( ForwardIterator first, ForwardIterator last )
{
    return minmax_element ( first, last, 
        std::less<typename std::iterator_traits<ForwardIterator>::value_type> ());
}
   
#endif

/*
    mtc - 12-05-2011
         The range-based versions of minmax_element take their range parameter
    by reference rather than by const reference. This is to make it possible to 
    return non-const iterators; to match the behavior of the iterator-based versions.
    Otherwise code like this would fail to compile:
    
    std::vector<int> v;
    std::pair<std::vector<int>::iterator, std::vector<int>::iterator> p1;
    
    p1 = minmax_element ( v.begin (), v.end ());    // works
    p1 = minmax_element ( v );                      // fails
*/

namespace detail {
/// \cond DOXYGEN_HIDE
    template <typename Range>
    struct range_pair {
        typedef std::pair<typename boost::range_iterator<Range>::type, 
                          typename boost::range_iterator<Range>::type> type;
        };
/// \endcond
}

/// \fn minmax_element ( Range &r )
/// \brief Returns a pair of iterators denoting the minimum and maximum values of a sequence.
/// 
/// \param r        The range to be searched
template<class Range>
typename detail::range_pair<Range>::type
minmax_element ( Range &r )
{
    return minmax_element (boost::begin (r), boost::end (r));
}


/// \fn minmax_element ( Range &r, Compare comp )
/// \brief Returns a pair of iterators denoting the minimum and maximum values of a sequence.
/// 
/// \param r        The range to be searched
/// \param comp     A predicate used to compare the values.
//
//  Disable this template when the first two parameters are the same type
//  That way the non-range version will be chosen.
template<class Range, class Compare>
typename boost::lazy_disable_if_c<
        boost::is_same<Range, Compare>::value, typename detail::range_pair<Range> >
    ::type
minmax_element ( Range &r, Compare comp )
{
    return minmax_element (boost::begin (r), boost::end (r), comp );
}

}}

#endif  // BOOST_ALGORITHM_MINMAX_ELEMENT_HPP