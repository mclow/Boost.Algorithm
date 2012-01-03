/* 
   Copyright (c) Marshall Clow 2011-2012.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <boost/config.hpp>
#include <boost/algorithm/minmax_element.hpp>
#include <boost/test/included/test_exec_monitor.hpp>

#include <string>
#include <iostream>
#include <vector>

namespace ba = boost::algorithm;
// namespace ba = boost;

template <typename Container>
void test_sequence_no_comparison ( Container const &v, 
	typename Container::const_iterator minI, 
	typename Container::const_iterator maxI ) {

	typedef std::pair <typename Container::const_iterator, typename Container::const_iterator> Pair;

	Pair res = ba::minmax_element ( v.begin (), v.end ());
//	std::cout << "Size = " << v.size () << std::endl;
//	std::cout << " Expected: { " << minI - v.begin () << ", " << maxI - v.begin () << " }" << std::endl;
//	std::cout << " Got     : { " << res.first - v.begin () << ", " << res.second - v.begin () << " }" << std::endl;
	BOOST_CHECK ( res.first == minI );
	BOOST_CHECK ( res.second == maxI );
	}

template <typename Container, typename Comp >
void test_sequence_with_comparison ( Container const &v, Comp comp, 
	typename Container::const_iterator minI, 
	typename Container::const_iterator maxI ) {

	typedef std::pair <typename Container::const_iterator, typename Container::const_iterator> Pair;

	Pair res = ba::minmax_element ( v.begin (), v.end (), comp );
//	std::cout << "Size = " << v.size () << std::endl;
//	std::cout << " Expected: { " << minI - v.begin () << ", " << maxI - v.begin () << " }" << std::endl;
//	std::cout << " Got     : { " << res.first - v.begin () << ", " << res.second - v.begin () << " }" << std::endl;
	BOOST_CHECK ( res.first == minI );
	BOOST_CHECK ( res.second == maxI );
	}


void test_sequence1 () {
	std::vector<int> v;

	v.clear ();
	test_sequence_no_comparison   ( v,                       v.end (), v.end ());	// No elements
	test_sequence_with_comparison ( v, std::greater<int> (), v.end (), v.end ());
	
	v.push_back ( 5 );
	test_sequence_no_comparison   ( v,                       v.begin (), v.end () - 1 );	// one element
	test_sequence_with_comparison ( v, std::greater<int> (), v.begin (), v.end () - 1 );
	v.push_back ( 5 );
	
	test_sequence_no_comparison   ( v,                       v.begin (), v.end () - 1 );	// two elements
	test_sequence_with_comparison ( v, std::greater<int> (), v.begin (), v.end () - 1 );
		
	v.clear ();
	for ( int i = 5; i < 15; ++i )
		v.push_back ( 10 );
	test_sequence_no_comparison   ( v,                       v.begin (), v.end () - 1 );	// even # of elements
	test_sequence_with_comparison ( v, std::greater<int> (), v.begin (), v.end () - 1 );
	
	v.clear ();
	for ( int i = 5; i < 16; ++i )
		v.push_back ( 9 );
	test_sequence_no_comparison   ( v,                       v.begin (), v.end () - 1 );	// odd # of elements
	test_sequence_with_comparison ( v, std::greater<int> (), v.begin (), v.end () - 1 );
	}


int test_main( int , char* [] )
{
  test_sequence1 ();
  return 0;
}
