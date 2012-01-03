/* 
   Copyright (c) Marshall Clow 2011-2012.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <iostream>
#include <algorithm>
#include <string>

#include <boost/algorithm/hex.hpp>

#define	kNumTests	100000

namespace test0 {
//	No work; just the loop
	unsigned long tohex ( const char *source ) {
		std::clock_t b_time = std::clock ();
		for ( int i = 0; i < kNumTests; ++i ) {
			}
		return std::clock () - b_time;
		}

	}

namespace test1 {
//	C version
//	No memory allocation - just write the data into a buffer
	unsigned long tohex ( const char *source ) {
		char output [ 1000 ];
		const char *ptr;
		char *out;
		std::clock_t b_time = std::clock ();
		
		for ( int i = 0; i < kNumTests; ++i ) {
			ptr = source;
			out = output;
			while ( *ptr ) {
				*out++ = "0123456789ABCDEF" [ ( *ptr >> 4 ) & 0x0F ];
				*out++ = "0123456789ABCDEF" [ ( *ptr      ) & 0x0F ];
				ptr++;
				}
			}
//		*out = 0;
//		std::cout << "Test1 got: " << &output[0] << std::endl;
		return std::clock () - b_time;
		}

	}
	
namespace test2 {
//	No memory allocation - just write the data into a buffer
	unsigned long tohex ( const char *source ) {
		char output [ 1000 ];
		std::clock_t b_time = std::clock ();
		for ( int i = 0; i < kNumTests; ++i ) {
			boost::algorithm::hex ( source, output );
			}
//		output [ 2 * std::strlen ( source ) ] = '\0';
//		std::cout << "Test2 got: " << &output[0] << std::endl;
		return std::clock () - b_time;
		}
	}
	
namespace test3 {
//	Should be slowest; has (posible) memory allocation each time through the loop.
	unsigned long tohex ( const char *source ) {
		std::clock_t b_time = std::clock ();
		std::string result;
		std::back_insert_iterator<std::string> back = std::back_inserter ( result );

		for ( int i = 0; i < kNumTests; ++i ) {
			result.clear ();
			boost::algorithm::hex ( source, back );
			}
	//	std::cout << "Test3 got: " << result << std::endl;			
	//	std::cout << "Test3 got: " << result.size () << std::endl;			
		return std::clock () - b_time;
		}
	}

#define	kShortTest		"abcde"
#define	kMediumTest		"asdkfjafadsmfadmfa13413241vdadsfa"
#define	kLongTest		"asdlfkjaf23418q49qfakljfa;kldfjklaefjlaldk;sfal;sdjfl;ajsdfl;ajlsdfajlsdflk;adkls;fkljaewlr;a"


int main ( int argc, char *argv [] ) {
	unsigned long zero, one, two, three;
	std::cout << "There are " << CLOCKS_PER_SEC << " ticks per second" << std::endl;
	zero  = test0::tohex ( kShortTest );
	std::cout << "Empty tests took " << ((double) zero)   / CLOCKS_PER_SEC << " seconds;\t" << zero  << " ticks" << std::endl;

	std::cout << "Running tohex short tests " << kNumTests << " times" << std::endl;
	one   = test1::tohex ( kShortTest );
	two   = test2::tohex ( kShortTest );
	three = test3::tohex ( kShortTest );
	std::cout << "Test1 took " << ((double) one)   / CLOCKS_PER_SEC << " seconds;\t" << one   << " ticks" << std::endl;
	std::cout << "Test2 took " << ((double) two)   / CLOCKS_PER_SEC << " seconds;\t" << two   << " ticks" << std::endl;
	std::cout << "Test3 took " << ((double) three) / CLOCKS_PER_SEC << " seconds;\t" << three << " ticks" << std::endl;

	std::cout << "Running tohex medium tests " << kNumTests << " times" << std::endl;
	one   = test1::tohex ( kMediumTest );
	two   = test2::tohex ( kMediumTest );
	three = test3::tohex ( kMediumTest );
	std::cout << "Test1 took " << ((double) one)   / CLOCKS_PER_SEC << " seconds;\t" << one   << " ticks" << std::endl;
	std::cout << "Test2 took " << ((double) two)   / CLOCKS_PER_SEC << " seconds;\t" << two   << " ticks" << std::endl;
	std::cout << "Test3 took " << ((double) three) / CLOCKS_PER_SEC << " seconds;\t" << three << " ticks" << std::endl;

	std::cout << "Running tohex long tests " << kNumTests << " times" << std::endl;
	one   = test1::tohex ( kLongTest );
	two   = test2::tohex ( kLongTest );
	three = test3::tohex ( kLongTest );
	std::cout << "Test1 took " << ((double) one)   / CLOCKS_PER_SEC << " seconds;\t" << one   << " ticks" << std::endl;
	std::cout << "Test2 took " << ((double) two)   / CLOCKS_PER_SEC << " seconds;\t" << two   << " ticks" << std::endl;
	std::cout << "Test3 took " << ((double) three) / CLOCKS_PER_SEC << " seconds;\t" << three << " ticks" << std::endl;

	return 0;
	}
