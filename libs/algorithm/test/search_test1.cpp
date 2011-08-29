/* 
   Copyright (c) Marshall Clow 2008-2010.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <boost/algorithm/search.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

#include <iostream>
#include <string>
#include <vector>



template <typename Iter>
std::string make_str ( Iter first, std::size_t len ) {
    std::string retVal ( len + 2, '\'' );
    std::copy ( first, first+len, retVal.begin () + 1);
    return retVal;
    }

namespace {

//  Check using iterators
    template<typename Container>
    void check_one_iter ( const Container &haystack, const std::string &needle, int expected ) {
        typedef typename Container::const_iterator iter_type;
        iter_type hBeg = haystack.begin ();
        iter_type hEnd = haystack.end ();
        iter_type nBeg = needle.begin ();
        iter_type nEnd = needle.end ();

        iter_type it0  = std::search                                  (hBeg, hEnd, nBeg, nEnd);
        iter_type it1  = boost::algorithm::boyer_moore_search         (hBeg, hEnd, nBeg, nEnd);
        iter_type it2  = boost::algorithm::boyer_moore_horspool_search(hBeg, hEnd, nBeg, nEnd);
        iter_type it3  = boost::algorithm::knuth_morris_pratt_search  (hBeg, hEnd, nBeg, nEnd);
        int dist = it1 == hEnd ? -1 : std::distance ( hBeg, it1 );

        std::cout << "Pattern is " << needle.length () << ", haysstack is " << haystack.length () << " chars long; " << std::endl;
        try {
            if ( it0 != it1 ) {
                throw std::runtime_error ( 
                    std::string ( "results mismatch between std::search and boyer-moore search" ));
                }

            if ( it1 != it2 ) {
                throw std::runtime_error ( 
                    std::string ( "results mismatch between boyer-moore and boyer-moore-horspool search" ));
                }

            if ( it1 != it3 )
                throw std::runtime_error ( 
                    std::string ( "results mismatch between boyer-moore and knuth-morris-pratt search" ));

            }

        catch ( ... ) {
            std::cout << "Searching for: " << needle << std::endl;
            std::cout << "Expected: " << expected << "\n";
            std::cout << "  std:    " << std::distance ( hBeg, it0 ) << "\n";
            std::cout << "  bm:     " << std::distance ( hBeg, it1 ) << "\n";
            std::cout << "  bmh:    " << std::distance ( hBeg, it2 ) << "\n";
            std::cout << "  kpm:    " << std::distance ( hBeg, it3 )<< "\n";
            std::cout << std::flush;
        //  throw;
            }

        BOOST_CHECK_EQUAL ( dist, expected );
        }

//  Check using pointers
    template<typename Container>
    void check_one_pointer ( const Container &haystack, const std::string &needle, int expected ) {
        typedef const typename Container::value_type *ptr_type;
        ptr_type hBeg = &*haystack.begin ();
        ptr_type hEnd = &*haystack.end ();
        ptr_type nBeg = &*needle.begin ();
        ptr_type nEnd = &*needle.end ();

        ptr_type it0  = std::search                                  (hBeg, hEnd, nBeg, nEnd);
        ptr_type it1  = boost::algorithm::boyer_moore_search         (hBeg, hEnd, nBeg, nEnd);
        ptr_type it2  = boost::algorithm::boyer_moore_horspool_search(hBeg, hEnd, nBeg, nEnd);
        ptr_type it3  = boost::algorithm::knuth_morris_pratt_search  (hBeg, hEnd, nBeg, nEnd);
        int dist = it1 == hEnd ? -1 : std::distance ( hBeg, it1 );

        std::cout << "Pattern is " << needle.length () << ", haysstack is " << haystack.length () << " chars long; " << std::endl;
        try {
            if ( it0 != it1 ) {
                throw std::runtime_error ( 
                    std::string ( "results mismatch between std::search and boyer-moore search" ));
                }

            if ( it1 != it2 ) {
                throw std::runtime_error ( 
                    std::string ( "results mismatch between boyer-moore and boyer-moore-horspool search" ));
                }

            if ( it1 != it3 )
                throw std::runtime_error ( 
                    std::string ( "results mismatch between boyer-moore and knuth-morris-pratt search" ));

            }

        catch ( ... ) {
            std::cout << "Searching for: " << needle << std::endl;
            std::cout << "Expected: " << expected << "\n";
            std::cout << "  std:    " << std::distance ( hBeg, it0 ) << "\n";
            std::cout << "  bm:     " << std::distance ( hBeg, it1 ) << "\n";
            std::cout << "  bmh:    " << std::distance ( hBeg, it2 ) << "\n";
            std::cout << "  kpm:    " << std::distance ( hBeg, it3 )<< "\n";
            std::cout << std::flush;
        //  throw;
            }

        BOOST_CHECK_EQUAL ( dist, expected );
        }

    template<typename Container>
    void check_one ( const Container &haystack, const std::string &needle, int expected ) {
        check_one_iter ( haystack, needle, expected );
        check_one_pointer ( haystack, needle, expected );
        }
    }


int test_main( int , char* [] )
{
    std::string haystack1 ( "NOW AN FOWE\220ER ANNMAN THE ANPANMANEND" );
    std::string needle1   ( "ANPANMAN" );
    std::string needle2   ( "MAN THE" );
    std::string needle3   ( "WE\220ER" );
    std::string needle4   ( "NOW " );   //  At the beginning
    std::string needle5   ( "NEND" );   //  At the end
    std::string needle6   ( "NOT FOUND" );  // Nowhere
    std::string needle7   ( "NOT FO\340ND" );   // Nowhere

    std::string haystack2 ( "ABC ABCDAB ABCDABCDABDE" );
    std::string needle11  ( "ABCDABD" );

    std::string haystack3 ( "abra abracad abracadabra" );
    std::string needle12  ( "abracadabra" );

    std::string needle13   ( "" );
    std::string haystack4  ( "" );

    check_one ( haystack1, needle1, 26 );
    check_one ( haystack1, needle2, 18 );
    check_one ( haystack1, needle3,  9 );
    check_one ( haystack1, needle4,  0 );
    check_one ( haystack1, needle5, 33 );
    check_one ( haystack1, needle6, -1 );
    check_one ( haystack1, needle7, -1 );

    check_one ( needle1, haystack1, -1 );   // cant find long pattern in short corpus
    check_one ( haystack1, haystack1, 0 );  // find something in itself

    check_one ( haystack2, needle11, 15 );
    check_one ( haystack3, needle12, 13 );

    check_one ( haystack1, needle13, 0 );   // find the empty string 
    check_one ( haystack4, needle1, -1 );  // can't find in an empty haystack

//	Mikhail Levin <svarneticist@gmail.com> found a problem, and this was the test
//	that triggered it.

  const std::string mikhail_pattern =	
"GATACACCTACCTTCACCAGTTACTCTATGCACTAGGTGCGCCAGGCCCATGCACAAGGGCTTGAGTGGATGGGAAGGA"
"TGTGCCCTAGTGATGGCAGCATAAGCTACGCAGAGAAGTTCCAGGGCAGAGTCACCATGACCAGGGACACATCCACGAG"
"CACAGCCTACATGGAGCTGAGCAGCCTGAGATCTGAAGACACGGCCATGTATTACTGTGGGAGAGATGTCTGGAGTGGT"
"TATTATTGCCCCGGTAATATTACTACTACTACTACTACATGGACGTCTGGGGCAAAGGGACCACG"
;
	const std::string mikhail_corpus = std::string (8, 'a') + mikhail_pattern;

	check_one ( mikhail_corpus, mikhail_pattern, 8 );
    return 0;
    }
