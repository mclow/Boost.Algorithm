#include <vector>
#include <boost/algorithm/search.hpp>

int main( int argc, char *argv [] )
{
//	Should fail to compile because the search objects are not default-constructible
	boost::algorithm::boyer_moore<std::vector<char>::iterator> bm;
   
   (void) argv; (void) argc;
   return 0;
}
