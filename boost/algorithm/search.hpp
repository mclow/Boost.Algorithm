/*
    Search algorithms
        * Boyer-Moore
        * Boyer-Moore-Horspool
        * Knuth-Morris-Pratt

    Copyright Marshall Clow 2010-2011. Use, modification and
    distribution is subject to the Boost Software License, Version
    1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
    
    05 Nov 2010 - Initial public version
    01 Mar 2011	- Refactored to use skip table objects
*/

#ifndef BOOST_ALGORITHM_SEARCH_HPP
#define BOOST_ALGORITHM_SEARCH_HPP

// #define  B_ALGO_DEBUG
#define USE_SKIP_TABLE_MAP      false

#ifdef  B_ALGO_DEBUG
#include <iostream>
#include <string>
#endif

#include <vector>
#include <functional>   // for std::equal_to
#include <tr1/unordered_map>

#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/array.hpp>
#include <boost/static_assert.hpp>

namespace boost { namespace algorithm {

namespace detail {

#ifdef  B_ALGO_DEBUG
//  Debugging support
    template <typename Iter>
    void PrintTable ( Iter first, Iter last ) {
        std::cout << std::distance ( first, last ) << ": { ";
        for ( Iter iter = first; iter != last; ++iter )
            std::cout << *iter << " ";
        std::cout << "}" << std::endl;
        }
    
    template <typename Iter>
    std::string make_str ( Iter first, std::size_t len ) {
        std::string retVal ( len + 2, '\'' );
        std::copy ( first, first+len, retVal.begin () + 1);
        return retVal;
        }
#endif

    namespace bm {
        template<typename Iter, bool useMap> class skip_table;
    
    //  General case for data searching other than bytes; use a map
        template<typename Iter>
        class skip_table<Iter, true> {
        private:
            typedef typename Iter::value_type value_type;
            typedef std::tr1::unordered_map<value_type, int> skip_map;
            skip_map skip_;
            
        public:
            skip_table ( Iter first, Iter last ) :skip_ ( std::distance ( first, last )) {
                std::size_t i = 0;
                for ( Iter iter = first; iter != last; ++iter, ++i )
                    skip_ [ *iter ] = i;
                }
            
            int operator [] ( typename Iter::value_type val ) const {
                typename skip_map::const_iterator it = skip_.find ( val );
                return it == skip_.end () ? -1 : it->second;
                }
                
    #ifdef  B_ALGO_DEBUG
            void PrintSkipTable () const {
                std::cout << "BM Skip Table <unordered_map>:" << std::endl;
                for ( typename skip_map::const_iterator it = skip_.begin (); it != skip_.end (); ++it )
                    std::cout << "  " << it->first << ": " << it->second << std::endl;
                std::cout << std::endl;
                }
    #endif
            };
            
        
    //  Special case small numeric values; use an array
        template<typename Iter>
        class skip_table<Iter, false> {
        private:
            typedef typename Iter::value_type value_type;
            typedef typename boost::make_unsigned<value_type>::type unsigned_value_type;
            typedef boost::array<int, 1U << (CHAR_BIT * sizeof(value_type))> skip_map;
            skip_map skip_;
        public:
            skip_table ( Iter first, Iter last ) {
                std::fill_n ( skip_.begin(), skip_.size(), -1 );
                std::size_t i = 0;
                for ( Iter iter = first; iter != last; ++iter, ++i )
                    skip_ [ static_cast<unsigned_value_type> (*iter) ] = i;
                }
            
            int operator [] ( typename Iter::value_type val ) const {
                return skip_ [ static_cast<unsigned_value_type> ( val ) ];
                }
    
    #ifdef  B_ALGO_DEBUG
            void PrintSkipTable () const {
                std::cout << "BM Skip Table <boost:array>:" << std::endl;
                typename skip_map::const_iterator it = skip_.begin ();
                for ( typename skip_map::const_iterator it = skip_.begin (); it != skip_.end (); ++it )
                    std::cout << "  " << std::distance (it, skip_.begin ()) << ": " << *it << std::endl;
                std::cout << std::endl;
                }
    #endif
            };

