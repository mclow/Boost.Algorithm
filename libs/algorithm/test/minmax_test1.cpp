/* 
   Copyright (c) Marshall Clow 2008-2011.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <boost/config.hpp>
#include <boost/algorithm/minmax.hpp>
#include <boost/test/included/test_exec_monitor.hpp>

#include <string>
#include <iostream>
#include <vector>

namespace ba = boost::algorithm;
// namespace ba = boost;

#if 0
void test_one () {
	std::pair<const int &, const int &> res1 = ba::minmax ( 5, 15 );
	std::pair<const int &, const int &> res2 = ba::minmax ( 15, 5 );
	BOOST_CHECK ( res1 == res2 );
	}
#endif

template <typename Container>
void test_sequence_no_comparison ( Container &v, typename Container::value_type minE, typename Container::value_type maxE ) {

	typedef std::pair <typename Container::iterator, typename Container::iterator> Pair;

	Pair res = ba::minmax_element ( v.begin (), v.end ());
//	std::cout << "Res1: { " << *res.first << ", " << *res.second << " }" << std::endl;
	BOOST_CHECK ( *res.first == minE );
	BOOST_CHECK ( *res.second == maxE );

	std::reverse ( v.begin (), v.end ());
	res = ba::minmax_element ( v.begin (), v.end ());
//	std::cout << "Res2: { " << *res.first << ", " << *res.second << " }" << std::endl;
	BOOST_CHECK ( *res.first == minE );
	BOOST_CHECK ( *res.second == maxE );
	
	std::random_shuffle ( v.begin (), v.end ());
	res = ba::minmax_element ( v.begin (), v.end ());
//	std::cout << "Res2: { " << *res.first << ", " << *res.second << " }" << std::endl;
	BOOST_CHECK ( *res.first == minE );
	BOOST_CHECK ( *res.second == maxE );
	
//	One element
	int v0 = v[0];
	res = ba::minmax_element ( v.begin (), v.begin () + 1 );
	BOOST_CHECK ( *res.first == v0 );
	BOOST_CHECK ( *res.second == v0 );
	
//	Two elements
	int v1 = v[1];
	res = ba::minmax_element ( v.begin (), v.begin () + 2 );
	BOOST_CHECK ( *res.first  == std::min ( v0, v1 ));
	BOOST_CHECK ( *res.second == std::max ( v0, v1 ));
	}


void test_sequence1 () {
	std::vector<int> v;
	
	v.clear ();
	for ( int i = 5; i < 15; ++i )
		v.push_back ( i );
	test_sequence_no_comparison ( v, 5, 14 );	// even # of elements
	
	v.clear ();
	for ( int i = 5; i < 16; ++i )
		v.push_back ( i );
	test_sequence_no_comparison ( v, 5, 15 );	// odd # of elements

	}


template <typename Container, typename Comp >
void test_sequence_with_comparison ( Container &v, Comp comp, typename Container::value_type minE, typename Container::value_type maxE ) {

	typedef std::pair <typename Container::iterator, typename Container::iterator> Pair;

	Pair res = ba::minmax_element ( v.begin (), v.end (), comp );
	BOOST_CHECK ( *res.first == minE );
	BOOST_CHECK ( *res.second == maxE );

	std::reverse ( v.begin (), v.end ());
	res = ba::minmax_element ( v.begin (), v.end (), comp );
	BOOST_CHECK ( *res.first == minE );
	BOOST_CHECK ( *res.second == maxE );
	
	std::random_shuffle ( v.begin (), v.end ());
	res = ba::minmax_element ( v.begin (), v.end (), comp );
	BOOST_CHECK ( *res.first == minE );
	BOOST_CHECK ( *res.second == maxE );
	
//	One element
	int v0 = v[0];
	res = ba::minmax_element ( v.begin (), v.begin () + 1, comp );
	BOOST_CHECK ( *res.first == v0 );
	BOOST_CHECK ( *res.second == v0 );
	
//	Two elements
	int v1 = v[1];
	res = ba::minmax_element ( v.begin (), v.begin () + 2, comp );
	if ( comp ( v0, v1 )) {
		BOOST_CHECK ( *res.first  == v0 );
		BOOST_CHECK ( *res.second == v1 );
		}
	else {
		BOOST_CHECK ( *res.first  == v1 );
		BOOST_CHECK ( *res.second == v0 );
		}
	
	}

void test_sequence2 () {
	std::vector<int> v;
	
	v.clear ();
	for ( int i = 5; i < 15; ++i )
		v.push_back ( i );
	test_sequence_with_comparison ( v, std::greater<int> (), 14, 5 );	// even # of elements
	
	v.clear ();
	for ( int i = 5; i < 16; ++i )
		v.push_back ( i );
	test_sequence_with_comparison ( v, std::greater<int> (), 15, 5 );	// odd # of elements
	}

struct counting_less {
public:
	counting_less ( int &foo ) : ctr ( foo ) { ctr = 0; }
	counting_less ( const counting_less &rhs ) : ctr ( rhs.ctr ) {}

	template <typename T>
	bool operator () ( const T &a, const T&b ) const { ++ctr; return a < b; }
private:
	counting_less operator = ( const counting_less &rhs );
	int & ctr;
	};

	
int max_comparisons ( int distance ) { return std::max ( 0, 3 * ( distance - 1 ) / 2 ); }
	
void test_sequence3 () {
	std::vector<int> v;
	typedef std::pair <std::vector<int>::iterator, std::vector<int>::iterator> Pair;
	
	Pair res;
	int counter;
	int max_possible;
	
	for ( int i = 5; i < 25; ++i )
		v.push_back ( i );

	for ( std::vector<int>::iterator iter = v.begin (); iter != v.end (); ++iter ) {
		std::random_shuffle ( v.begin (), v.end ());
		res = ba::minmax_element ( iter, v.end (), counting_less ( counter ));
		BOOST_CHECK ( *res.first <= *res.second );
		max_possible = max_comparisons ( std::distance ( iter, v.end ()));
		std::cout << "Distance, max comparisons, actual: " << std::distance ( iter, v.end ()) 
			<< " " << max_possible << " " << counter << std::endl;
		BOOST_CHECK ( counter <= max_possible );
		
		}
	}


int test_main( int , char* [] )
{
//  test_one ();
  test_sequence1 ();
  test_sequence2 ();
  test_sequence3 ();
  return 0;
}
