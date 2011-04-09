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
    01 Mar 2011 - Refactored to use skip table objects
    05 Mar 2011 - Created search objects to enable table reuse
    15 Mar 2011 - Added traits class to select skip table params
*/

#ifndef BOOST_ALGORITHM_SEARCH_HPP
#define BOOST_ALGORITHM_SEARCH_HPP

// #define  B_ALGO_DEBUG

#ifdef  B_ALGO_DEBUG
#include <iostream>
#include <string>
#endif

#include <cassert>
#include <vector>
#include <functional>   // for std::equal_to
#include <tr1/unordered_map>

#include <boost/type_traits/make_unsigned.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_integral.hpp>
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

//
//	Default implementations of the skip tables for B-M and B-M-H
//
	template<typename Iter, bool /*useArray*/> class skip_table;

//  General case for data searching other than bytes; use a map
	template<typename Iter>
	class skip_table<Iter, false> {
	private:
		typedef typename Iter::value_type value_type;
		typedef std::tr1::unordered_map<value_type, int> skip_map;
		const int k_default_value;
		skip_map skip_;
		
	public:
		skip_table ( std::size_t patSize, int default_value ) 
			: k_default_value ( default_value ), skip_ ( patSize ) {}
		
		void insert ( typename Iter::value_type key, int val ) {
			skip_ [ key ] = val;    // Would skip_.insert (<blah>) be better here?
			}

		int operator [] ( typename Iter::value_type val ) const {
			typename skip_map::const_iterator it = skip_.find ( val );
			return it == skip_.end () ? k_default_value : it->second;
			}
			
#ifdef  B_ALGO_DEBUG
		void PrintSkipTable () const {
			std::cout << "BM(H) Skip Table <unordered_map>:" << std::endl;
			for ( typename skip_map::const_iterator it = skip_.begin (); it != skip_.end (); ++it )
				std::cout << "  " << it->first << ": " << it->second << std::endl;
			std::cout << std::endl;
			}
#endif
		};
		
	
//  Special case small numeric values; use an array
	template<typename Iter>
	class skip_table<Iter, true> {
	private:
		typedef typename Iter::value_type value_type;
		typedef typename boost::make_unsigned<value_type>::type unsigned_value_type;
		typedef boost::array<int, 1U << (CHAR_BIT * sizeof(value_type))> skip_map;
		skip_map skip_;
	public:
		skip_table ( std::size_t patSize, int default_value ) {
			std::fill_n ( skip_.begin(), skip_.size(), default_value );
			}
		
		void insert ( typename Iter::value_type key, int val ) {
			skip_ [ static_cast<unsigned_value_type> ( key ) ] = val;
			}

		int operator [] ( typename Iter::value_type val ) const {
			return skip_ [ static_cast<unsigned_value_type> ( val ) ];
			}

#ifdef  B_ALGO_DEBUG
		void PrintSkipTable () const {
			std::cout << "BM(H) Skip Table <boost:array>:" << std::endl;
			typename skip_map::const_iterator it = skip_.begin ();
			for ( typename skip_map::const_iterator it = skip_.begin (); it != skip_.end (); ++it )
				std::cout << "  " << std::distance (it, skip_.begin ()) << ": " << *it << std::endl;
			std::cout << std::endl;
			}
#endif
		};

	template<typename Iter>
	struct BM_traits {
		typedef typename Iter::value_type value_type;
		typedef boost::algorithm::detail::skip_table<Iter, 
				boost::is_integral<value_type>::value && (sizeof(value_type)==1)> skip_table_t;
		};
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
        * The two iterator types (patIter and corpusIter) must 
        	"point to" the same underlying type and be comparable.
        * Additional requirements may be imposed but the skip table, such as:
        ** Numeric type (array-based skip table)
        ** Hashable type (map-based skip table)
