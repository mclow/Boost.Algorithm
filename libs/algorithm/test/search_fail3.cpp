/* 
   Copyright (c) Marshall Clow 2008-2010.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <vector>
#include <boost/algorithm/searching/boyer_moore.hpp>

int main( int argc, char *argv [] )
{
//  Should fail to compile because the search objects are not default-constructible
    boost::algorithm::boyer_moore<std::vector<char>::iterator> bm;
   
   (void) argv; (void) argc;
   return 0;
}
