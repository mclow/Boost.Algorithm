/* 
   Copyright (c) Marshall Clow 2008.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

 Revision history:
   27 June 2009 mtc First version
   23 Oct  2010 mtc Added predicate version
   
*/

/// \file clamp.hpp
/// \brief Clamp algorithm
/// \author Marshall Clow
///
/// Suggested by olafvdspek in https://svn.boost.org/trac/boost/ticket/3215

#ifndef BOOST_ALGORITHM_CLAMP_HPP
#define BOOST_ALGORITHM_CLAMP_HPP

namespace boost { namespace algorithm {

/// \fn clamp ( V val, V lo, V hi, Pred p )
/// \brief Returns the value "val" brought into the range [ lo, hi ]
///          If the value is greater than "hi", return hi. If the value is
///          less than "lo", return lo. Otherwise, return the original value.
/// 
/// \param val   The value to be clamped
/// \param lo    The low point of the range to be clamped to
/// \param hi    The high point of the range to be clamped to
///	\param p	 A predicate to use to compare the values.
///					p ( a, b ) returns either 'a' or 'b'.
///
  template<typename V, typename Pred> 
  V clamp ( V val, V lo, V hi, Pred p )
  {
    return p ( val, lo ) ? lo : p ( hi, val ) ? hi : val;
  } 


/// \fn clamp ( V val, V lo, V hi )
/// \brief Returns the value "val" brought into the range [ lo, hi ]
///          If the value is greater than "hi", return hi. If the value is
///          less than "lo", return lo. Otherwise, return the original value.
/// 
/// \param val   The value to be clamped
/// \param lo    The low point of the range to be clamped to
/// \param hi    The high point of the range to be clamped to
///
  template<typename V> 
  V clamp ( V val, V lo, V hi )
  {
//  return val >= hi ? hi : val <= lo ? lo : val;
//  Alternately, 
//  return std::max ( std::min ( val, hi ), lo );
//  Rewritten to only use operator <
//  return val < lo ? lo : hi < val ? hi : val;
    return clamp ( val, lo, hi, std::less<V>());
  } 

}}

#endif // BOOST_ALGORITHM_CLAMP_HPP
