/*
	Search algorithms
		* Boyer-Moore
		* Boyer-Moore-Horspool
		* Knuth-Morris-Pratt

    Copyright Marshall Clow 2010. Use, modification and
    distribution is subject to the Boost Software License, Version
    1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)

	For more information, see http://www.boost.org
	
	05 Nov 2010 - Initial public version
*/

#ifndef BOOST_ALGORITHM_SEARCH_HPP
#define BOOST_ALGORITHM_SEARCH_HPP

// #define	DEBUG

#ifdef	DEBUG
#include <iostream>
#include <string>
#endif


#include <vector>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/static_assert.hpp>

namespace boost { namespace algorithm {

namespace detail {

#ifdef	DEBUG
//	Debugging support
	template <typename Iter>
	void PrintTable ( Iter first, Iter last ) {
		std::cout << std::distance ( first, last ) << ": { ";
		for ( Iter iter = first; iter != last; ++iter )
			std::cout << *iter << " ";
		std::cout << "}" << std::endl;
		}
	
	void PrintSkipTable ( int *table, std::size_t size ) {
		for ( std::size_t idx = 0; idx < size; ++idx )
			if ( table[idx] != -1 )
				std::cout << "  " << (char) idx << ": " << table[idx] << std::endl;
		std::cout << std::endl;
		}
	
	
	template <typename Iter>
	std::string make_str ( Iter first, std::size_t len ) {
		std::string retVal ( len + 2, '\'' );
		std::copy ( first, first+len, retVal.begin () + 1);
		return retVal;
		}
#endif


//
//	--------
//	This whole block of code is designed to take an integral value from
//		the iterator, and return it as an unsigned value, suitable for using
//		as an index into a table.

//	The 'uv' template gives a type that is the same size as the input type,
//		only unsigned

	template <int sz> struct uv    { typedef std::size_t type; };
	template <>       struct uv<1> { typedef ::uint8_t   type; };
	template <>       struct uv<2> { typedef ::uint16_t  type; };
	template <>       struct uv<4> { typedef ::uint32_t  type; };
	BOOST_STATIC_ASSERT ( sizeof ( uv<1>::type ) == 1 );
	BOOST_STATIC_ASSERT ( sizeof ( uv<2>::type ) == 2 );
	BOOST_STATIC_ASSERT ( sizeof ( uv<4>::type ) == 4 );

//	The at_s takes a Iterator, and indirects it and returns an unsigned value.
	template<typename Iter, bool is_unsigned> struct at_s;

	template<typename Iter>
	struct at_s<Iter, true> {	// for unsigned values, we just return it.
		std::size_t operator () ( Iter iter ) const {
			return std::size_t ( *iter );
			}
	};
	
	template<typename Iter>
	struct at_s<Iter, false> {	// for signed values, we cast, then return it.
		typedef typename uv<sizeof (typename Iter::value_type)>::type Iter_unsigned;
	//	Rather than casting from the signed type to the unsigned type, 
	//		we make a pointer to the value, but the pointer is to an unsigned
	//		type of the same size as the original (signed value).
	//	Thanks to Dave Abrahams for the suggestion.
		std::size_t operator () ( Iter iter ) const {
			Iter_unsigned const *p = reinterpret_cast<Iter_unsigned const *> ( &*iter );
			return std::size_t ( *p );
			}
	};
	
//	A function that uses one of the above structures.	
	template<typename Iter>
	std::size_t at ( Iter iter ) {
		return at_s<Iter, boost::is_unsigned<typename Iter::value_type>::value >() ( iter );
		}
//
//	--------

	template<typename Iter>
	void compute_bm_prefix ( Iter pat_first, Iter pat_last, std::size_t *prefix /* [count] */ ) {
		const std::size_t count = std::distance ( pat_first, pat_last );
	
		prefix[0] = 0;
		int k = 0;
		for ( std::size_t i = 1; i < count; ++i ) {
			while ( k > 0 && pat_first[k] != pat_first[i] )
				k = prefix [ k - 1 ];
			if ( pat_first[k] == pat_first[i] )
				k++;
			prefix [ i ] = k;
			}
		}
	

