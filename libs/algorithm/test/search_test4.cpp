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

    bool Rot13 ( char ch1, char ch2 ) {
    	if ( ch1 >= 'a' && ch1 <= 'z' ) ch1 = "nopqrstuvwxyzabcdefghijklm" [ ch1 - 'a' ];
    	if ( ch1 >= 'A' && ch1 <= 'Z' ) ch1 = "NOPQRSTUVWXYZABCDEFGHIJKLM" [ ch1 - 'A' ];
    	return ch1 == ch2;
    	}
    
    

    template<typename Container>
    void check_one ( const Container &haystack, const std::string &needle, int expected ) {
        typename Container::const_iterator it0p = std::search                                  (haystack.begin (), haystack.end (), needle.begin (), needle.end (), Rot13);
        typename Container::const_iterator it1p = boost::algorithm::boyer_moore_search         (haystack.begin (), haystack.end (), needle.begin (), needle.end (), Rot13);
        typename Container::const_iterator it2p = boost::algorithm::boyer_moore_horspool_search(haystack.begin (), haystack.end (), needle.begin (), needle.end (), Rot13);
        typename Container::const_iterator it3p = boost::algorithm::knuth_morris_pratt_search  (haystack.begin (), haystack.end (), needle.begin (), needle.end (), Rot13);
        int dist = it1p == haystack.end () ? -1 : std::distance ( haystack.begin(), it1p );
        
        std::cout << "Pattern is " << needle.length () << ", haysstack is " << haystack.length () << " chars long; " << std::endl;
        try {

            if ( it0p != it1p ) {
                throw std::runtime_error ( 
                    std::string ( "results mismatch between std::search and boyer-moore search" ));
                }

            if ( it1p != it2p ) {
                throw std::runtime_error ( 
                    std::string ( "results mismatch between boyer-moore and boyer-moore-horspool search" ));
                }
            
            if ( it1p != it3p )
                throw std::runtime_error ( 
                    std::string ( "results mismatch between boyer-moore and knuth-morris-pratt search" ));

			if ( dist != expected )
                throw std::runtime_error ( 
                    std::string ( "results mismatch between actual and expected" ));
				
            }

        catch ( ... ) {
            std::cout << "Searching for: " << needle << std::endl;
            std::cout << "Expected: " << expected << "\n";
            std::cout << "  std(p): " << it0p - haystack.begin () << "\n";
            std::cout << "  bm(p):  " << it1p - haystack.begin () << "\n";
            std::cout << "  bmh(p): " << it2p - haystack.begin () << "\n";
            std::cout << "  kpm(p): " << it3p - haystack.begin () << "\n";
            std::cout << std::flush;
        //  throw;
            }
        
        BOOST_CHECK_EQUAL ( dist, expected );
        }
    }

int test_main( int , char* [] )
{
    std::string haystack1 ( "ABCDEFGHIJKLMNOPQRSTUVWXYZ" );
    std::string needle1   ( "ABC" );
    std::string needle2   ( "NOPQ" );
    
    std::string haystack2 ( "ABC ABCDAB ABCDABCDABDE" );
    std::string needle11  ( "NOPQNOQ" );
    
    check_one ( haystack1, needle1, 13 );
    check_one ( haystack1, needle2,  0 );
    check_one ( haystack2, needle11, 15 );


    return 0;
    }
