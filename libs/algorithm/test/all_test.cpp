//  (C) Copyright Jesse Williamson 2009
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/config.hpp>
#include <boost/algorithm/all.hpp>
#include <boost/test/included/test_exec_monitor.hpp>

#include <utility>
#include <iostream>
#include <functional>

// This prettyprinter is useful when debugging:
template <class T>
void show(std::vector<T>& v)
{
 for(typename std::vector<T>::const_iterator i = v.begin(); i != v.end(); ++i)
  std::cout << *i << '\n';

 std::cout << "----------" << std::endl;
}

template<typename T>
struct is_ {
    T val_;
    is_ ( T v ) : val_ ( v ) {}
    ~is_ () {}
    bool operator () ( T comp ) const { return val_ == comp; }
    };

namespace ba = boost::algorithm;

void test_none()
{
//  Note: The literal values here are tested against directly, careful if you change them:
    int some_numbers[] = { 1, 5, 0, 18, 1 };
    std::vector<int> vi(some_numbers, some_numbers + 5);
    
    int some_letters[] = { 'a', 'q', 'n', 'y', 'n' };
    std::vector<char> vc(some_letters, some_letters + 5);
    
    BOOST_CHECK ( ba::none_of_equal ( vi,                                  100 ));
    BOOST_CHECK ( ba::none_of       ( vi,                       is_<int> ( 100 )));
    BOOST_CHECK ( ba::none_of_equal ( vi.begin(),     vi.end(),            100 ));
    BOOST_CHECK ( ba::none_of       ( vi.begin(),     vi.end(), is_<int> ( 100 )));

    BOOST_CHECK (!ba::none_of_equal ( vi,                                    1 ));
    BOOST_CHECK (!ba::none_of       ( vi,                       is_<int> (   1 )));
    BOOST_CHECK (!ba::none_of_equal ( vi.begin(),     vi.end(),              1 ));
    BOOST_CHECK (!ba::none_of       ( vi.begin(),     vi.end(), is_<int> (   1 )));

    BOOST_CHECK ( ba::none_of_equal ( vi.end(),       vi.end(),              0 ));
    BOOST_CHECK ( ba::none_of       ( vi.end(),       vi.end(), is_<int> (   0 )));

//   5 is not in { 0, 18, 1 }, but 1 is
    BOOST_CHECK ( ba::none_of_equal ( vi.begin() + 2, vi.end(),              5 ));
    BOOST_CHECK ( ba::none_of       ( vi.begin() + 2, vi.end(), is_<int> (   5 )));
    BOOST_CHECK (!ba::none_of_equal ( vi.begin() + 2, vi.end(),              1 ));
    BOOST_CHECK (!ba::none_of       ( vi.begin() + 2, vi.end(), is_<int> (   1 )));

//  18 is not in { 1, 5, 0 }, but 5 is
    BOOST_CHECK ( ba::none_of_equal ( vi.begin(),     vi.begin() + 3,            18 ));
    BOOST_CHECK ( ba::none_of       ( vi.begin(),     vi.begin() + 3, is_<int> ( 18 )));
    BOOST_CHECK (!ba::none_of_equal ( vi.begin(),     vi.begin() + 3,             5 ));
    BOOST_CHECK (!ba::none_of       ( vi.begin(),     vi.begin() + 3, is_<int> (  5 )));
    
    BOOST_CHECK ( ba::none_of_equal ( vc,             'z' ));
    BOOST_CHECK ( ba::none_of       ( vc, is_<char> ( 'z' )));

    BOOST_CHECK (!ba::none_of_equal ( vc,             'a' ));
    BOOST_CHECK (!ba::none_of       ( vc, is_<char> ( 'a' )));

    BOOST_CHECK (!ba::none_of_equal ( vc,             'n' ));
    BOOST_CHECK (!ba::none_of       ( vc, is_<char> ( 'n' )));

    BOOST_CHECK ( ba::none_of_equal ( vi.begin(), vi.begin(),   1 ));
    BOOST_CHECK ( ba::none_of_equal ( vc.begin(), vc.begin(), 'a' ));
    BOOST_CHECK ( ba::none_of       ( vi.begin(), vi.begin(), is_<int>  (   1 )));
    BOOST_CHECK ( ba::none_of       ( vc.begin(), vc.begin(), is_<char> ( 'a' )));
}