        template<typename Iter>
        void compute_bm_prefix ( Iter pat_first, Iter pat_last, std::size_t *prefix /* [count] */ ) {
            const std::size_t count = std::distance ( pat_first, pat_last );
        
            prefix[0] = 0;
            std::size_t k = 0;
            for ( std::size_t i = 1; i < count; ++i ) {
                while ( k > 0 && pat_first[k] != pat_first[i] )
                    k = prefix [ k - 1 ];
                if ( pat_first[k] == pat_first[i] )
                    k++;
                prefix [ i ] = k;
                }
            }

        template<typename Iter>
        void create_suffix_table ( Iter pat_first, Iter pat_last, std::size_t *result /* count+1 */ ) {
            const std::size_t count = (std::size_t) std::distance ( pat_first, pat_last );
            std::vector<typename Iter::value_type> reversed(count);     
            (void) std::copy_backward ( pat_first, pat_last, reversed.end ());
            
            std::vector<std::size_t> prefix (count);
            compute_bm_prefix ( pat_first, pat_last, &*prefix.begin ());
    
            std::vector<std::size_t> prefix_reversed (count);
            compute_bm_prefix ( reversed.begin (), reversed.end (), &*prefix_reversed.begin ());
            
            std::size_t i;
            for ( i = 0; i <= count; i++ )
                result[i] = count - prefix [count-1];
     
            for (i = 0; i < count; i++) {
                const std::size_t j = count - prefix_reversed[i];
                const std::size_t k = i -     prefix_reversed[i] + 1;
     
                if (result[j] > k)
                    result[j] = k;
                }
            }
        
    }   // namespace bm
            

    namespace bmh {
        template<typename Iter, bool useMap> class skip_table;

    //  General case for data searching other than bytes; use a map
        template<typename Iter>
        class skip_table<Iter, true> {
        private:
            typedef typename Iter::value_type value_type;
            typedef std::tr1::unordered_map<value_type, int> skip_map;
            const std::size_t k_pattern_length;
            skip_map skip_;
            
        public:
            skip_table ( Iter first, Iter last )
                : k_pattern_length (std::distance( first, last )),
                    skip_ ( std::distance ( first, last )) {
                std::size_t i = 0;
                for ( Iter iter = first; iter != last-1; ++iter, ++i )
                    skip_ [ *iter ] = ( k_pattern_length - 1 ) - i;
                }
            
            int operator [] ( typename Iter::value_type val ) const {
                typename skip_map::const_iterator it = skip_.find ( val );
                return it == skip_.end () ? k_pattern_length : it->second;
                }
                
    #ifdef  B_ALGO_DEBUG
            void PrintSkipTable () const {
                std::cout << "BMH Skip Table <unordered_map>:" << std::endl;
                for ( typename skip_map::const_iterator it = skip_.begin (); it != skip_.end (); ++it )
                    std::cout << "  " << it->first << ": " << it->second << std::endl;
                std::cout << std::endl;
                }
    #endif
            };
            
        
    //  Special case small numeric values; use an array
        template<typename Iter>
        class skip_table<Iter, false> {
        private:
            typedef typename Iter::value_type value_type;
            typedef typename boost::make_unsigned<value_type>::type unsigned_value_type;
            typedef std::vector <value_type> skip_map;
            const std::size_t k_pattern_length;
            skip_map skip_;
    
        public:
            skip_table ( Iter first, Iter last ) 
                : k_pattern_length (std::distance( first, last )),
                      skip_ ( 1U << (CHAR_BIT * sizeof(value_type)), k_pattern_length ) {
                std::size_t i = 0;
                for ( Iter iter = first; iter != last-1; ++iter, ++i )
                    skip_ [ static_cast<unsigned_value_type> ( *iter ) ] = ( k_pattern_length - 1 ) - i;
                }
            
            int operator [] ( typename Iter::value_type val ) const {
                return skip_ [ static_cast<unsigned_value_type> ( val ) ];
                }
    
    #ifdef  B_ALGO_DEBUG
            void PrintSkipTable () const {
                std::cout << "BMH Skip Table <vector>:" << std::endl;
                typename skip_map::const_iterator it = skip_.begin ();
                for ( typename skip_map::const_iterator it = skip_.begin (); it != skip_.end (); ++it )
                    std::cout << "  " << std::distance (it, skip_.begin ()) << ": " << *it << std::endl;
                std::cout << std::endl;
                }
    #endif
            };
    }   // namespace bmh
        
