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


const wchar_t *tohex [] = {
    L"",
    L"a",
    L"\001",
    L"12",
    L"asdfadsfsad",
    L"01234567890ABCDEF",
    NULL        // End of the list
    };

void test_to_hex () {
    for ( const wchar_t **p = tohex; *p; p++ ) {
        std::wstring arg, argh, one, two, three;
        arg.assign ( *p );
        boost::algorithm::hex ( *p, std::back_inserter ( one ));
        boost::algorithm::hex ( arg, std::back_inserter ( two ));
        boost::algorithm::hex ( arg.begin (), arg.end (), std::back_inserter ( three ));
        BOOST_CHECK ( one == two );
        BOOST_CHECK ( two == three );
        argh = one;
        one.clear (); two.clear (); three.clear ();
        boost::algorithm::unhex ( argh.c_str (), std::back_inserter ( one ));
        boost::algorithm::unhex ( argh, std::back_inserter ( two ));
        boost::algorithm::unhex ( argh.begin (), argh.end (), std::back_inserter ( three ));
        BOOST_CHECK ( one == two );
        BOOST_CHECK ( two == three );
        BOOST_CHECK ( one == arg );
        }
    }

const wchar_t *fromhex [] = {
    L"00000020",
    L"2122234556FF3456",
    NULL        // End of the list
    };


void test_from_hex_success () {
    for ( const wchar_t **p = fromhex; *p; p++ ) {
        std::wstring arg, argh, one, two, three;
        arg.assign ( *p );
        boost::algorithm::unhex ( *p, std::back_inserter ( one ));
        boost::algorithm::unhex ( arg, std::back_inserter ( two ));
        boost::algorithm::unhex ( arg.begin (), arg.end (), std::back_inserter ( three ));
        BOOST_CHECK ( one == two );
        BOOST_CHECK ( two == three );
        argh = one;
        one.clear (); two.clear (); three.clear ();
        boost::algorithm::hex ( argh.c_str (), std::back_inserter ( one ));
        boost::algorithm::hex ( argh, std::back_inserter ( two ));
        boost::algorithm::hex ( argh.begin (), argh.end (), std::back_inserter ( three ));
        BOOST_CHECK ( one == two );
        BOOST_CHECK ( two == three );
        BOOST_CHECK ( one == arg );
        }
    }


const wchar_t *fromhex_fail [] = {
    L"2",
    L"12",
    L"H",
    L"234",
    L"21222G4556FF",
    NULL        // End of the list
    };


void test_from_hex_failure () {
    int num_catches;
    for ( const wchar_t **p = fromhex_fail; *p; p++ ) {
        std::wstring arg, one;
        arg.assign ( *p );
        num_catches = 0;

        try { boost::algorithm::unhex ( *p, std::back_inserter ( one )); }
        catch ( const std::runtime_error &ex ) { num_catches++; }
        try { boost::algorithm::unhex ( arg, std::back_inserter ( one )); }
        catch ( const std::runtime_error &ex ) { num_catches++; }
        try { boost::algorithm::unhex ( arg.begin (), arg.end (), std::back_inserter ( one )); }
        catch ( const std::runtime_error &ex ) { num_catches++; }
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
