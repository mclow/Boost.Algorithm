/*
	Boyer Moore Search

References:
	http://www.cs.utexas.edu/users/moore/best-ideas/string-searching/
	http://www.cs.utexas.edu/~moore/publications/fstrpos.pdf
	
Explanations:
	http://en.wikipedia.org/wiki/Boyer-Moore_string_search_algorithm
	http://www.movsd.com/bm.htm
	http://www.cs.ucdavis.edu/~gusfield/cs224f09/bnotes.pdf


Since I have to make a copy of the needle, should I just save it
and the tables in an object, so that people can reuse the precomputed
tables (looking for the same thing in several cases?

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

//	A simple predicate for testing the predicate versions
	bool Equal ( const char &ch1, const char &ch2 ) {	return ~ch1 == ~ch2; }
	

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
		
	//		BOOST_CHECK_EQUAL ( it1, it2 );
	//		BOOST_CHECK_EQUAL ( it1, it3 );
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
		//	throw;
			}
		
		BOOST_CHECK_EQUAL ( dist, expected );
		 
#if 0
		std::cout << "Pattern is " << needle.length () << ", haysstack is " << haystack.length () << " chars long; ";
		if ( it1 == haystack.end ())
			std::cout << "not found" << std::endl;
		else {
			dist = std::distance ( haystack.begin(), it1 );
			std::cout << "found at pos " << dist << std::endl;
			std::cout << "Looking for string '" << needle << "'" << std::endl;
			std::cout << "Found string       "  << make_str ( haystack.begin () + dist, needle.size ()) << std::endl;
			}
		
		if ( it1 != it2 ) {
			int d2 = it2 == haystack.end () ? -1 : std::distance ( haystack.begin(), it2 );
			std::cout << "#### Mismatch (1) result! " << dist << " vs. " << d2 << std::endl;
			}
			
		if ( it1 != it3 ) {
			int d2 = it2 == haystack.end () ? -1 : std::distance ( haystack.begin(), it3 );
			std::cout << "#### Mismatch (2) result! " << dist << " vs. " << d2 << std::endl;
			}
			
		if ( dist != expected )
			std::cout << "#### Unexpected result! " << expected << " vs. " << dist << std::endl;
		std::cout << "-------------" << std::endl;
#endif
		}
	}

int test_main( int , char* [] )
{
	std::string haystack1 ( "NOW AN FOWE\220ER ANNMAN THE ANPANMANEND" );
	std::string needle1   ( "ANPANMAN" );
	std::string needle2   ( "MAN THE" );
	std::string needle3   ( "WE\220ER" );
	std::string needle4   ( "NOW " );	//	At the beginning
	std::string needle5   ( "NEND" );	//  At the end
	std::string needle6   ( "NOT FOUND" );	// Nowhere
	std::string needle7   ( "NOT FO\340ND" );	// Nowhere

	std::string haystack2 ( "ABC ABCDAB ABCDABCDABDE" );
	std::string needle11  ( "ABCDABD" );
	
	std::string haystack3 ( "abra abracad abracadabra" );
	std::string needle12  ( "abracadabra" );

	check_one ( haystack1, needle1, 26 );
	check_one ( haystack1, needle2, 18 );
	check_one ( haystack1, needle3,  9 );
	check_one ( haystack1, needle4,  0 );
	check_one ( haystack1, needle5, 33 );
	check_one ( haystack1, needle6, -1 );
	check_one ( haystack1, needle7, -1 );

	check_one ( needle1, haystack1, -1 );	// cant find long pattern in short corpus
	check_one ( haystack1, haystack1, 0 );	// find something in itself
	
	check_one ( haystack2, needle11, 15 );
	check_one ( haystack3, needle12, 13 );

	return 0;
	}
