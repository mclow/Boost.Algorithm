#include <string>
#include <boost/algorithm/search.hpp>

int main( int argc, char *argv [] )
{
	const std::string cs;
	std::string ncs;
	
//	Should compile successfully because the underlying types are the same.
//	One is const and the other is not, but that's OK
	(void) boost::algorithm::boyer_moore_search (
		cs.begin (), cs.end (), ncs.begin (), ncs.end ());

	(void) boost::algorithm::boyer_moore_horspool_search (
		cs.begin (), cs.end (), ncs.begin (), ncs.end ());

	(void) boost::algorithm::knuth_morris_pratt_search (
		cs.begin (), cs.end (), ncs.begin (), ncs.end ());

   (void) argv; (void) argc;
   return 0;
}
