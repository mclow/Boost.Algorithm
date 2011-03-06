//  Copyright (c) 2010 Nuovation System Designs, LLC
//    Grant Erickson <gerickson@nuovations.com>
//
//	Reworked by Marshall Clow; August 2010
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  See http://www.boost.org/ for latest version.

#include <algorithm>
#include <iostream>

#include <boost/algorithm/ordered.hpp>
#include <boost/test/included/test_exec_monitor.hpp>

using namespace boost;

/* Preprocessor Defines */

#define elementsof(v)	(sizeof (v) / sizeof (v[0]))
#define begin(v)		(&v[0])
#define end(v)			(v + elementsof (v))
#define b_e(v)			begin(v),end(v)
#define	range(v)		v

namespace ba = boost::algorithm;

static void
test_ordered(void)
{
	const int strictlyIncreasingValues[] = { 1, 2, 3, 4, 5 };
	const int strictlyDecreasingValues[] = { 9, 8, 7, 6, 5 };
	const int increasingValues[] = { 1, 2, 2, 2, 5 };
	const int decreasingValues[] = { 9, 7, 7, 7, 5 };
	const int randomValues[] = { 3, 6, 1, 2, 7 };
	const int constantValues[] = { 7, 7, 7, 7, 7 };

	// Test a strictly increasing sequence
	BOOST_CHECK (  ba::is_strictly_increasing (b_e(strictlyIncreasingValues)));
	BOOST_CHECK (  ba::is_increasing          (b_e(strictlyIncreasingValues)));
	BOOST_CHECK ( !ba::is_strictly_decreasing (b_e(strictlyIncreasingValues)));
	BOOST_CHECK ( !ba::is_decreasing          (b_e(strictlyIncreasingValues)));

//	BOOST_CHECK (  ba::is_strictly_increasing (range(strictlyIncreasingValues)));
//	BOOST_CHECK (  ba::is_increasing          (range(strictlyIncreasingValues)));
//	BOOST_CHECK ( !ba::is_strictly_decreasing (range(strictlyIncreasingValues)));
//	BOOST_CHECK ( !ba::is_decreasing          (range(strictlyIncreasingValues)));

	// Test a strictly decreasing sequence
	BOOST_CHECK ( !ba::is_strictly_increasing (b_e(strictlyDecreasingValues)));
	BOOST_CHECK ( !ba::is_increasing          (b_e(strictlyDecreasingValues)));
	BOOST_CHECK (  ba::is_strictly_decreasing (b_e(strictlyDecreasingValues)));
	BOOST_CHECK (  ba::is_decreasing          (b_e(strictlyDecreasingValues)));

	// Test an increasing sequence
	BOOST_CHECK ( !ba::is_strictly_increasing (b_e(increasingValues)));
	BOOST_CHECK (  ba::is_increasing          (b_e(increasingValues)));
	BOOST_CHECK ( !ba::is_strictly_decreasing (b_e(increasingValues)));
	BOOST_CHECK ( !ba::is_decreasing          (b_e(increasingValues)));

	// Test a decreasing sequence
	BOOST_CHECK ( !ba::is_strictly_increasing (b_e(decreasingValues)));
	BOOST_CHECK ( !ba::is_increasing          (b_e(decreasingValues)));
	BOOST_CHECK ( !ba::is_strictly_decreasing (b_e(decreasingValues)));
	BOOST_CHECK (  ba::is_decreasing          (b_e(decreasingValues)));

	// Test a random sequence
	BOOST_CHECK ( !ba::is_strictly_increasing (b_e(randomValues)));
	BOOST_CHECK ( !ba::is_increasing          (b_e(randomValues)));
	BOOST_CHECK ( !ba::is_strictly_decreasing (b_e(randomValues)));
	BOOST_CHECK ( !ba::is_decreasing          (b_e(randomValues)));

	// Test a constant sequence
	BOOST_CHECK ( !ba::is_strictly_increasing (b_e(constantValues)));
	BOOST_CHECK (  ba::is_increasing          (b_e(constantValues)));
	BOOST_CHECK ( !ba::is_strictly_decreasing (b_e(constantValues)));
	BOOST_CHECK (  ba::is_decreasing          (b_e(constantValues)));

}

int test_main( int, char * [] )
{
	test_ordered ();

	return 0;
}
