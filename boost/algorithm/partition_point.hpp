/* 
   Copyright (c) Marshall Clow 2011.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/// \file  partition_point.hpp
/// \brief Find the partition point in a sequence
/// \author Marshall Clow

#ifndef BOOST_ALGORITHM_PARTITION_POINT_HPP
#define BOOST_ALGORITHM_PARTITION_POINT_HPP

#include <algorithm>	// for std::partition_point, if available

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm {

#if __cplusplus >= 201103L
//  Use the C++11 versions of iota if it is available
using std::partition_point;      // Section 25.3.13
#else
/// \fn partition_point ( ForwardIterator first, ForwardIterator last, Predicate p )
/// \desc Given a partitioned range, returns the partition point, i.e, the first element 
///		that does not satisfy p
/// 
/// \param first    The start of the input sequence
/// \param last     One past the end of the input sequence
/// \param p        The predicicate to test the values with
/// \note           This function is part of the C++2011 standard library.
///  We will use the standard one if it is available, 
///  otherwise we have our own implementation.
template <typename ForwardIterator, typename Predicate>
ForwardIterator partition_point ( ForwardIterator first, ForwardIterator last, Predicate p )
{
    std::size_t dist = std::distance ( first, last );
    while ( first != last ) {
        std::size_t d2 = dist / 2;
        ForwardIterator ret_val = first;
        std::advance (ret_val, d2);
//      std::cout << *first << " " << *ret_val << " " << dist << std::endl;
        if (p (*ret_val)) {
            first = ++ret_val;
            dist -= d2 + 1;
            }
        else {
            last = ret_val;
            dist = d2;
            }
        }
    return first;
}
#endif

/// \fn is_partititioned ( const Range &r, Predicate p )
/// \desc Given a partitioned range, returns the partition point
/// 
/// \param range The input range
/// \param value    The initial value of the sequence to be generated
///
template <typename Range, typename Predicate>
typename boost::range_iterator<Range> partition_point ( Range &r, Predicate p )
{
    partition_point (boost::begin(r), boost::end(r), p);
}


}}

#endif  // BOOST_ALGORITHM_PARTITION_POINT_HPP