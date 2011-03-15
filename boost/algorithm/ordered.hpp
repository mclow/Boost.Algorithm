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
//	along with the general purpose one:
//		- is_ordered ( sequence, predicate )

#ifndef BOOST_ALGORITHM_ORDERED_HPP
#define BOOST_ALGORITHM_ORDERED_HPP

#include <algorithm>
#include <functional>
#include <iterator>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost { namespace algorithm {

    //  FI is an iterator: ForwardIterator or better
    //  Pred is a binary predicate taking two types
    //     that FI::value_type can be converted to.
    //  Returns true iff every adjacent pair in [first, last)
    //     satisfies the predicate Pred
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

    template <typename R, typename Pred>
    typename boost::range_const_iterator<R>::type is_ordered ( const R &range, Pred p )
    {
        return (is_ordered) ( boost::begin ( range ), boost::end ( range ), p );
    }



    template <typename FI>
    bool is_increasing ( FI first, FI last )
    {
        typedef typename std::iterator_traits<FI>::value_type value_type;
        return (is_ordered) (first, last, std::less_equal<value_type>()) == last;
    }

    template <typename R>
    bool is_increasing ( const R &range )
    {
        return (is_increasing) ( boost::begin ( range ), boost::end ( range ));
    }



    template <typename FI>
    bool is_decreasing ( FI first, FI last )
    {
        typedef typename std::iterator_traits<FI>::value_type value_type;
        return (is_ordered) (first, last, std::greater_equal<value_type>()) == last;
    }

    template <typename R>
    bool is_decreasing ( const R &range )
    {
        return (is_decreasing) ( boost::begin ( range ), boost::end ( range ));
    }



    template <typename FI>
    bool is_strictly_increasing ( FI first, FI last )
    {
        typedef typename std::iterator_traits<FI>::value_type value_type;
        return (is_ordered) (first, last, std::less<value_type>()) == last;
    }

    template <typename R>
    bool is_strictly_increasing ( const R &range )
    {
        return (is_strictly_increasing) ( boost::begin ( range ), boost::end ( range ));
    }


    template <typename FI>
    bool is_strictly_decreasing ( FI first, FI last )
    {
        typedef typename std::iterator_traits<FI>::value_type value_type;
        return (is_ordered) (first, last, std::greater<value_type>()) == last;
    }

    template <typename R>
    bool is_strictly_decreasing ( const R &range )
    {
        return (is_strictly_decreasing) ( boost::begin ( range ), boost::end ( range ));
    }

}} // namespace boost

#endif  // BOOST_ALGORITHM_ORDERED_HPP