void test_any ()
{
//  Note: The literal values here are tested against directly, careful if you change them:
    int some_numbers[] = { 1, 5, 0, 18, 10 };
    std::vector<int> vi(some_numbers, some_numbers + 5);
    
    int some_letters[] = { 'a', 'q', 'n', 'y', 'n' };
    std::vector<char> vc(some_letters, some_letters + 5);
    
    BOOST_CHECK ( ba::any_of_equal ( vi,                                   1 ));
    BOOST_CHECK ( ba::any_of       ( vi,                       is_<int> (  1 )));
    BOOST_CHECK ( ba::any_of_equal ( vi.begin(),     vi.end(),             1 ));
    BOOST_CHECK ( ba::any_of       ( vi.begin(),     vi.end(), is_<int> (  1 )));

    BOOST_CHECK (!ba::any_of_equal ( vi,                                   9 ));
    BOOST_CHECK (!ba::any_of       ( vi,                       is_<int> (  9 )));
    BOOST_CHECK (!ba::any_of_equal ( vi.begin(),     vi.end(),             9 ));
    BOOST_CHECK (!ba::any_of       ( vi.begin(),     vi.end(), is_<int> (  9 )));

    BOOST_CHECK ( ba::any_of_equal ( vi,                                  10 ));
    BOOST_CHECK ( ba::any_of       ( vi,                       is_<int> ( 10 )));
    BOOST_CHECK (!ba::any_of_equal ( vi,                                   4 ));
    BOOST_CHECK (!ba::any_of       ( vi,                       is_<int> (  4 )));

    BOOST_CHECK (!ba::any_of_equal ( vi.end(),       vi.end(),            0 ));
    BOOST_CHECK (!ba::any_of       ( vi.end(),       vi.end(), is_<int> ( 0 )));

//   5 is not in { 0, 18, 10 }, but 10 is
    BOOST_CHECK ( ba::any_of_equal ( vi.begin() + 2, vi.end(),            10 ));
    BOOST_CHECK ( ba::any_of       ( vi.begin() + 2, vi.end(), is_<int> ( 10 )));

    BOOST_CHECK (!ba::any_of_equal ( vi.begin() + 2, vi.end(),             5 ));
    BOOST_CHECK (!ba::any_of       ( vi.begin() + 2, vi.end(), is_<int> (  5 )));

//  18 is not in { 1, 5, 0 }, but 5 is
    BOOST_CHECK ( ba::any_of_equal ( vi.begin(), vi.begin() + 3,             5 ));
    BOOST_CHECK ( ba::any_of       ( vi.begin(), vi.begin() + 3, is_<int> (  5 )));

    BOOST_CHECK (!ba::any_of_equal ( vi.begin(), vi.begin() + 3,            18 ));
    BOOST_CHECK (!ba::any_of       ( vi.begin(), vi.begin() + 3, is_<int> ( 18 )));

    BOOST_CHECK ( ba::any_of_equal ( vc,             'q' ));
    BOOST_CHECK ( ba::any_of       ( vc, is_<char> ( 'q' )));

    BOOST_CHECK (!ba::any_of_equal ( vc,             '!' ));
    BOOST_CHECK (!ba::any_of       ( vc, is_<char> ( '!' )));

    BOOST_CHECK ( ba::any_of_equal ( vc,             'n' ));
    BOOST_CHECK ( ba::any_of       ( vc, is_<char> ( 'n' )));

    BOOST_CHECK (!ba::any_of_equal ( vi.begin(), vi.begin(),   1 ));
    BOOST_CHECK (!ba::any_of_equal ( vc.begin(), vc.begin(), 'a' ));
    BOOST_CHECK (!ba::any_of       ( vi.begin(), vi.begin(), is_<int>  (   1 )));
    BOOST_CHECK (!ba::any_of       ( vc.begin(), vc.begin(), is_<char> ( 'a' )));
}


void test_all ()
{
//  Note: The literal values here are tested against directly, careful if you change them:
    int some_numbers[] = { 1, 1, 1, 18, 10 };
    std::vector<int> vi(some_numbers, some_numbers + 5);
    
    int some_letters[] = { 'a', 'q', 'n', 'y', 'n' };
    std::vector<char> vc(some_letters, some_letters + 5);
    
    BOOST_CHECK (!ba::all_of_equal ( vi,                                  1 ));
    BOOST_CHECK (!ba::all_of       ( vi,                       is_<int> ( 1 )));
    BOOST_CHECK (!ba::all_of_equal ( vi.begin(),     vi.end(),            1 ));
    BOOST_CHECK (!ba::all_of       ( vi.begin(),     vi.end(), is_<int> ( 1 )));
    
    BOOST_CHECK (!ba::all_of_equal ( vi,                                  0 ));
    BOOST_CHECK (!ba::all_of       ( vi,                       is_<int> ( 0 )));
    BOOST_CHECK (!ba::all_of_equal ( vi.begin(),     vi.end(),            0 ));
    BOOST_CHECK (!ba::all_of       ( vi.begin(),     vi.end(), is_<int> ( 0 )));

    BOOST_CHECK ( ba::all_of_equal ( vi.end(),       vi.end(),            0 ));
    BOOST_CHECK ( ba::all_of       ( vi.end(),       vi.end(), is_<int> ( 0 )));

    BOOST_CHECK ( ba::all_of_equal ( vi.begin(), vi.begin () + 3,            1 ));
    BOOST_CHECK ( ba::all_of       ( vi.begin(), vi.begin () + 3, is_<int> ( 1 )));
    
    BOOST_CHECK ( ba::all_of_equal ( vc.begin() + 1, vc.begin() + 2,             'q' ));
    BOOST_CHECK ( ba::all_of       ( vc.begin() + 1, vc.begin() + 2, is_<char> ( 'q' )));

    BOOST_CHECK (!ba::all_of_equal ( vc,             '!' ));
    BOOST_CHECK (!ba::all_of       ( vc, is_<char> ( '!' )));

    BOOST_CHECK ( ba::all_of_equal ( vi.begin(), vi.begin(),   1 ));
    BOOST_CHECK ( ba::all_of_equal ( vc.begin(), vc.begin(), 'a' ));
    BOOST_CHECK ( ba::all_of       ( vi.begin(), vi.begin(), is_<int>  (   1 )));
    BOOST_CHECK ( ba::all_of       ( vc.begin(), vc.begin(), is_<char> ( 'a' )));
}


