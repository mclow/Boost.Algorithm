#include <vector>
#include <boost/cstdint.hpp>
#include <boost/algorithm/search.hpp>

int main( int argc, char *argv [] )
{
	std::vector<boost::uint8_t>	cv;
	std::vector<boost:: int8_t>	iv;
	
//	Should fail to compile because the underlying types are different
//	They are the same size, but one is signed, and the other is not.
	(void) boost::algorithm::boyer_moore_search (
		cv.begin (), cv.end (), iv.begin (), iv.end ());

   
   (void) argv; (void) argc;
   return 0;
}
