/* 
   Copyright (c) Marshall Clow 2008-2011.

   Distributed under the Boost Software License, Version 1.0. (See accompanying
   file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
*/

#include <boost/config.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/test/included/test_exec_monitor.hpp>

#include <string>
#include <iostream>


const char *tohex [] = {
    "",
    "a",
    "\001",
    "12",
    "asdfadsfsad",
    "01234567890ABCDEF",
    NULL        // End of the list
    };

void test_to_hex () {
    for ( const char **p = tohex; *p; p++ ) {
        std::string arg, argh, one, two, three, four;
        arg.assign ( *p );
        boost::algorithm::hex ( *p, std::back_inserter ( one ));
        boost::algorithm::hex ( arg, std::back_inserter ( two ));
        boost::algorithm::hex ( arg.begin (), arg.end (), std::back_inserter ( three ));
        four = boost::algorithm::hex ( arg );
        BOOST_CHECK ( one == two );
        BOOST_CHECK ( one == three );
        BOOST_CHECK ( one == four );
        argh = one;
        one.clear (); two.clear (); three.clear (); four.clear ();
        boost::algorithm::unhex ( argh.c_str (), std::back_inserter ( one ));
        boost::algorithm::unhex ( argh, std::back_inserter ( two ));
        boost::algorithm::unhex ( argh.begin (), argh.end (), std::back_inserter ( three ));
        four = boost::algorithm::unhex ( argh );
        BOOST_CHECK ( one == two );
        BOOST_CHECK ( one == three );
        BOOST_CHECK ( one == four );
        BOOST_CHECK ( one == arg );
        }
    }

const char *fromhex [] = {
    "20",
    "2122234556FF",
    NULL        // End of the list
    };


void test_from_hex_success () {
    for ( const char **p = fromhex; *p; p++ ) {
        std::string arg, argh, one, two, three, four;
        arg.assign ( *p );
        boost::algorithm::unhex ( *p, std::back_inserter ( one ));
        boost::algorithm::unhex ( arg, std::back_inserter ( two ));
        boost::algorithm::unhex ( arg.begin (), arg.end (), std::back_inserter ( three ));
        four = boost::algorithm::unhex ( arg );
        BOOST_CHECK ( one == two );
        BOOST_CHECK ( one == three );
        BOOST_CHECK ( one == four );
        argh = one;
        one.clear (); two.clear (); three.clear (); four.clear ();
        boost::algorithm::hex ( argh.c_str (), std::back_inserter ( one ));
        boost::algorithm::hex ( argh, std::back_inserter ( two ));
        boost::algorithm::hex ( argh.begin (), argh.end (), std::back_inserter ( three ));
        four = boost::algorithm::hex ( argh );
        BOOST_CHECK ( one == two );
        BOOST_CHECK ( one == three );
        BOOST_CHECK ( one == four );
        BOOST_CHECK ( one == arg );
        }
    }


const char *fromhex_fail [] = {
    "2",
    "H",
    "234",
    "21222G4556FF",
    NULL        // End of the list
    };


void test_from_hex_failure () {
    int num_catches;
    for ( const char **p = fromhex_fail; *p; p++ ) {
        std::string arg, one;
        arg.assign ( *p );
        num_catches = 0;

        try { boost::algorithm::unhex ( *p, std::back_inserter ( one )); }
        catch ( const boost::algorithm::hex_decode_error &ex ) { num_catches++; }
        try { boost::algorithm::unhex ( arg, std::back_inserter ( one )); }
        catch ( const boost::algorithm::hex_decode_error &ex ) { num_catches++; }
        try { boost::algorithm::unhex ( arg.begin (), arg.end (), std::back_inserter ( one )); }
        catch ( const boost::algorithm::hex_decode_error &ex ) { num_catches++; }
        BOOST_CHECK ( num_catches == 3 );
        }
    }


int test_main( int , char* [] )
{
  test_to_hex ();
  test_from_hex_success ();
  test_from_hex_failure ();
  return 0;
}
