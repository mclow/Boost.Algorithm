/* 
   Copyright (c) Marshall Clow 2008-2011.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <iostream>

#include <boost/config.hpp>
#include <boost/algorithm/move.hpp>
#include <boost/test/included/test_exec_monitor.hpp>

#include <string>
#include <vector>
#include <list>

namespace ba = boost::algorithm;

void test_sequence1 () {
	std::vector<int> v, v1;
	
    for ( int i = 5; i < 15; ++i )
        v.push_back ( i );
	ba::move ( v.begin (), v.end (), std::back_inserter ( v1 ));
	BOOST_CHECK ( v1.size () == 10 );
	BOOST_CHECK ( *v1.begin ()  ==  5 );
	BOOST_CHECK ( *v1.rbegin () == 14 );
	
/*	Can't pass back_insert_iterator to move_backward -- it wants to decrement it
	v.clear (); v1.clear ();
    for ( int i = 5; i < 15; ++i )
        v.push_back ( i );
	ba::move_backward ( v.begin (), v.end (), std::back_inserter ( v1 ));
	BOOST_CHECK ( v1.size () == 10 );
	BOOST_CHECK ( *v1.rbegin () ==  5 );
	BOOST_CHECK ( *v1.begin ()  == 14 );
*/
	}
	

int test_main( int , char* [] )
{
  test_sequence1 ();
  return 0;
}