*/

    template <typename patIter, typename traits=detail::BM_traits<patIter> >
    class boyer_moore {
    public:
        boyer_moore ( patIter first, patIter last ) 
                : pat_first ( first ), pat_last ( last ),
                  k_pattern_length ( (std::size_t) std::distance ( pat_first, pat_last )),
                  skip_ ( k_pattern_length, -1 ),
                  suffix_ ( k_pattern_length + 1 ) {

        //  Build the skip table
            std::size_t i = 0;
            for ( patIter iter = pat_first; iter != pat_last; ++iter, ++i )
                skip_.insert ( *iter, i );
    
            this->create_suffix_table ( pat_first, pat_last );
#ifdef B_ALGO_DEBUG
            skip_.PrintSkipTable ();
            detail::PrintTable ( suffix_.begin (), suffix_.end ());
#endif
            }
            
        ~boyer_moore () {}
        
        template <typename corpusIter, typename Pred>
        corpusIter operator () ( corpusIter corpus_first, corpusIter corpus_last, Pred p ) {
            BOOST_STATIC_ASSERT (( boost::is_same<typename patIter::value_type, typename corpusIter::value_type>::value ));
            if ( corpus_first == corpus_last ) return corpus_last;  // if nothing to search, we didn't find it!
            if ( pat_first == pat_last ) return corpus_first;       // empty pattern matches at start

            const std::size_t k_corpus_length  = (std::size_t) std::distance ( corpus_first, corpus_last );
            if ( k_corpus_length < k_pattern_length ) 
                return corpus_last;     // If the pattern is larger than the corpus....
    
        /*  ---- Do the matching ---- */
            corpusIter curPos = corpus_first;
            const corpusIter lastPos = corpus_last - k_pattern_length;
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
                k = skip_ [ curPos [ j - 1 ]];
                m = j - k - 1;
                if ( k < (int) j && m > (int) suffix_ [ j ] )
                    curPos += m;
                else
                    curPos += suffix_ [ j ];
                }
        
            return corpus_last;     // We didn't find anything
            }
    
        template <typename corpusIter>
        corpusIter operator () ( corpusIter corpus_first, corpusIter corpus_last ) {
            return this->operator () ( corpus_first, corpus_last, 
                std::equal_to<typename corpusIter::value_type> ());
            }
        
    private:
        patIter pat_first, pat_last;
        const std::size_t k_pattern_length;
        typename traits::skip_table_t skip_;
        std::vector <std::size_t> suffix_;

        template<typename Iter, typename Container>
        void compute_bm_prefix ( Iter pat_first, Iter pat_last, Container &prefix ) {
            const std::size_t count = std::distance ( pat_first, pat_last );
        	assert ( count > 0 );
        	assert ( prefix.size () == count );
				        	
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

        void create_suffix_table ( patIter pat_first, patIter pat_last ) {
            const std::size_t count = (std::size_t) std::distance ( pat_first, pat_last );
            
            if ( count > 0 ) {	// empty pattern
				std::vector<typename patIter::value_type> reversed(count);     
				(void) std::copy_backward ( pat_first, pat_last, reversed.end ());
				
				std::vector<std::size_t> prefix (count);
				compute_bm_prefix ( pat_first, pat_last, prefix );
		
				std::vector<std::size_t> prefix_reversed (count);
				compute_bm_prefix ( reversed.begin (), reversed.end (), prefix_reversed );
				
				for ( std::size_t i = 0; i <= count; i++ )
					suffix_[i] = count - prefix [count-1];
		 
				for ( std::size_t i = 0; i < count; i++) {
					const std::size_t j = count - prefix_reversed[i];
					const std::size_t k = i -     prefix_reversed[i] + 1;
		 
					if (suffix_[j] > k)
						suffix_[j] = k;
					}
				}
			}        
        };
    

//  Bummer(1): We could make this better - remove code duplication
//  If only there were partial specialization of function templates
    template <typename patIter, typename corpusIter, typename Pred>
    corpusIter boyer_moore_search ( 
            corpusIter corpus_first, corpusIter corpus_last, 
            patIter pat_first, patIter pat_last, Pred p ) {
        boyer_moore <patIter> bm ( pat_first, pat_last );
        return bm ( corpus_first, corpus_last, p );
        }

    template <typename patIter, typename corpusIter>
    corpusIter boyer_moore_search ( 
            corpusIter corpus_first, corpusIter corpus_last, 
            patIter pat_first, patIter pat_last ) {
        boyer_moore <patIter> bm ( pat_first, pat_last );
        return bm ( corpus_first, corpus_last );
        }