	template<typename Iter>
	void create_bm_suffix_table ( Iter pat_first, Iter pat_last, std::size_t *result /* count+1 */ ) {
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
	
	template<typename Iter>
	void create_kmp_table ( Iter pat_first, Iter pat_last, int *skip /* count+1 */ ) {
		const /*std::size_t*/ int count = std::distance ( pat_first, pat_last );

	    int j;
	    skip [ 0 ] = -1;
	    for ( int i = 1; i < count; ++i ) {
	    	j = skip [ i - 1 ];
	    	while ( j >= 0 ) {
	    		if ( pat_first [ j ] == pat_first [ i - 1 ] )
	    			break;
	    		j = skip [ j ];
	    		}
	    	skip [ i ] = j + 1;
	    	}
		}
		
//	A simple comparator.
//	Uses operator == to compare two (possibly different) types.
//	Will fail to compile if no comparision exists
	template <typename V1, typename V2>
	struct equality_predicate {
		bool operator () ( const V1 &v1, const V2 &v2 ) const { return v1 == v2; }
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
		* value types are convertable to an integral type
		* Small size for the pattern type (usually a byte).
	Internally, this algorithm builds a table of all possible values. For 8-bit
	quantities, that means a 256 entry table. For 16-bit ones, that's a 64K entry
	table, and so on. 
		
		* Equality comparison for the value type of the iterators (I1 and I2)
	In particular, you should note that I1::value_type and I2::value_type do not have
	to be the same type, or even the same size - they just need to be comparable; i.e,
	for two iterators 
		I1 iter1 and I2 iter2
	
		then "*iter1 == *iter2" needs to be a valid expression.
*/

template <typename I1, typename I2, typename Pred>
I2 boyer_moore_search ( I1 corpus_first, I1 corpus_last,
					I2 pat_first, I2 pat_last, Pred p ) {
	if ( pat_first == pat_last ) return corpus_first;	// empty pattern matches at start
	if ( corpus_first == corpus_last ) return corpus_last;	// if nothing to search, we didn't find it!
	
	const std::size_t k_alphabet_size = 1U << ( CHAR_BIT * sizeof ( typename I2::value_type ));
	const /*std::size_t*/ int k_pattern_length = std::distance ( pat_first, pat_last );
	if ( std::distance ( corpus_first, corpus_last ) < k_pattern_length ) 
		return corpus_last;		// If the pattern is larger than the corpus....

//	Allocate the skip table and populate it with the analysis of the needle
	std::vector <int> skip ( k_alphabet_size, -1 );
	std::size_t i = 0;
	for ( I1 iter = pat_first; iter != pat_last; ++iter, ++i )
		skip [ detail::at ( iter ) ] = i;
//	detail::PrintSkipTable ( &*skip.begin (), k_alphabet_size);
	
/*	Prepare the suffix table */
	std::vector <std::size_t> suffix ( k_pattern_length + 1 );
	detail::create_bm_suffix_table ( pat_first, pat_last, &*suffix.begin ());
#ifdef DEBUG
	detail::PrintTable ( suffix.begin (), suffix.end ());
#endif

/*	---- Do the matching ---- */
#ifdef DEBUG
	I2 lastGoRound;
#endif
	I2 curPos = corpus_first;
	const I2 lastPos = corpus_last - k_pattern_length;
	std::size_t j;
	int k;
	int m;
	while ( curPos <= lastPos ) {
/*	while ( std::distance ( curPos, corpus_last ) >= k_pattern_length ) { */
	//	Do we match right where we are?
		j = k_pattern_length;
		while ( p ( pat_first [j-1], curPos [j-1] )) {
			j--;
		//	We matched - we're done!
			if ( j == 0 )
				return curPos;
			}
		
	//	Since we didn't match, figure out how far to skip forward
		k = skip [ detail::at ( curPos + j - 1 ) ];
		m = j - k - 1;
		if ( k < (int) j && m > (int) suffix [ j ] )
			curPos += m;
		else
			curPos += suffix [ j ];
		}

	return corpus_last;		// We didn't find anything
	}

//	Default predicate version
template <typename I1, typename I2>
I2 boyer_moore_search ( I1 corpus_first, I1 corpus_last, I2 pat_first, I2 pat_last ) {
	return boyer_moore_search ( corpus_first, corpus_last, pat_first, pat_last, 
		detail::equality_predicate<typename I1::value_type, typename I2::value_type> ());
	}

/*
	A templated version of the boyer-moore-horspool searching algorithm.
	
	Requirements:
		* Random-access iterators
		* value types convertible to a std::size_t
		* Equality comparison for the value type of the iterators (I1 and I2)
	In particular, you should note that I1::value_type and I2::value_type do not have
	to be the same type, or even the same size - they just need to be comparable; i.e,
	for two iterators 
		I1 iter1 and I2 iter2
	
		then "*iter1 == *iter2" needs to be a valid expression.

http://www-igm.univ-mlv.fr/%7Elecroq/string/node18.html

*/
template <typename I1, typename I2, typename Pred>
I2 boyer_moore_horspool_search ( I1 corpus_first, I1 corpus_last,
					I2 pat_first, I2 pat_last, Pred p ) {
	if ( pat_first == pat_last ) return corpus_first;	// empty pattern matches at start
	if ( corpus_first == corpus_last ) return corpus_last;	// if nothing to search, we didn't find it!
	
	const std::size_t k_alphabet_size = 1U << ( CHAR_BIT * sizeof ( typename I2::value_type ));
	const /*std::size_t*/ int k_pattern_length = std::distance ( pat_first, pat_last );
	if ( std::distance ( corpus_first, corpus_last ) < k_pattern_length ) 
		return corpus_last;		// If the pattern is larger than the corpus....

//	Create the skip table
	std::vector <int> skip ( k_alphabet_size, k_pattern_length );
/*	and populate it with the analysis of the needle */
	std::size_t i = 0;
	for ( I1 iter = pat_first; iter != pat_last-1; ++iter, ++i )
		skip [ detail::at ( iter ) ] = ( k_pattern_length - 1 ) - i;
		
/*	---- Do the matching ---- */
	I2 curPos = corpus_first;
	const I2 lastPos = corpus_last - k_pattern_length;
	while ( curPos <= lastPos ) {
/*	while ( std::distance ( curPos, corpus_last ) >= k_pattern_length ) { */

	//	Do we match right where we are?
		std::size_t j = k_pattern_length - 1;
		while ( p ( pat_first [j], curPos [j] )) {
		//	We matched - we're done!
			if ( j == 0 )
				return curPos;
			j--;
			}

		curPos += skip [ detail::at ( curPos + k_pattern_length - 1 ) ];
		}
	
	return corpus_last;
	}

//	Default predicate version
template <typename I1, typename I2>
I2 boyer_moore_horspool_search ( I1 corpus_first, I1 corpus_last, I2 pat_first, I2 pat_last ) {
	return boyer_moore_horspool_search ( corpus_first, corpus_last, pat_first, pat_last, 
		detail::equality_predicate<typename I1::value_type, typename I2::value_type> ());
	}


/*
	A templated version of the Knuth-Pratt-Morris searching algorithm.
	
	Requirements:
		* Random-access iterators
		* Equality comparison for the value type of the iterators (I1 and I2)
	In particular, you should note that I1::value_type and I2::value_type do not have
	to be the same type, or even the same size - they just need to be comparable; i.e,
	for two iterators 
		I1 iter1 and I2 iter2
	
		then "*iter1 == *iter2" needs to be a valid expression.

	http://en.wikipedia.org/wiki/Knuth–Morris–Pratt_algorithm
	http://www.inf.fh-flensburg.de/lang/algorithmen/pattern/kmpen.htm
*/

template <typename I1, typename I2, typename Pred>
I2 knuth_morris_pratt_search ( I1 corpus_first, I1 corpus_last,
					I2 pat_first, I2 pat_last, Pred p ) {
	if ( pat_first == pat_last ) return corpus_first;	// empty pattern matches at start
	if ( corpus_first == corpus_last ) return corpus_last;	// if nothing to search, we didn't find it!

//	Build the skip table
	const std::size_t k_pattern_length = (std::size_t) std::distance ( pat_first, pat_last );
	const std::size_t k_corpus_length  = (std::size_t) std::distance ( corpus_first, corpus_last );
	if ( k_corpus_length < k_pattern_length ) 
		return corpus_last;		// If the pattern is larger than the corpus....

	std::vector <int> skip ( k_pattern_length + 1 );
	detail::create_kmp_table ( pat_first, pat_last, &*skip.begin ());
	
/*	---- Do the matching ---- */
	std::size_t idx = 0;
	/* std::size_t*/ int j = 0;
	I2 curPos = corpus_first;
	const I2 lastPos = corpus_last - k_pattern_length;
//	while ( curPos <= lastPos ) {
	while ( idx < k_corpus_length ) {
#if 0
//	Do we match right where we are?
	//	std::cout << "Looking for match at position " << std::distance ( corpus_first, curPos ) << std::endl;
	//	std::cout << "Starting at offset " << j << std::endl;
		if ( p ( pat_first [j], curPos[j] )) {
		//	We matched - we're done!
			if ( ++j == k_pattern_length )
				return curPos;
			}
		else {
		//	std::cout << "Mismatch found after comparing " << j << " chars" << std::endl;
		//	std::cout << "Skip table = " << skip [ j ] << std::endl <<std::endl;
			curPos += j - skip [ j ];
		//	j = skip [ j ] > 0 ? skip [ j ] : 0;
			if ( j > 0 ) j = skip [ j ];
			}
#else
		while ( j >= 0 && !p ( pat_first [j], corpus_first[idx] ))
			j = skip [ j ];
		idx ++;
		if ( ++j == (int) k_pattern_length )
			return corpus_first + idx - j;
#endif
		}
	
//	We didn't find anything
	return corpus_last;
	}

//	Default predicate version
template <typename I1, typename I2>
I2 knuth_morris_pratt_search ( I1 corpus_first, I1 corpus_last, I2 pat_first, I2 pat_last ) {
	return knuth_morris_pratt_search ( corpus_first, corpus_last, pat_first, pat_last, 
		detail::equality_predicate<typename I1::value_type, typename I2::value_type> ());
	}

}}

#endif
