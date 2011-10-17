/*
    Copyright Marshall Clow 2010-2011. Use, modification and
    distribution is subject to the Boost Software License, Version
    1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#ifndef BOOST_ALGORITHM_BOYER_MOORE_SEARCH_HPP
#define BOOST_ALGORITHM_BOYER_MOORE_SEARCH_HPP

#include <iterator>     // for std::iterator_traits

#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/algorithm/searching/detail/bm_traits.hpp>
#include <boost/algorithm/searching/detail/debugging.hpp>


// #define  BOOST_ALGORITHM_BOYER_MOORE_DEBUG_HPP

namespace boost { namespace algorithm {

/*
    A templated version of the boyer-moore searching algorithm.
    
References:
    http://www.cs.utexas.edu/users/moore/best-ideas/string-searching/
    http://www.cs.utexas.edu/~moore/publications/fstrpos.pdf
    
Explanations:   boostinspect:noascii (test tool complains)
    http://en.wikipedia.org/wiki/Boyer–Moore_string_search_algorithm
    http://www.movsd.com/bm.htm
    http://www.cs.ucdavis.edu/~gusfield/cs224f09/bnotes.pdf

The Boyer-Moore search algorithm uses two tables, a "bad character" table
to tell how far to skip ahead when it hits a character that is not in the pattern,
and a "good character" table to tell how far to skip ahead when it hits a
mismatch on a character that _is_ in the pattern.

Requirements:
        * Random access iterators
        * The two iterator types (patIter and corpusIter) must 
            "point to" the same underlying type and be comparable.
        * Additional requirements may be imposed but the skip table, such as:
        ** Numeric type (array-based skip table)
        ** Hashable type (map-based skip table)
*/

    template <typename patIter, typename traits = detail::BM_traits<typename std::iterator_traits<patIter>::value_type> >
    class boyer_moore {
    public:
        boyer_moore ( patIter first, patIter last ) 
                : pat_first ( first ), pat_last ( last ),
                  k_pattern_length ( (std::size_t) std::distance ( pat_first, pat_last )),
                  skip_ ( k_pattern_length, -1 ),
                  suffix_ ( k_pattern_length + 1 )
            {
#ifdef  BOOST_ALGORITHM_BOYER_MOORE_DEBUG_HPP
            std::cout << "Pattern length: " << k_pattern_length << std::endl;
#endif
            this->build_bm_tables ();
            }
            
        ~boyer_moore () {}
        
        /// \fn operator ( corpusIter corpus_first, corpusIter corpus_last )
        /// \brief Searches the corpus for the pattern that was passed into the constructor
        /// 
        /// \param corpus_first The start of the data to search (Random Access Iterator)
        /// \param corpus_last  One past the end of the data to search
        ///
        template <typename corpusIter>
        corpusIter operator () ( corpusIter corpus_first, corpusIter corpus_last ) const {
            BOOST_STATIC_ASSERT (( boost::is_same<
                                    typename std::iterator_traits<patIter>::value_type, 
                                    typename std::iterator_traits<corpusIter>::value_type>::value ));

            if ( corpus_first == corpus_last ) return corpus_last;  // if nothing to search, we didn't find it!
            if (    pat_first ==    pat_last ) return corpus_first; // empty pattern matches at start

            const std::size_t k_corpus_length  = (std::size_t) std::distance ( corpus_first, corpus_last );
#ifdef  BOOST_ALGORITHM_BOYER_MOORE_DEBUG_HPP
            std::cout << "Corpus length: " << k_corpus_length << std::endl;
#endif
        //  If the pattern is larger than the corpus, we can't find it!
            if ( k_corpus_length < k_pattern_length ) 
                return corpus_last;

        //  Do the search 
            return this->do_search   ( corpus_first, corpus_last, k_corpus_length );
            }
            
    private:
/// \cond DOXYGEN_HIDE
        patIter pat_first, pat_last;
        const std::size_t k_pattern_length;
        typename traits::skip_table_t skip_;
        std::vector <std::size_t> suffix_;

        void build_bm_tables () {
        //  Build the skip table
            std::size_t i = 0;
            for ( patIter iter = pat_first; iter != pat_last; ++iter, ++i )
                skip_.insert ( *iter, i );
    
            this->create_suffix_table ( pat_first, pat_last );
                
#ifdef BOOST_ALGORITHM_BOYER_MOORE_DEBUG_HPP
            skip_.PrintSkipTable ();
            std::cout << "Boyer Moore suffix table:" << std::endl;
            std::cout << "  " << 0 << ": " << suffix_[0] << std::endl;
            for ( i = 0; i < suffix_.size (); ++i )
                if ( suffix_[i] != suffix_[0] )
                    std::cout << "  " << i << ": " << suffix_[i] << std::endl;
//          detail::PrintTable ( suffix_.begin (), suffix_.end ());
#endif
            }
        

        /// \fn operator ( corpusIter corpus_first, corpusIter corpus_last, Pred p )
        /// \brief Searches the corpus for the pattern that was passed into the constructor
        /// 
        /// \param corpus_first The start of the data to search (Random Access Iterator)
        /// \param corpus_last  One past the end of the data to search
        /// \param p            A predicate used for the search comparisons.
        ///
        template <typename corpusIter>
        corpusIter do_search ( corpusIter corpus_first, corpusIter corpus_last, std::size_t k_corpus_length ) const {
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
                while ( pat_first [j-1] == curPos [j-1] ) {
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


        template<typename Iter, typename Container>
        void compute_bm_prefix ( Iter pat_first, Iter pat_last, Container &prefix ) {
            const std::size_t count = std::distance ( pat_first, pat_last );
            BOOST_ASSERT ( count > 0 );
            BOOST_ASSERT ( prefix.size () == count );
                            
            prefix[0] = 0;
            std::size_t k = 0;
            for ( std::size_t i = 1; i < count; ++i ) {
                BOOST_ASSERT ( k < count );
                while ( k > 0 && ( pat_first[k] != pat_first[i] )) {
                    BOOST_ASSERT ( k < count );
                    k = prefix [ k - 1 ];
                    }
                    
                if ( pat_first[k] == pat_first[i] )
                    k++;
                prefix [ i ] = k;
                }
            }

        void create_suffix_table ( patIter pat_first, patIter pat_last ) {
            const std::size_t count = (std::size_t) std::distance ( pat_first, pat_last );
            
            if ( count > 0 ) {  // empty pattern
                std::vector<typename std::iterator_traits<patIter>::value_type> reversed(count);
                (void) std::reverse_copy ( pat_first, pat_last, reversed.begin ());
                
                std::vector<std::size_t> prefix (count);
                compute_bm_prefix ( pat_first, pat_last, prefix );
        
                std::vector<std::size_t> prefix_reversed (count);
                compute_bm_prefix ( reversed.begin (), reversed.end (), prefix_reversed );
                
                for ( std::size_t i = 0; i <= count; i++ )
                    suffix_[i] = count - prefix [count-1];
         
                for ( std::size_t i = 0; i < count; i++ ) {
                    const std::size_t j = count - prefix_reversed[i];
                    const std::size_t k = i -     prefix_reversed[i] + 1;
         
                    if (suffix_[j] > k)
                        suffix_[j] = k;
                    }
                }
            }
/// \endcond
        };

/// \fn boyer_moore_search ( corpusIter corpus_first, corpusIter corpus_last, 
///       patIter pat_first, patIter pat_last )
/// \brief Searches the corpus for the pattern.
/// 
/// \param corpus_first The start of the data to search (Random Access Iterator)
/// \param corpus_last  One past the end of the data to search
/// \param pat_first    The start of the pattern to search for (Random Access Iterator)
/// \param pat_last     One past the end of the data to search for
///
    template <typename patIter, typename corpusIter>
    corpusIter boyer_moore_search ( corpusIter corpus_first, corpusIter corpus_last, 
                                            patIter pat_first, patIter pat_last ) {
        boyer_moore<patIter> bm ( pat_first, pat_last );
        return bm ( corpus_first, corpus_last );
        }

}}

#endif  //  BOOST_ALGORITHM_BOYER_MOORE_SEARCH_HPP
