/*
    Copyright Marshall Clow 2010-2011. Use, modification and
    distribution is subject to the Boost Software License, Version
    1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <iostream>		// for cout, etc

#include <boost/algorithm/clamp.hpp>

namespace ba = boost::algorithm;

bool compare_string_lengths ( const std::string &one, const std::string &two )
{
	return one.length () < two.length ();
}

int main ( int /*argc*/, char */*argv*/ [] ) {
//	Clamp takes a value and two "fenceposts", and brings the value "between" the fenceposts.

//	If the input value is "between" the fenceposts, then it is returned unchanged.
	std::cout << "Clamping   5 to between [1, 10] -> " << ba::clamp ( 5, 1, 10 ) << std::endl;

//	If the input value is out side the range of the fenceposts, it "brought into" range.
	std::cout << "Clamping  15 to between [1, 10] -> " << ba::clamp (  15, 1, 10 ) << std::endl;
	std::cout << "Clamping -15 to between [1, 10] -> " << ba::clamp ( -15, 1, 10 ) << std::endl;

//	It doesn't just work for ints
	std::cout << "Clamping 5.1 to between [1, 10] -> " << ba::clamp ( 5.1, 1.0, 10.0 ) << std::endl;

	{
		std::string one ( "Lower Bound" ), two ( "upper bound!" ), test1 ( "test#" ), test2 ( "#test" );
		std::cout << "Clamping '" << test1 << "' between ['" << one << "' and '" << two << "'] -> '" << 
			ba::clamp ( test1, one, two ) << "'" << std::endl;
		std::cout << "Clamping '" << test2 << "' between ['" << one << "' and '" << two << "'] -> '" << 
			ba::clamp ( test2, one, two ) << "'" << std::endl;
	//	There is also a predicate based version, if you want to compare objects in your own way
		std::cout << "Clamping '" << test1 << "' between ['" << one << "' and '" << two << "'] (comparing lengths) -> '" << 
			ba::clamp ( test1, one, two, compare_string_lengths ) << "'" << std::endl;
		std::cout << "Clamping '" << test2 << "' between ['" << one << "' and '" << two << "'] (comparing lengths) -> '" << 
			ba::clamp ( test2, one, two, compare_string_lengths ) << "'" << std::endl;
	
	}

	return 0;
	}