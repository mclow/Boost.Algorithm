/*
    Copyright Marshall Clow 2010-2011. Use, modification and
    distribution is subject to the Boost Software License, Version
    1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#ifndef BOOST_ALGORITHM_SEARCH_DETAIL_BM_TRAITS_HPP
#define BOOST_ALGORITHM_SEARCH_DETAIL_BM_TRAITS_HPP

#include <climits>      // for CHAR_BIT
#include <vector>
#include <iterator>     // for std::iterator_traits

#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/type_traits/remove_const.hpp>

#include <boost/array.hpp>
#include <boost/tr1/tr1/unordered_map>

#include <boost/algorithm/searching/detail/debugging.hpp>

namespace boost { namespace algorithm { namespace detail {

//
//  Default implementations of the skip tables for B-M and B-M-H
//
    template<typename key_type, bool /*useArray*/> class skip_table;

//  General case for data searching other than bytes; use a map
    template<typename key_type>
    class skip_table<key_type, false> {
    private:
        typedef std::tr1::unordered_map<key_type, int> skip_map;
        const int k_default_value;
        skip_map skip_;
        
    public:
        skip_table ( std::size_t patSize, int default_value ) 
            : k_default_value ( default_value ), skip_ ( patSize ) {}
        
        void insert ( key_type key, int val ) {
            skip_ [ key ] = val;    // Would skip_.insert (val) be better here?
            }

        int operator [] ( key_type key ) const {
            typename skip_map::const_iterator it = skip_.find ( key );
            return it == skip_.end () ? k_default_value : it->second;
            }
            
        void PrintSkipTable () const {
            std::cout << "BM(H) Skip Table <unordered_map>:" << std::endl;
            for ( typename skip_map::const_iterator it = skip_.begin (); it != skip_.end (); ++it )
                if ( it->second != k_default_value )
                    std::cout << "  " << it->first << ": " << it->second << std::endl;
            std::cout << std::endl;
            }
        };
        
    
//  Special case small numeric values; use an array
    template<typename key_type>
    class skip_table<key_type, true> {
    private:
        typedef typename boost::make_unsigned<key_type>::type unsigned_key_type;
        typedef boost::array<int, 1U << (CHAR_BIT * sizeof(key_type))> skip_map;
        skip_map skip_;
        const int k_default_value;
    public:
        skip_table ( std::size_t patSize, int default_value ) : k_default_value ( default_value ) {
            std::fill_n ( skip_.begin(), skip_.size(), default_value );
            }
        
        void insert ( key_type key, int val ) {
            skip_ [ static_cast<unsigned_key_type> ( key ) ] = val;
            }

        int operator [] ( key_type key ) const {
            return skip_ [ static_cast<unsigned_key_type> ( key ) ];
            }

        void PrintSkipTable () const {
            std::cout << "BM(H) Skip Table <boost:array>:" << std::endl;
            for ( typename skip_map::const_iterator it = skip_.begin (); it != skip_.end (); ++it )
                if ( *it != k_default_value )
                    std::cout << "  " << std::distance (skip_.begin (), it) << ": " << *it << std::endl;
            std::cout << std::endl;
            }
        };

    template<typename pattern_type>
    struct BM_traits {
        typedef pattern_type value_type;
        typedef pattern_type key_type;
        typedef boost::algorithm::detail::skip_table<key_type, 
                boost::is_integral<key_type>::value && (sizeof(key_type)==1)> skip_table_t;
        };

}}} // namespaces

#endif  //  BOOST_ALGORITHM_SEARCH_DETAIL_BM_TRAITS_HPP
