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

//  A simple predicate for testing the predicate versions
    bool Equal ( char ch1, char ch2 ) {   return ~ch1 == ~ch2; }
    

    template<typename Container>
    void check_one ( const Container &haystack, const std::string &needle, int expected ) {
        typename Container::const_iterator it0  = std::search                                  (haystack.begin (), haystack.end (), needle.begin (), needle.end ());
        typename Container::const_iterator it0p = std::search                                  (haystack.begin (), haystack.end (), needle.begin (), needle.end (), Equal);
        typename Container::const_iterator it1  = boost::algorithm::boyer_moore_search         (haystack.begin (), haystack.end (), needle.begin (), needle.end ());
        typename Container::const_iterator it1p = boost::algorithm::boyer_moore_search         (haystack.begin (), haystack.end (), needle.begin (), needle.end (), Equal);
        typename Container::const_iterator it2  = boost::algorithm::boyer_moore_horspool_search(haystack.begin (), haystack.end (), needle.begin (), needle.end ());
        typename Container::const_iterator it2p = boost::algorithm::boyer_moore_horspool_search(haystack.begin (), haystack.end (), needle.begin (), needle.end (), Equal);
        typename Container::const_iterator it3  = boost::algorithm::knuth_morris_pratt_search  (haystack.begin (), haystack.end (), needle.begin (), needle.end ());
        typename Container::const_iterator it3p = boost::algorithm::knuth_morris_pratt_search  (haystack.begin (), haystack.end (), needle.begin (), needle.end (), Equal);
        int dist = it1 == haystack.end () ? -1 : std::distance ( haystack.begin(), it1 );
        
        std::cout << "Pattern is " << needle.length () << ", haysstack is " << haystack.length () << " chars long; " << std::endl;
        try {
            if ( it0 != it0p || it1 != it1p || it2 != it2p || it3 != it3p )
                throw std::runtime_error ( 
                    std::string ( "results mismatch between predicate and non-predicate search" ));
        
            if ( it0 != it1 || it0p != it1p ) {
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
            std::cout << "  std:    " << it0  - haystack.begin () << "\n";
            std::cout << "  std(p): " << it0p - haystack.begin () << "\n";
            std::cout << "  bm:     " << it1  - haystack.begin () << "\n";
            std::cout << "  bm(p):  " << it1p - haystack.begin () << "\n";
            std::cout << "  bmh:    " << it2  - haystack.begin () << "\n";
            std::cout << "  bmh(p): " << it2p - haystack.begin () << "\n";
            std::cout << "  kpm:    " << it3  - haystack.begin () << "\n";
            std::cout << "  kpm(p): " << it3p - haystack.begin () << "\n";
            std::cout << std::flush;
        //  throw;
            }
        
        BOOST_CHECK_EQUAL ( dist, expected );
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

    return 0;
    }
