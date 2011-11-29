/* 
   Copyright (c) Marshall Clow 2008-2011.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/// \file  partition_copy.hpp
/// \brief Copy a subset of a sequence to a new sequence
/// \author Marshall Clow

#ifndef BOOST_ALGORITHM_PARTITION_COPY_HPP
#define BOOST_ALGORITHM_PARTITION_COPY_HPP

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm {

#if __cplusplus >= 201103L
//  Use the C++11 versions of partition_copy if it is available
using std::partition_copy;  // Section 25.3.13
#else
template <typename InputIterator, 
        typename OutputIterator1, typename OutputIterator2, typename Predicate>
std::pair<OutputIterator1, OutputIterator2>
partition_copy ( InputIterator first, InputIterator last,
        OutputIterator1 out_true, OutputIterator2 out_false, Predicate p )
{
    for ( ; first != last; ++first )
        if ( p (*first))
            *out_true++ = *first;
        else
            *out_false++ = *first;
    return std::make_pair<OutputIterator1, OutputIterator2> ( out_true, out_false );
}
#endif

template <typename Range, typename OutputIterator1, typename OutputIterator2, 
            typename Predicate>
std::pair<OutputIterator1, OutputIterator2>
partition_copy ( const Range &r, OutputIterator1 out_true, OutputIterator2 out_false, 
                                Predicate p )
{
    return partition_copy (boost::begin(r), boost::end(r), out_true, out_false, p );
}

}} // namespace boost and algorithm

#endif  // BOOST_ALGORITHM_PARTITION_COPY_HPP
