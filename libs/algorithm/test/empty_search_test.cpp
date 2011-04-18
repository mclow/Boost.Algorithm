#include <string>
#include <boost/algorithm/search.hpp>

int main( int argc, char *argv [] )
{
	const std::string cs;
	std::string estr;
	std::string str ( "abc" );
	
//	Should compile successfully because the underlying types are the same.
//	One is const and the other is not, but that's OK
	(void) boost::algorithm::boyer_moore_search (
		cs.begin (), cs.end (), estr.begin (), estr.end ());

	(void) boost::algorithm::boyer_moore_horspool_search (
		cs.begin (), cs.end (), estr.begin (), estr.end ());

	(void) boost::algorithm::knuth_morris_pratt_search (
		cs.begin (), cs.end (), estr.begin (), estr.end ());

//	empty corpus, non-empty pattern
	(void) boost::algorithm::boyer_moore_search (
		estr.begin (), estr.end (), str.begin (), str.end ());

	(void) boost::algorithm::boyer_moore_horspool_search (
		estr.begin (), estr.end (), str.begin (), str.end ());

	(void) boost::algorithm::knuth_morris_pratt_search (
		estr.begin (), estr.end (), str.begin (), str.end ());

//	non-empty corpus, empty pattern
	(void) boost::algorithm::boyer_moore_search (
		str.begin (), str.end (), estr.begin (), estr.end ());

	(void) boost::algorithm::boyer_moore_horspool_search (
		str.begin (), str.end (), estr.begin (), estr.end ());

	(void) boost::algorithm::knuth_morris_pratt_search (
		str.begin (), str.end (), estr.begin (), estr.end ());

   (void) argv; (void) argc;
   return 0;
}