void test_one ()
{
//  Note: The literal values here are tested against directly, careful if you change them:
    int some_numbers[] = { 1, 1, 2, 3, 5 };
    std::vector<int> vi(some_numbers, some_numbers + 5);
    
    int some_letters[] = { 'a', 'q', 'n', 'y', 'n' };
    std::vector<char> vc(some_letters, some_letters + 5);
    
    BOOST_CHECK (!ba::one_of_equal ( vi,                                  1 ));
    BOOST_CHECK (!ba::one_of       ( vi,                       is_<int> ( 1 )));
    BOOST_CHECK (!ba::one_of_equal ( vi.begin(),     vi.end(),            1 ));
    BOOST_CHECK (!ba::one_of       ( vi.begin(),     vi.end(), is_<int> ( 1 )));

    BOOST_CHECK (!ba::one_of_equal ( vi,                                  0 ));
    BOOST_CHECK (!ba::one_of       ( vi,                       is_<int> ( 0 )));
    BOOST_CHECK (!ba::one_of_equal ( vi.begin(),     vi.end(),            0 ));
    BOOST_CHECK (!ba::one_of       ( vi.begin(),     vi.end(), is_<int> ( 0 )));
    
    BOOST_CHECK ( ba::one_of_equal ( vi,                                  2 ));
    BOOST_CHECK ( ba::one_of       ( vi,                       is_<int> ( 2 )));
    BOOST_CHECK ( ba::one_of_equal ( vi.begin(),     vi.end(),            2 ));
    BOOST_CHECK ( ba::one_of       ( vi.begin(),     vi.end(), is_<int> ( 2 )));

//  Check for a match at the end
    BOOST_CHECK ( ba::one_of_equal ( vi,                                  5 ));
    BOOST_CHECK ( ba::one_of       ( vi,                       is_<int> ( 5 )));
    BOOST_CHECK ( ba::one_of_equal ( vi.begin(),     vi.end(),            5 ));
    BOOST_CHECK ( ba::one_of       ( vi.begin(),     vi.end(), is_<int> ( 5 )));

    BOOST_CHECK ( ba::one_of_equal ( vi.begin() + 1, vi.end(),            1 ));
    BOOST_CHECK ( ba::one_of       ( vi.begin() + 1, vi.end(), is_<int> ( 1 )));
    
    BOOST_CHECK ( ba::one_of_equal ( vc.begin() + 1, vc.begin() + 2,             'q' ));
    BOOST_CHECK ( ba::one_of       ( vc.begin() + 1, vc.begin() + 2, is_<char> ( 'q' )));

    BOOST_CHECK (!ba::one_of_equal ( vc, '!' ));
    BOOST_CHECK (!ba::one_of       ( vc, is_<char> ( '!' )));
    
    BOOST_CHECK (!ba::one_of_equal ( vc, 'n' ));
    BOOST_CHECK (!ba::one_of       ( vc, is_<char> ( 'n' )));

//  Empty range check
    BOOST_CHECK (!ba::one_of_equal ( vi.begin(), vi.begin(),   1 ));
    BOOST_CHECK (!ba::one_of_equal ( vc.begin(), vc.begin(), 'a' ));
    BOOST_CHECK (!ba::one_of       ( vi.begin(), vi.begin(), is_<int>  (   1 )));
    BOOST_CHECK (!ba::one_of       ( vc.begin(), vc.begin(), is_<char> ( 'a' )));
}


int test_main( int , char* [] )
{
  test_none();
  test_any ();
  test_all ();
  test_one ();
  return 0;
}
