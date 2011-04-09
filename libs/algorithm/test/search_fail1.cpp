#include <vector>
#include <boost/algorithm/search.hpp>

int main( int argc, char *argv [] )
{
	std::vector<char>	cv;
	std::vector<int>	iv;
	
//	Should fail to compile because the underlying types are different
//	They are (almost certainly) different sizes
	(void) boost::algorithm::boyer_moore_search (
		cv.begin (), cv.end (), iv.begin (), iv.end ());

   
   (void) argv; (void) argc;
   return 0;
}
