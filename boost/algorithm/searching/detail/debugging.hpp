/*
    Copyright Marshall Clow 2010-2011. Use, modification and
    distribution is subject to the Boost Software License, Version
    1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)

    For more information, see http://www.boost.org
*/

#ifndef BOOST_ALGORITHM_SEARCH_DETAIL_DEBUG_HPP
#define BOOST_ALGORITHM_SEARCH_DETAIL_DEBUG_HPP

#include <iostream>

namespace boost { namespace algorithm { namespace detail {

//  Debugging support
    template <typename Iter>
    void PrintTable ( Iter first, Iter last ) {
        std::cout << std::distance ( first, last ) << ": { ";
        for ( Iter iter = first; iter != last; ++iter )
            std::cout << *iter << " ";
        std::cout << "}" << std::endl;
        }
    
}}}

#endif	//	BOOST_ALGORITHM_SEARCH_DETAIL_DEBUG_HPP