/*
    A templated version of the boyer-moore-horspool searching algorithm.
    
    Requirements:
        * Random access iterators
        * The two iterator types (patIter and corpusIter) must 
        	"point to" the same underlying type and be comparable.
        * Additional requirements may be imposed but the skip table, such as:
        ** Numeric type (array-based skip table)
        ** Hashable type (map-based skip table)

http://www-igm.univ-mlv.fr/%7Elecroq/string/node18.html

*/

    template <typename patIter, typename traits=detail::BM_traits<patIter> >
    class boyer_moore_horspool {
    public:
        boyer_moore_horspool ( patIter first, patIter last ) 
                : pat_first ( first ), pat_last ( last ),
                  k_pattern_length ( (std::size_t) std::distance ( pat_first, pat_last )),
                  skip_ ( k_pattern_length, k_pattern_length ) {
                  
        //  Build the skip table
            std::size_t i = 0;
            if ( first != last )	// empty pattern?
            	for ( patIter iter = first; iter != last-1; ++iter, ++i )
                	skip_.insert ( *iter, k_pattern_length - 1 - i );
#ifdef B_ALGO_DEBUG
            skip_.PrintSkipTable ();
#endif
            }
            
        ~boyer_moore_horspool () {}
        
        template <typename corpusIter, typename Pred>
        corpusIter operator () ( corpusIter corpus_first, corpusIter corpus_last, Pred p ) {
            BOOST_STATIC_ASSERT (( boost::is_same<typename patIter::value_type, typename corpusIter::value_type>::value ));
            if ( corpus_first == corpus_last ) return corpus_last;  // if nothing to search, we didn't find it!
            if ( pat_first == pat_last ) return corpus_first;       // empty pattern matches at start

            const std::size_t k_corpus_length  = (std::size_t) std::distance ( corpus_first, corpus_last );
            if ( k_corpus_length < k_pattern_length ) 
                return corpus_last;     // If the pattern is larger than the corpus....
    
        /*  ---- Do the matching ---- */
            corpusIter curPos = corpus_first;
            const corpusIter lastPos = corpus_last - k_pattern_length;
            while ( curPos <= lastPos ) {
            //  Do we match right where we are?
                std::size_t j = k_pattern_length - 1;
                while ( p ( pat_first [j], curPos [j] )) {
                //  We matched - we're done!
                    if ( j == 0 )
                        return curPos;
                    j--;
                    }
        
                curPos += skip_ [ curPos [ k_pattern_length - 1 ]];
                }
            
            return corpus_last;
            }
    
        template <typename corpusIter>
        corpusIter operator () ( corpusIter corpus_first, corpusIter corpus_last ) {
            return this->operator () ( corpus_first, corpus_last, 
                std::equal_to<typename corpusIter::value_type> ());
            }
        
    private:
        patIter pat_first, pat_last;
        const std::size_t k_pattern_length;
        typename traits::skip_table_t skip_;
        };
    
//  Bummer(2): We could make this better - remove code duplication
//  If only there were partial specialization of function templates
    template <typename patIter, typename corpusIter, typename Pred>
    corpusIter boyer_moore_horspool_search ( 
            corpusIter corpus_first, corpusIter corpus_last, 
            patIter pat_first, patIter pat_last, Pred p) {
        boyer_moore_horspool <patIter> bmh ( pat_first, pat_last );
        return bmh ( corpus_first, corpus_last, p );
        }

    template <typename patIter, typename corpusIter>
    corpusIter boyer_moore_horspool_search ( 
            corpusIter corpus_first, corpusIter corpus_last, 
            patIter pat_first, patIter pat_last ) {
        boyer_moore_horspool<patIter> bmh ( pat_first, pat_last );
        return bmh ( corpus_first, corpus_last );
        }

