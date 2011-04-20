//  Copyright (c) 2010 Nuovation System Designs, LLC
//    Grant Erickson <gerickson@nuovations.com>
//
//  Reworked somewhat by Marshall Clow; August 2010
//  
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/ for latest version.
//
//  Description:
//    A set of four algorithms for inquiring about the properties of
//    sequences:
//      - Increasing
//      - Decreasing 
//      - Strictly Increasing
//      - Strictly Decreasing
//
//  along with the general purpose one:
//      - is_ordered ( sequence, predicate )

#ifndef BOOST_ALGORITHM_ORDERED_HPP
#define BOOST_ALGORITHM_ORDERED_HPP

#include <algorithm>
#include <functional>
#include <iterator>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm {

/// \fn is_ordered ( FI first, FI last, Pred p )
/// \return the point in the sequence [first, last) where the elements are unordered
///     (according to the comparison predicate 'p').
/// 
/// \param first The start of the sequence to be tested.
/// \param last  One past the end of the sequence
/// \param p     A binary predicate that returns true if two elements are ordered.
///
    template <typename FI, typename Pred>
    FI is_ordered ( FI first, FI last, Pred p )
    {
        if ( first == last ) return first;  // the empty sequence is ordered
        FI next = first;
        while ( ++next != last )
        {
            if ( !p ( *first, *next ))
                return first;
            first = next;
        }
        return last;    
    }

/// \fn is_ordered ( const R &range, Pred p )
/// \return the point in the sequence [first, last) where the elements are unordered
///     (according to the comparison predicate 'p').
/// 
/// \param range The range to be tested.
/// \param p     A binary predicate that returns true if two elements are ordered.
///
    template <typename R, typename Pred>
    typename boost::range_const_iterator<R>::type is_ordered ( const R &range, Pred p )
    {
        return (is_ordered) ( boost::begin ( range ), boost::end ( range ), p );
    }



/// \fn is_increasing ( FI first, FI last )
/// \return true if the entire sequence is increasing; i.e, each item is greater than or  
///     equal to the previous one.
/// 
/// \param first The start of the sequence to be tested.
/// \param last  One past the end of the sequence
///
/// \note This function will return true for sequences that contain items that compare
///     equal. If that is not what you intended, you should use is_strictly_increasing instead.
    template <typename FI>
    bool is_increasing ( FI first, FI last )
    {
        typedef typename std::iterator_traits<FI>::value_type value_type;
        return (is_ordered) (first, last, std::less_equal<value_type>()) == last;
    }


/// \fn is_increasing ( const R &range )
/// \return true if the entire sequence is increasing; i.e, each item is greater than or  
///     equal to the previous one.
/// 
/// \param range The range to be tested.
///
/// \note This function will return true for sequences that contain items that compare
///     equal. If that is not what you intended, you should use is_strictly_increasing instead.
    template <typename R>
    bool is_increasing ( const R &range )
    {
        return (is_increasing) ( boost::begin ( range ), boost::end ( range ));
    }



/// \fn is_decreasing ( FI first, FI last )
/// \return true if the entire sequence is decreasing; i.e, each item is less than 
///     or equal to the previous one.
/// 
/// \param first The start of the sequence to be tested.
/// \param last  One past the end of the sequence
///
/// \note This function will return true for sequences that contain items that compare
///     equal. If that is not what you intended, you should use is_strictly_decreasing instead.
    template <typename FI>
    bool is_decreasing ( FI first, FI last )
    {
        typedef typename std::iterator_traits<FI>::value_type value_type;
        return (is_ordered) (first, last, std::greater_equal<value_type>()) == last;
    }

/// \fn is_decreasing ( const R &range )
/// \return true if the entire sequence is decreasing; i.e, each item is less than 
///     or equal to the previous one.
/// 
/// \param range The range to be tested.
///
/// \note This function will return true for sequences that contain items that compare
///     equal. If that is not what you intended, you should use is_strictly_decreasing instead.
    template <typename R>
    bool is_decreasing ( const R &range )
    {
        return (is_decreasing) ( boost::begin ( range ), boost::end ( range ));
    }



/// \fn is_strictly_increasing ( FI first, FI last )
/// \return true if the entire sequence is strictly increasing; i.e, each item is greater
///     than the previous one
/// 
/// \param first The start of the sequence to be tested.
/// \param last  One past the end of the sequence
///
/// \note This function will return false for sequences that contain items that compare
///     equal. If that is not what you intended, you should use is_increasing instead.
    template <typename FI>
    bool is_strictly_increasing ( FI first, FI last )
    {
        typedef typename std::iterator_traits<FI>::value_type value_type;
        return (is_ordered) (first, last, std::less<value_type>()) == last;
    }

/// \fn is_strictly_increasing ( const R &range )
/// \return true if the entire sequence is strictly increasing; i.e, each item is greater
///     than the previous one
/// 
/// \param range The range to be tested.
///
/// \note This function will return false for sequences that contain items that compare
///     equal. If that is not what you intended, you should use is_increasing instead.
    template <typename R>
    bool is_strictly_increasing ( const R &range )
    {
        return (is_strictly_increasing) ( boost::begin ( range ), boost::end ( range ));
    }


/// \fn is_strictly_decreasing ( FI first, FI last )
/// \return true if the entire sequence is strictly decreasing; i.e, each item is less than
///     the previous one
/// 
/// \param first The start of the sequence to be tested.
/// \param last  One past the end of the sequence
///
/// \note This function will return false for sequences that contain items that compare
///     equal. If that is not what you intended, you should use is_decreasing instead.
    template <typename FI>
    bool is_strictly_decreasing ( FI first, FI last )
    {
        typedef typename std::iterator_traits<FI>::value_type value_type;
        return (is_ordered) (first, last, std::greater<value_type>()) == last;
    }

/// \fn is_strictly_decreasing ( const R &range )
/// \return true if the entire sequence is strictly decreasing; i.e, each item is less than
///     the previous one
/// 
/// \param range The range to be tested.
///
/// \note This function will return false for sequences that contain items that compare
///     equal. If that is not what you intended, you should use is_decreasing instead.
    template <typename R>
    bool is_strictly_decreasing ( const R &range )
    {
        return (is_strictly_decreasing) ( boost::begin ( range ), boost::end ( range ));
    }

}} // namespace boost

#endif  // BOOST_ALGORITHM_ORDERED_HPP