    template<typename Iter>
    void create_kmp_table ( Iter first, Iter last, int *skip /* count+1 */ ) {
        const /*std::size_t*/ int count = std::distance ( first, last );

        int j;
        skip [ 0 ] = -1;
        for ( int i = 1; i < count; ++i ) {
            j = skip [ i - 1 ];
            while ( j >= 0 ) {
                if ( first [ j ] == first [ i - 1 ] )
                    break;
                j = skip [ j ];
                }
            skip [ i ] = j + 1;
            }
        }

    }

/*
    A templated version of the boyer-moore searching algorithm.
    
References:
    http://www.cs.utexas.edu/users/moore/best-ideas/string-searching/
    http://www.cs.utexas.edu/~moore/publications/fstrpos.pdf
    
Explanations:
    http://en.wikipedia.org/wiki/Boyer–Moore_string_search_algorithm
    http://www.movsd.com/bm.htm
    http://www.cs.ucdavis.edu/~gusfield/cs224f09/bnotes.pdf


Requirements:
        * Random access iterators
        * value types are convertable to an integral type
        * Small size for the pattern type (usually a byte).
    Internally, this algorithm builds a table of all possible values. For 8-bit
    quantities, that means a 256 entry table. For 16-bit ones, that's a 64K entry
    table, and so on. 
        
        * The two iterator types (I1 and I2) must "point to" the same underlying type.
*/

template <typename I1, typename I2, typename Pred>
I1 boyer_moore_search ( I1 corpus_first, I1 corpus_last,
                    I2 pat_first, I2 pat_last, Pred p ) {
    BOOST_STATIC_ASSERT (( boost::is_same<typename I1::value_type, typename I2::value_type>::value ));
    if ( pat_first == pat_last )       return corpus_first; // empty pattern matches at start
    if ( corpus_first == corpus_last ) return corpus_last;  // if nothing to search, we didn't find it!
    
    const std::size_t k_pattern_length = std::distance ( pat_first, pat_last );
    if ( static_cast<std::size_t> ( std::distance ( corpus_first, corpus_last )) < k_pattern_length ) 
        return corpus_last;     // If the pattern is larger than the corpus....

    detail::bm::skip_table<I2, USE_SKIP_TABLE_MAP> skip ( pat_first, pat_last );
#ifdef B_ALGO_DEBUG
    skip.PrintSkipTable ();
#endif
    
/*  Prepare the suffix table */
    std::vector <std::size_t> suffix ( k_pattern_length + 1 );
    detail::bm::create_suffix_table ( pat_first, pat_last, &*suffix.begin ());
#ifdef B_ALGO_DEBUG
    detail::PrintTable ( suffix.begin (), suffix.end ());
#endif

/*  ---- Do the matching ---- */
    I1 curPos = corpus_first;
    const I1 lastPos = corpus_last - k_pattern_length;
    std::size_t j;
    int k;
    int m;
    while ( curPos <= lastPos ) {
/*  while ( std::distance ( curPos, corpus_last ) >= k_pattern_length ) { */
    //  Do we match right where we are?
        j = k_pattern_length;
        while ( p ( pat_first [j-1], curPos [j-1] )) {
            j--;
        //  We matched - we're done!
            if ( j == 0 )
                return curPos;
            }
        
    //  Since we didn't match, figure out how far to skip forward
        k = skip [ curPos [ j - 1 ]];
        m = j - k - 1;
        if ( k < (int) j && m > (int) suffix [ j ] )
            curPos += m;
        else
            curPos += suffix [ j ];
        }

    return corpus_last;     // We didn't find anything
    }

//  Default predicate version
template <typename I1, typename I2>
I1 boyer_moore_search ( I1 corpus_first, I1 corpus_last, I2 pat_first, I2 pat_last ) {
    return boost::algorithm::boyer_moore_search 
        ( corpus_first, corpus_last, pat_first, pat_last, 
            std::equal_to<typename I1::value_type> ());
    }

/*
    A templated version of the boyer-moore-horspool searching algorithm.
    
    Requirements:
        * Random-access iterators
        * value types convertible to a std::size_t
        * The two iterator types (I1 and I2) must "point to" the same underlying type.

http://www-igm.univ-mlv.fr/%7Elecroq/string/node18.html

*/
template <typename I1, typename I2, typename Pred>
I1 boyer_moore_horspool_search ( I1 corpus_first, I1 corpus_last,
                    I2 pat_first, I2 pat_last, Pred p ) {
    BOOST_STATIC_ASSERT (( boost::is_same<typename I1::value_type, typename I2::value_type>::value ));
    if ( pat_first == pat_last )       return corpus_first; // empty pattern matches at start
    if ( corpus_first == corpus_last ) return corpus_last;  // if nothing to search, we didn't find it!
    
    const std::size_t k_pattern_length = std::distance ( pat_first, pat_last );
    if ( static_cast <std::size_t> ( std::distance ( corpus_first, corpus_last )) < k_pattern_length ) 
        return corpus_last;     // If the pattern is larger than the corpus....

    detail::bmh::skip_table<I2, USE_SKIP_TABLE_MAP> skip ( pat_first, pat_last );
#ifdef B_ALGO_DEBUG
    skip.PrintSkipTable ();
#endif

/*  ---- Do the matching ---- */
    I1 curPos = corpus_first;
    const I1 lastPos = corpus_last - k_pattern_length;
    while ( curPos <= lastPos ) {
/*  while ( std::distance ( curPos, corpus_last ) >= k_pattern_length ) { */

    //  Do we match right where we are?
        std::size_t j = k_pattern_length - 1;
        while ( p ( pat_first [j], curPos [j] )) {
        //  We matched - we're done!
            if ( j == 0 )
                return curPos;
            j--;
            }

        curPos += skip [ curPos [ k_pattern_length - 1 ]];
        }
    
    return corpus_last;
    }

//  Default predicate version
template <typename I1, typename I2>
I1 boyer_moore_horspool_search ( I1 corpus_first, I1 corpus_last, I2 pat_first, I2 pat_last ) {
    return boost::algorithm::boyer_moore_horspool_search 
        ( corpus_first, corpus_last, pat_first, pat_last, 
            std::equal_to<typename I1::value_type> ());
    }


/*
    A templated version of the Knuth-Pratt-Morris searching algorithm.
    
    Requirements:
        * Random-access iterators
        * The two iterator types (I1 and I2) must "point to" the same underlying type.

    http://en.wikipedia.org/wiki/Knuth–Morris–Pratt_algorithm
    http://www.inf.fh-flensburg.de/lang/algorithmen/pattern/kmpen.htm
*/

template <typename I1, typename I2, typename Pred>
I1 knuth_morris_pratt_search ( I1 corpus_first, I1 corpus_last,
                    I2 pat_first, I2 pat_last, Pred p ) {
    BOOST_STATIC_ASSERT (( boost::is_same<typename I1::value_type, typename I2::value_type>::value ));
    if ( pat_first == pat_last ) return corpus_first;   // empty pattern matches at start
    if ( corpus_first == corpus_last ) return corpus_last;  // if nothing to search, we didn't find it!

//  Build the skip table
    const std::size_t k_pattern_length = (std::size_t) std::distance ( pat_first, pat_last );
    const std::size_t k_corpus_length  = (std::size_t) std::distance ( corpus_first, corpus_last );
    if ( k_corpus_length < k_pattern_length ) 
        return corpus_last;     // If the pattern is larger than the corpus....

    std::vector <int> skip ( k_pattern_length + 1 );
    detail::create_kmp_table ( pat_first, pat_last, &*skip.begin ());
    
/*  ---- Do the matching ---- */
    std::size_t idx = 0;
    /* std::size_t*/ int j = 0;
    I1 curPos = corpus_first;
    const I1 lastPos = corpus_last - k_pattern_length;
//  while ( curPos <= lastPos ) {
    while ( idx < k_corpus_length ) {
        while ( j >= 0 && !p ( pat_first [j], corpus_first[idx] ))
            j = skip [ j ];
        idx ++;
        if ( ++j == (int) k_pattern_length )
            return corpus_first + idx - j;
        }
    
//  We didn't find anything
    return corpus_last;
    }

//  Default predicate version
template <typename I1, typename I2>
I1 knuth_morris_pratt_search ( I1 corpus_first, I1 corpus_last, I2 pat_first, I2 pat_last ) {
    return boost::algorithm::knuth_morris_pratt_search ( 
        corpus_first, corpus_last, pat_first, pat_last, 
            std::equal_to<typename I1::value_type> ());
    }

}}

#endif
