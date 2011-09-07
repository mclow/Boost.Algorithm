/*
    Copyright Marshall Clow 2010-2011. Use, modification and
    distribution is subject to the Boost Software License, Version
    1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <iostream>		// for cout, etc

#include <boost/algorithm/all.hpp>

const int one []   = { 1, 2, 2, 2, 5 };

#define elementsof(v)   (sizeof (v) / sizeof (v[0]))

template <int x>
bool less_than_x ( int val ) { return val < x; }

bool is_even ( int val ) { return val % 2 == 0; }


namespace ba = boost::algorithm;

int main ( int /*argc*/, char */*argv*/ [] ) {
//	The routines in all.hpp let you test a sequence of values to see if they satisfy a property
	if ( ba::all_of ( one, less_than_x<10> ))
		std::cout << "They're all less than 10" << std::endl;
	else
		std::cout << "They're NOT all less than 10" << std::endl;

//	They work with both iterators and ranges
	if ( ba::all_of ( one, one + elementsof(one), less_than_x<5> ))
		std::cout << "They're all less than 5" << std::endl;
	else
		std::cout << "They're NOT all less than 5" << std::endl;

//	There's also "any_of", which returns true if the condition is true for
//	any of the elements in a sequence.
	if ( ba::any_of ( one, one + elementsof(one), is_even ))
		std::cout << "At least one of them is even (1)" << std::endl;
	else
		std::cout << "None of them are even (1)" << std::endl;
	
	if ( ba::none_of ( one, one + elementsof(one), is_even ))
		std::cout << "None of them are even (2)" << std::endl;
	else
		std::cout << "At least one of them is even (2)" << std::endl;

//	Besides the predicate-based tests, there are value based tests.
	if ( ba::all_of_val ( one, one + elementsof(one), 2 ))
		std::cout << "All your values are two" << std::endl;
	else
		std::cout << "NOT all your values are two" << std::endl;

//	And, of course, you don't have to test the entire container
	if ( ba::all_of_val ( one + 1, one + 4, 2 ))
		std::cout << "All your values are two" << std::endl;
	else
		std::cout << "NOT all your values are two" << std::endl;
		
	return 0;
	}