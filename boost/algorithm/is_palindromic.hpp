/*
  Copyright (c) Alexander Zaitsev <zamazan4ik@gmail.com>, 2016

  Distributed under the Boost Software License, Version 1.0. (See
  accompanying file LICENSE_1_0.txt or copy at
  http://www.boost.org/LICENSE_1_0.txt)

  See http://www.boost.org/ for latest version.
*/

/// \file  is_palindromic.hpp
/// \brief Checks the input sequence on palindromic.
/// \author Alexander Zaitsev

#ifndef BOOST_ALGORITHM_is_palindromic_HPP
#define BOOST_ALGORITHM_is_palindromic_HPP

#include <iterator>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost {  namespace algorithm {

/// \fn is_palindromic ( BidirectionalIterator begin, BidirectionalIterator end, Predicate p )
/// \return true if the entire sequence is palindromic
///
/// \param begin    The start of the input sequence
/// \param end		One past the end of the input sequence
/// \param p        A predicate used to compare the values.
///
/// \note This function will return true for empty sequences and for palindromic
///     sequences. For other sequences function will return false.
///     Complexity: O(N).
template <typename BidirectionalIterator, typename Predicate>
bool is_palindromic(BidirectionalIterator begin, BidirectionalIterator end, Predicate p)
{
    if(begin == end)
    {
        return true;
    }

    --end;
    while(begin != end)
    {
        if(!p(*begin, *end))
        {
            return false;
        }
        ++begin;
        if(begin == end)
        {
            break;
        }
        --end;
    }
    return true;
}

/// \fn is_palindromic ( BidirectionalIterator begin, BidirectionalIterator end )
/// \return true if the entire sequence is palindromic
///
/// \param begin    The start of the input sequence
/// \param end		One past the end of the input sequence
///
/// \note This function will return true for empty sequences and for palindromic
///     sequences. For other sequences function will return false.
///     Complexity: O(N).
template <typename BidirectionalIterator>
bool is_palindromic(BidirectionalIterator begin, BidirectionalIterator end)
{
    return is_palindromic(begin, end,
                         std::equal_to<typename std::iterator_traits<BidirectionalIterator>::value_type> ());
}

/// \fn is_palindromic ( const R& range )
/// \return true if the entire sequence is palindromic
///
/// \param range The range to be tested.
///
/// \note This function will return true for empty sequences and for palindromic
///     sequences. For other sequences function will return false.
///     Complexity: O(N).
template <typename R>
bool is_palindromic(const R& range)
{
    return is_palindromic(boost::begin(range), boost::end(range));
}

/// \fn is_palindromic ( const R& range, Predicate p )
/// \return true if the entire sequence is palindromic
///
/// \param range The range to be tested.
/// \param p     A predicate used to compare the values.
///
/// \note This function will return true for empty sequences and for palindromic
///     sequences. For other sequences function will return false.
///     Complexity: O(N).
template <typename R, typename Predicate>
bool is_palindromic(const R& range, Predicate p)
{
    return is_palindromip(boost::begin(range), boost::end(range), p);
}

}}

#endif // BOOST_ALGORITHM_is_palindromic_HPP