/*
    A templated version of the Knuth-Pratt-Morris searching algorithm.
    
    Requirements:
        * Random-access iterators
        * The two iterator types (I1 and I2) must "point to" the same underlying type.

    http://en.wikipedia.org/wiki/Knuth–Morris–Pratt_algorithm
    http://www.inf.fh-flensburg.de/lang/algorithmen/pattern/kmpen.htm
*/

    template <typename patIter>
    class knuth_morris_pratt {
    public:
        knuth_morris_pratt ( patIter first, patIter last ) 
                : pat_first ( first ), pat_last ( last ),
                  k_pattern_length ( (std::size_t) std::distance ( pat_first, pat_last )),
                  skip_ ( k_pattern_length + 1 ) {
            init_skip_table ( pat_first, pat_last );
#ifdef B_ALGO_DEBUG
            detail::PrintTable ( skip_.begin (), skip_.end ());
#endif
            }
            
        ~knuth_morris_pratt () {}
        
        template <typename corpusIter, typename Pred>
        corpusIter operator () ( corpusIter corpus_first, corpusIter corpus_last, Pred p ) {
            BOOST_STATIC_ASSERT (( boost::is_same<typename patIter::value_type, typename corpusIter::value_type>::value ));
            if ( corpus_first == corpus_last ) return corpus_last;  // if nothing to search, we didn't find it!
            if ( pat_first == pat_last )       return corpus_first; // empty pattern matches at start

            const std::size_t k_corpus_length  = (std::size_t) std::distance ( corpus_first, corpus_last );
            if ( k_corpus_length < k_pattern_length ) 
                return corpus_last;     // If the pattern is larger than the corpus....

//  At this point, we know:
//          k_pattern_length <= k_corpus_length
//          for all elements of skip, it holds -1 .. k_pattern_length
//      
//          In the loop, we have the following invariants
//              idx is in the range 0 .. k_pattern_length
//              match_start is in the range 0 .. k_corpus_length - k_pattern_length + 1

            std::size_t idx = 0;          // position in the pattern we're comparing
            std::size_t match_start = 0;  // position in the corpus that we're matching
            const std::size_t last_match = k_corpus_length - k_pattern_length;
            while ( match_start <= last_match ) {
                while ( p ( pat_first [ idx ], corpus_first [ match_start + idx ] )) {
                    if ( ++idx == k_pattern_length )
                        return corpus_first + match_start;
                    }
            //  Figure out where to start searching again
           //   assert ( idx - skip_ [ idx ] > 0 ); // we're always moving forward
                match_start += idx - skip_ [ idx ];
                idx = skip_ [ idx ] >= 0 ? skip_ [ idx ] : 0;
           //   assert ( idx >= 0 && idx < k_pattern_length );
                }
                
        //  We didn't find anything
            return corpus_last;
            }
    
        template <typename corpusIter>
        corpusIter operator () ( corpusIter corpus_first, corpusIter corpus_last ) {
            return this->operator () ( corpus_first, corpus_last, 
                std::equal_to<typename corpusIter::value_type> ());
            }
        
    private:
        patIter pat_first, pat_last;
        const std::size_t k_pattern_length;
        std::vector <int> skip_;

		void init_skip_table ( patIter first, patIter last ) {
			const /*std::size_t*/ int count = std::distance ( first, last );
	
			int j;
			skip_ [ 0 ] = -1;
			for ( int i = 1; i < count; ++i ) {
				j = skip_ [ i - 1 ];
				while ( j >= 0 ) {
					if ( first [ j ] == first [ i - 1 ] )
						break;
					j = skip_ [ j ];
					}
				skip_ [ i ] = j + 1;
				}
			}    
        };
    
//  Bummer(3): We could make this better - remove code duplication
//  If only there were partial specialization of function templates
    template <typename patIter, typename corpusIter, typename Pred>
    corpusIter knuth_morris_pratt_search ( 
            corpusIter corpus_first, corpusIter corpus_last, 
            patIter pat_first, patIter pat_last, Pred p) {
        knuth_morris_pratt<patIter> kmp ( pat_first, pat_last );
        return kmp ( corpus_first, corpus_last, p );
        }

    template <typename patIter, typename corpusIter>
    corpusIter knuth_morris_pratt_search ( 
            corpusIter corpus_first, corpusIter corpus_last, 
            patIter pat_first, patIter pat_last ) {
        knuth_morris_pratt<patIter> kmp ( pat_first, pat_last );
        return kmp ( corpus_first, corpus_last );
        }
}}

#endif
