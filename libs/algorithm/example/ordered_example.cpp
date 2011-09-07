/*
    Copyright Marshall Clow 2010-2011. Use, modification and
    distribution is subject to the Boost Software License, Version
    1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#include <list>         //  for std::list
#include <iterator>     //  for std::back_inserter
#include <iostream>     //  for cout, etc.

#include <boost/algorithm/ordered.hpp>


#define elementsof(v)   (sizeof (v) / sizeof (v[0]))

namespace ba = boost::algorithm;

const int one []   = { 1, 2, 2, 2, 5 };

int main ( int /*argc*/, char */*argv*/ [] ) {
//  The routines in ordered.hpp let you test a sequence of values
//  to see if they are "ordered"

    if ( ba::is_increasing ( one ))
        std::cout << "The elements of 'one' are increasing (1)" << std::endl;
    else
        std::cout << "The elements of 'one' are NOT increasing (1)" << std::endl;

//  There are both range and iterator-based versions
    if ( ba::is_increasing ( one, one + elementsof(one)))
        std::cout << "The elements of 'one' are increasing (2)" << std::endl;
    else
        std::cout << "The elements of 'one' are NOT increasing (2)" << std::endl;
        
//  They work with STL containers, too.
//  And not just with random-access iterators
    std::list<int> list_one;
    std::copy ( one, one + elementsof (one), std::back_inserter ( list_one ));
    if ( ba::is_increasing ( list_one.begin (), list_one.end ()))
        std::cout << "The elements of 'one' are increasing (3)" << std::endl;
    else
        std::cout << "The elements of 'one' are NOT increasing (3)" << std::endl;

//  There's also a predicate-based version that returns the first out-of-order element
    if ( ba::is_ordered ( list_one.begin (), list_one.end (), std::less_equal<int>()) == list_one.end ())
        std::cout << "The elements of 'one' are increasing (4)" << std::endl;
    else
        std::cout << "The elements of 'one' are NOT increasing (4)" << std::endl;

//  There are other calls as well. is_strictly_increasing returns true iff each item
//  in the sequence is greater than the one before.
    if ( ba::is_strictly_increasing ( one, one + elementsof(one)))
        std::cout << "The elements of 'one' are strictly increasing (5)" << std::endl;
    else
        std::cout << "The elements of 'one' are NOT strictly increasing (5)" << std::endl;

    return 0;
    }
