/* 
   Copyright (c) Marshall Clow 2008-2011.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

/// \file  move.hpp
/// \brief Move a sequence of items to a new location
/// \author Marshall Clow

#ifndef BOOST_ALGORITHM_MOVE_HPP
#define BOOST_ALGORITHM_MOVE_HPP

#include <algorithm>	// for std::move, if available
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

#if __cplusplus >= 201103L
namespace boost {
	namespace algorithm {
//  Use the C++11 versions of move and move_backward if they are available
		using std::move;         	// Section 25.3.2
		using std::move_backward;   // Section 25.3.2
	}
}
#else
#include <boost/move/move.hpp>

namespace boost {
	namespace algorithm {
//  Use the boost versions of move and move_backward
		using boost::move;
		using boost::move_backward;
	}
}
#endif


namespace boost { namespace algorithm {

/// \fn move ( const Range &r, OutputIterator result )
/// \brief Move all the elements from the input range to the output 
///		range, copying front to back.
/// \return The updated output iterator
/// 
/// \param r        The input range
/// \param result   An output iterator to write the results into
///
template<typename Range, typename OutputIterator>
OutputIterator move ( const Range &r, OutputIterator result )
{
    return boost::algorithm::move (boost::begin (r), boost::end(r), result);
}

/// \fn move_backward ( const Range &r, OutputIterator result )
/// \brief Move all the elements from the input range to the output 
///		range, copying back to front.
/// \return The updated output iterator
/// 
/// \param r        The input range
/// \param result   An output iterator to write the results into
///
template<typename Range, typename OutputIterator>
OutputIterator move_backward ( const Range &r, OutputIterator result )
{
    return boost::algorithm::move_backward (boost::begin (r), boost::end(r), result);
}

}} // namespace boost and algorithm

#endif  // BOOST_ALGORITHM_COPY_IF_HPP
