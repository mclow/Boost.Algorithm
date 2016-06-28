#ifndef BOOST_ALGORITHM_is_palindromic_HPP
#define BOOST_ALGORITHM_is_palindromic_HPP

#include <iterator>

#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>

namespace boost {  namespace algorithm {

template <typename BidirectionalIterator, typename Comp>
bool is_palindromic(BidirectionalIterator begin, BidirectionalIterator end, Comp comp)
{
    --end;
    while(std::distance(begin, end) > 0)
    {
        if(!comp(*begin, *end))
        {
            return false;
        }
        ++begin;
        --end;
    }
    return true;
}

template <typename BidirectionalIterator>
bool is_palindromic(BidirectionalIterator begin, BidirectionalIterator end)
{
    return is_palindromic(begin, end,
                         std::equal_to<typename std::iterator_traits<BidirectionalIterator>::value_type> ());
}


template <typename R>
bool is_palindromic(const R& range)
{
    return is_palindromic(boost::begin(range), boost::end(range));
}


template <typename R, typename Comp>
bool is_palindromic(const R& range, Comp c)
{
    return is_palindromic(boost::begin(range), boost::end(range), c);
}

}}

#endif // BOOST_ALGORITHM_is_palindromic_HPP
