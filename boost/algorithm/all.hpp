/* 
   Copyright (c) Marshall Clow 2008-2012.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

 Revision history:
   05 May 2008 mtc First version - as part of BoostCon 2008
   07 May 2009 mtc Changed names to match n2666
   02 Jul 2010 mtc Changed namespace t
        on a suggestion by Sean Parent.
   24 May 2011 mtc Changed the names to match the ones in the C++11 standard,
        and to use those when available.
*/

/// \file
/// \brief Test ranges against predicates.
/// \author Marshall Clow

#ifndef BOOST_ALGORITHM_ALL_HPP
#define BOOST_ALGORITHM_ALL_HPP

#include <algorithm>            // for std::find and std::find_if

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>



namespace boost { namespace algorithm {

//  Use the C++11 versions of the routines if they are available
#if __cplusplus >= 201103L
using std::all_of;      // Section 25.2.1
using std::any_of;      // Section 25.2.2
using std::none_of;     // Section 25.2.3
#else

/// \fn all_of ( InputIterator first, InputIterator last, Predicate p )
/// \return true if all elements in [first, last) satisfy the predicate 'p'
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param p     A predicate for testing the elements of the sequence
///
  template<typename InputIterator, typename Predicate> 
  bool all_of ( InputIterator first, InputIterator last, Predicate p )
  {
    for ( ; first != last; ++first )
        if ( !p(*first)) 
          return false;
    return true; 
  } 


/// \fn any_of ( InputIterator first, InputIterator last, Predicate p )
/// \return true if any of the elements in [first, last) satisfy the predicate
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param p     A predicate for testing the elements of the sequence
///
  template<typename InputIterator, typename Predicate> 
  bool any_of ( InputIterator first, InputIterator last, Predicate p) 
  {
    for ( ; first != last; ++first )
        if ( p(*first)) 
          return true;
    return false; 
  } 


/// \fn none_of ( InputIterator first, InputIterator last, Predicate p )
/// \return true if none of the elements in [first, last) satisfy the predicate 'p'
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param p     A predicate for testing the elements of the sequence
///
  template<typename InputIterator, typename Predicate> 
  bool none_of ( InputIterator first, InputIterator last, Predicate p )
  {
    for ( ; first != last; ++first )
        if ( p(*first)) 
          return false;
    return true;
  } 


#endif


/// \fn all_of ( const R &range, Predicate p )
/// \return true if all elements in the range satisfy the predicate 'p'
/// 
/// \param range The input range
/// \param p     A predicate for testing the elements of the range
///
  template<typename R, typename Predicate> 
  bool all_of ( const R &range, Predicate p )
  {
    return (all_of) ( boost::begin ( range ), boost::end ( range ), p );
  } 

/// \fn none_of ( const R &range, Predicate p )
/// \return true if none of the elements in the range satisfy the predicate 'p'
/// 
/// \param range The input range
/// \param p     A predicate for testing the elements of the range
///
  template<typename R, typename Predicate> 
  bool none_of ( const R &range, Predicate p )
  {
    return (none_of) ( boost::begin ( range ), boost::end ( range ), p );
  } 

/// \fn any_of ( const R &range, Predicate p )
/// \return true if any elements in the range satisfy the predicate 'p'
/// 
/// \param range The input range
/// \param p     A predicate for testing the elements of the range
///
  template<typename R, typename Predicate> 
  bool any_of ( const R &range, Predicate p )
  {
    return (any_of) ( boost::begin ( range ), boost::end ( range ), p );
  } 


/// \fn one_of ( InputIterator first, InputIterator last, Predicate p )
/// \return true if the predicate 'p' is true for exactly one item in [first, last).
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param p     A predicate for testing the elements of the sequence
///
  template<typename InputIterator, typename Predicate> 
  bool  one_of ( InputIterator first, InputIterator last, Predicate p )
  {
    InputIterator i = std::find_if (first, last, p);
    if (i == last) return false;    // Didn't occur at all
    return (none_of) (++i, last, p);
  }

/// \fn one_of ( const R &range, Predicate p )
/// \return true if the predicate 'p' is true for exactly one item in the range.
/// 
/// \param range The input range
/// \param p     A predicate for testing the elements of the range
///
  template<typename R, typename Predicate> 
  bool one_of ( const R &range, Predicate p ) 
  {
    return (one_of) ( boost::begin ( range ), boost::end ( range ), p );
  } 



/// \fn all_of_equal ( InputIterator first, InputIterator last, const V &val )
/// \return true if all elements in [first, last) are equal to 'val'
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param val   A value to compare against
///
  template<typename InputIterator, typename V> 
  bool all_of_equal ( InputIterator first, InputIterator last, const V &val )
  {
    for ( ; first != last; ++first )
        if ( val != *first ) 
          return false;
    return true; 
  } 


/// \fn all_of_equal ( const R &range, const V &val )
/// \return true if all elements in the range are equal to 'val'
/// 
/// \param range The input range
/// \param val   A value to compare against
///
  template<typename R, typename V> 
  bool all_of_equal ( const R &range, const V &val ) 
  {
    return (all_of_equal) ( boost::begin ( range ), boost::end ( range ), val );
  } 

/// \fn any_of_equal ( InputIterator first, InputIterator last, const V &val )
/// \return true if any of the elements in [first, last) are equal to 'val'
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param val   A value to compare against
///
  template<typename InputIterator, typename V> 
  bool any_of_equal ( InputIterator first, InputIterator last, const V &val ) 
  {
    for ( ; first != last; ++first )
        if ( val == *first )
          return true;
    return false; 
  } 

/// \fn any_of_equal ( const R &range, const V &val )
/// \return true if any of the elements in the range are equal to 'val'
/// 
/// \param range The input range
/// \param val   A value to compare against
///
  template<typename R, typename V> 
  bool any_of_equal ( const R &range, const V &val ) 
  {
    return (any_of_equal) ( boost::begin ( range ), boost::end ( range ), val );
  } 

/// \fn none_of_equal ( InputIterator first, InputIterator last, const V &val )
/// \return true if none of the elements in [first, last) are equal to 'val'
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param val   A value to compare against
///
  template<typename InputIterator, typename V> 
  bool none_of_equal ( InputIterator first, InputIterator last, const V &val ) 
  {
    for ( ; first != last; ++first )
        if ( val == *first )
          return false;
    return true; 
  } 

/// \fn none_of_equal ( const R &range, const V &val )
/// \return true if none of the elements in the range are equal to 'val'
/// 
/// \param range The input range
/// \param val   A value to compare against
///
  template<typename R, typename V> 
  bool none_of_equal ( const R &range, const V & val ) 
  {
    return (none_of_equal) ( boost::begin ( range ), boost::end ( range ), val );
  } 


/// \fn one_of_equal ( InputIterator first, InputIterator last, const V &val )
/// \return true if the value 'val' exists only once in [first, last).
/// 
/// \param first The start of the input sequence
/// \param last  One past the end of the input sequence
/// \param val   A value to compare against
///
  template<typename InputIterator, typename V> 
  bool one_of_equal ( InputIterator first, InputIterator last, const V &val )
  {
    InputIterator i = std::find (first, last, val); // find first occurrence of 'val'
    if (i == last) return false;                    // Didn't occur at all
    return (none_of_equal) (++i, last, val);
  }

/// \fn one_of_equal ( const R &range, const V &val )
/// \return true if the value 'val' exists only once in the range.
/// 
/// \param range The input range
/// \param val   A value to compare against
///
  template<typename R, typename V> 
  bool one_of_equal ( const R &range, const V &val )
  {
    return (one_of_equal) ( boost::begin ( range ), boost::end ( range ), val );
  } 

}} // namespace boost and algorithm

#endif // BOOST_ALGORITHM_ALL_HPP
