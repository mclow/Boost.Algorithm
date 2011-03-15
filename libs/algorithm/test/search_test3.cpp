/*

*/

#include <boost/algorithm/search.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

#include <iostream>
#include <algorithm>
#include <vector>
#include <string>

typedef	std::vector<std::string> vec;
#define	NUM_TRIES	100

#define	runOne(call, refDiff)	{ \
	std::clock_t bTime, eTime;								\
	bTime = std::clock ();									\
	for ( i = 0; i < NUM_TRIES; ++i ) {						\
		res = boost::algorithm::call 						\
			( haystack.begin (), haystack.end (), 			\
						needle.begin (), needle.end ());	\
		if ( res != exp ) {									\
			std::cout << "On run # " << i << " expected " 	\
				<< exp - haystack.begin () << " got " 		\
				<< res - haystack.begin () << std::endl;	\
			throw std::runtime_error 						\
				( "Unexpected result from " #call );		\
			}												\
		}													\
	eTime = std::clock ();									\
	printRes ( #call, eTime - bTime, refDiff ); }

#define	runOneP(call, refDiff)	{ \
	std::clock_t bTime, eTime;								\
	bTime = std::clock ();									\
	for ( i = 0; i < NUM_TRIES; ++i ) {						\
		res = boost::algorithm::call 						\
			( haystack.begin (), haystack.end (), 			\
				needle.begin (), needle.end (), Equal );	\
		if ( res != exp ) {									\
			std::cout << "On run # " << i << " expected " 	\
				<< exp - haystack.begin () << " got " 		\
				<< res - haystack.begin () << std::endl;	\
			throw std::runtime_error 						\
				( "Unexpected result from " #call " (p)" );	\
			}												\
		}													\
	eTime = std::clock ();									\
	printRes ( #call " (p)", eTime - bTime, refDiff ); }
	
#define runObject(obj, refDiff)	{ \
	std::clock_t bTime, eTime;								\
	bTime = std::clock ();									\
	boost::algorithm::obj <vec::const_iterator> 			\
				s_o ( needle.begin (), needle.end ());		\
	for ( i = 0; i < NUM_TRIES; ++i ) {						\
		res = s_o ( haystack.begin (), haystack.end ());	\
		if ( res != exp ) {									\
			std::cout << "On run # " << i << " expected " 	\
			<< exp - haystack.begin () << " got " 			\
			<< res - haystack.begin () << std::endl;		\
			throw std::runtime_error 						\
			( "Unexpected result from " #obj " object" );	\
			}												\
		}													\
	eTime = std::clock ();									\
	printRes ( #obj " object", eTime - bTime, refDiff ); }
	
#define runObjectP(obj, refDiff)	{ \
	std::clock_t bTime, eTime;								\
	bTime = std::clock ();									\
	boost::algorithm::obj <vec::const_iterator> 			\
				s_o ( needle.begin (), needle.end ());		\
	for ( i = 0; i < NUM_TRIES; ++i ) {						\
		res = s_o ( haystack.begin (), haystack.end (), Equal );	\
		if ( res != exp ) {									\
			std::cout << "On run # " << i << " expected " 	\
			<< exp - haystack.begin () << " got " 			\
			<< res - haystack.begin () << std::endl;		\
			throw std::runtime_error 						\
			( "Unexpected result from " #obj " object (p)" );	\
			}												\
		}													\
	eTime = std::clock ();									\
	printRes ( #obj " object (p)", eTime - bTime, refDiff ); }

namespace {

	vec ReadFromFile ( const char *name ) {
		std::ifstream in ( name, std::ios_base::binary | std::ios_base::in );
		std::string temp;
		vec retVal;
		while ( std::getline ( in, temp ))
			retVal.push_back ( temp );
		
		return retVal;
		}
	
//	A simple predicate for testing the predicate versions
	bool Equal ( const std::string &ch1, const std::string &ch2 ) {
		if ( ch1.length () == ch2.length ())
			return std::equal ( ch1.begin (), ch1.end (), ch2.begin ());
		return false;
		}
	
	void printRes ( const char *prompt, float diff, float stdDiff ) {
		std::cout 
			<< std::setw(40) << prompt << " "
			<< std::setw(6)  << diff / CLOCKS_PER_SEC << " seconds\t"
			<< std::setw(5)  << (diff / stdDiff) * 100 << "% " 
			<< std::setw(12) << diff;
		if ( diff > stdDiff ) 
			std::cout << " !!";
		std::cout << std::endl;
		}
	
	void check_one ( const vec &haystack, const vec &needle, int expected ) {
		std::size_t i;
		std::clock_t sTime;
		float stdDiff, stdPDiff;
		
		vec::const_iterator res;
		vec::const_iterator exp;		// the expected result
		
		if ( expected >= 0 )
			exp = haystack.begin () + expected;
		else if ( expected == -1 )
			exp = haystack.end ();		// we didn't find it1
		else if ( expected == -2 )
			exp = std::search ( haystack.begin (), haystack.end (), needle.begin (), needle.end ());
		else	
			throw std::logic_error ( "Expected must be -2, -1, or >= 0" );

		std::cout << "Pattern is " << needle.size ()   << " entries long" << std::endl;
		std::cout << "Corpus  is " << haystack.size () << " entries long" << std::endl;

	//	First, the std library search
		sTime = std::clock ();
		for ( i = 0; i < NUM_TRIES; ++i ) {
			res = std::search ( haystack.begin (), haystack.end (), needle.begin (), needle.end ());
			if ( res != exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from std::search" );
				}
			}
		stdDiff = std::clock () - sTime;
		printRes ( "std::search", stdDiff, stdDiff );

		runOne    ( boyer_moore_search,          stdDiff );
		runObject ( boyer_moore,                 stdDiff );
		runOne    ( boyer_moore_horspool_search, stdDiff );
		runObject ( boyer_moore_horspool,        stdDiff );
		
		sTime = std::clock ();
		for ( i = 0; i < NUM_TRIES; ++i ) {
			res = std::search ( haystack.begin (), haystack.end (), needle.begin (), needle.end (), Equal );
			if ( res != exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from std::search (p)" );
				}
			}
		stdPDiff = std::clock () - sTime;
		printRes ( "std::search (p)", stdPDiff, stdPDiff );

		runOneP    ( boyer_moore_search,          stdPDiff );
		runObjectP ( boyer_moore,                 stdPDiff );
		runOneP    ( boyer_moore_horspool_search, stdPDiff );
		runObjectP ( boyer_moore_horspool,        stdPDiff );
		runOneP    ( knuth_morris_pratt_search,   stdPDiff );
		runObjectP ( knuth_morris_pratt,          stdPDiff );
		}
	}

int test_main( int , char* [] )
{
	vec c1  = ReadFromFile ( "data-files/0001.corpus" );
	vec p1b = ReadFromFile ( "data-files/0002b.pat" );
	vec p1e = ReadFromFile ( "data-files/0002e.pat" );
	vec p1n = ReadFromFile ( "data-files/0002n.pat" );
	vec p1f = ReadFromFile ( "data-files/0002f.pat" );

	std::cout << std::ios::fixed << std::setprecision(4);
//	std::cout << "Corpus is " << c1.size () << " entries long\n";
	std::cout << "--- Beginning ---" << std::endl;
	check_one ( c1, p1b, 0 );		//	Find it at position zero
	std::cout << "---- Middle -----" << std::endl;
	check_one ( c1, p1f, -2 );		//	Don't know answer
	std::cout << "------ End ------" << std::endl;
	check_one ( c1, p1e, c1.size() - p1e.size ());	
	std::cout << "--- Not found ---" << std::endl;
	check_one ( c1, p1n, -1 );		//	Not found
	
	return 0;
	}
