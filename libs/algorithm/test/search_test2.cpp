/*

*/

#include <boost/algorithm/search.hpp>

#include <boost/test/included/test_exec_monitor.hpp>

#include <iostream>
#include <algorithm>
#include <vector>

typedef	std::vector<char> vec;
#define	NUM_TRIES	100

void pre_KMP ( const char *pat_first, const char *pat_last, int *skip /* count+1 */ )
{
  int m = pat_last - pat_first;
  int k;
  skip[0] = -1;
  for (int i = 1; i<m; i++)
    {
     k = skip[i-1];
     while (k>=0)
       {
        if (pat_first[k]==pat_first[i-1])
            break;
        else
            k = skip[k];
       }
     skip[i] = k + 1;
    }
}

const char * KMP( const char *corpus_first, const char * corpus_last,
					const char * pat_first, const char * pat_last )
{
 int m = pat_last - pat_first;
 int n = corpus_last - corpus_first;
 std::vector<int> f ( m + 1 );
 boost::algorithm::detail::kmp::create_skip_table ( pat_first, pat_last, f );
// pre_KMP(pat_first, pat_last, f);
 int i = 0;
 int k = 0;
 while (i<n)
    {
     if (k==-1)
        {
         i++;
         k = 0;
        }
     else if (corpus_first[i]==pat_first[k]) 
        {
         i++;
         k++;
         if (k==m)
            return corpus_first + i-k;
        }
     else
        k=f[k];
    }
 return corpus_last;
}


namespace {

	vec ReadFromFile ( const char *name ) {
		std::ifstream in ( name, std::ios_base::binary | std::ios_base::in );
		vec retVal;
		std::istream_iterator<char, char> begin(in);
		std::istream_iterator<char, char> end;
		
		std::copy ( begin, end, std::back_inserter ( retVal ));
		return retVal;
		}
	
	float timeDiffToSecs ( std::clock_t start, std::clock_t stop ) {
		return ((float) ( stop - start )) / CLOCKS_PER_SEC;
		}
	
//	A simple predicate for testing the predicate versions
	bool Equal ( const char &ch1, const char &ch2 ) {	return ~ch1 == ~ch2; }
	
	void printRes ( const char *prompt, float secs, float stdSecs ) {
		std::cout << prompt 
			<< std::setw(6) << secs << " seconds\t"
			<< std::setw(5) << (secs / stdSecs) * 100 << '%';
		if ( secs > stdSecs ) 
			std::cout << " !!";
		std::cout << std::endl;
		}
	
	void check_one ( const vec &haystack, const vec &needle, int expected ) {
		std::size_t i;
		std::clock_t bTime, eTime;
		float tmpSecs;
		
		vec::const_iterator res;
		vec::const_iterator exp;		// the expected result
		
		if ( expected >= 0 )
			exp = haystack.begin () + expected;
		else if ( expected == -1 )
			exp = haystack.end ();		// we didn't find it1
		else if ( expected == -2 )
			exp = std::search ( haystack.begin (), haystack.end (), needle.begin (), needle.end ());
		else	
			throw std::logic_error ( "Expected must be -2, -1, or >= 0" );

		std::cout << "Pattern is " << needle.size ()   << " entries long" << std::endl;
		std::cout << "Corpus  is " << haystack.size () << " entries long" << std::endl;
	//	First, the std library search
		float stdSecs;
		bTime = std::clock ();
		for ( i = 0; i < NUM_TRIES; ++i ) {
			res = std::search ( haystack.begin (), haystack.end (), needle.begin (), needle.end ());
			if ( res != exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from std::search" );
				}
			}
		eTime = std::clock ();
		stdSecs = timeDiffToSecs ( bTime, eTime );
		tmpSecs = timeDiffToSecs ( bTime, eTime );
		printRes ( "std::search                               took ", tmpSecs, stdSecs );

	//	Now, the boyer_moore search
		bTime = std::clock ();
		for ( i = 0; i < NUM_TRIES; ++i ) {
			res = boost::algorithm::boyer_moore_search 
				( haystack.begin (), haystack.end (), needle.begin (), needle.end ());
			if ( res != exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from boyer_moore_search" );
				}
			}
		eTime = std::clock ();
		tmpSecs = timeDiffToSecs ( bTime, eTime );
		printRes ( "boyer_moore_search                        took ", tmpSecs, stdSecs );

	//	Search using an object
		bTime = std::clock ();
		boost::algorithm::boyer_moore <vec::const_iterator> bm1 ( needle.begin (), needle.end ());
		for ( i = 0; i < NUM_TRIES; ++i ) {
			res = bm1 ( haystack.begin (), haystack.end ());
			if ( res != exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from knuth_morris_pratt_search" );
				}
			}
		eTime = std::clock ();
		tmpSecs = timeDiffToSecs ( bTime, eTime );
		printRes ( "boyer-moore object                        took ", tmpSecs, stdSecs );

	//	Now, the boyer_moore_horspool search
		bTime = std::clock ();
		for ( i = 0; i < NUM_TRIES; ++i ) {
			res = boost::algorithm::boyer_moore_horspool_search 
				( haystack.begin (), haystack.end (), needle.begin (), needle.end ());
			if ( res != exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from boyer_moore_horspool_search" );
				}
			}
		eTime = std::clock ();
		tmpSecs = timeDiffToSecs ( bTime, eTime );
		printRes ( "boyer_moore_horspool_search               took ", tmpSecs, stdSecs );

	//	Search using an object
		bTime = std::clock ();
		boost::algorithm::boyer_moore_horspool <vec::const_iterator> bmh1 ( needle.begin (), needle.end ());
		for ( i = 0; i < NUM_TRIES; ++i ) {
			res = bmh1 ( haystack.begin (), haystack.end ());
			if ( res != exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from knuth_morris_pratt_search" );
				}
			}
		eTime = std::clock ();
		tmpSecs = timeDiffToSecs ( bTime, eTime );
		printRes ( "boyer-moore-horspool object               took ", tmpSecs, stdSecs );

	//	Now, the knuth_morris_pratt search
		bTime = std::clock ();
		const char *pRes;
		for ( i = 0; i < NUM_TRIES; ++i ) {
			pRes = KMP ( &*haystack.begin (), &*haystack.end (), &*needle.begin (), &*needle.end ());
			if ( pRes != &*exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from knuth_morris_pratt_search" );
				}
			}
		eTime = std::clock ();
		tmpSecs = timeDiffToSecs ( bTime, eTime );
		printRes ( "knuth_morris_pratt_search (C version)     took ", tmpSecs, stdSecs );

	//	Now, the knuth_morris_pratt search
		bTime = std::clock ();
		for ( i = 0; i < NUM_TRIES; ++i ) {
			res = boost::algorithm::knuth_morris_pratt_search 
				( haystack.begin (), haystack.end (), needle.begin (), needle.end ());
			if ( res != exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from knuth_morris_pratt_search" );
				}
			}
		eTime = std::clock ();
		tmpSecs = timeDiffToSecs ( bTime, eTime );
		printRes ( "knuth_morris_pratt_search                 took ", tmpSecs, stdSecs );

	//	Now, the knuth_morris_pratt search
		bTime = std::clock ();
		boost::algorithm::knuth_morris_pratt<vec::const_iterator> kmp1 ( needle.begin (), needle.end ());
		for ( i = 0; i < NUM_TRIES; ++i ) {
			res = kmp1 ( haystack.begin (), haystack.end ());
			if ( res != exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from knuth_morris_pratt_search" );
				}
			}
		eTime = std::clock ();
		tmpSecs = timeDiffToSecs ( bTime, eTime );
		printRes ( "knuth_morris_pratt object                 took ", tmpSecs, stdSecs );
		std::cout << std::endl;

		float stdPSecs;
		bTime = std::clock ();
		for ( i = 0; i < NUM_TRIES; ++i ) {
			res = std::search ( haystack.begin (), haystack.end (), needle.begin (), needle.end (), Equal );
			if ( res != exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from std::search(p)" );
				}
			}
		eTime = std::clock ();
		stdPSecs = timeDiffToSecs ( bTime, eTime );
		tmpSecs = timeDiffToSecs ( bTime, eTime );
		printRes ( "std::search (w/predicate)                 took ", tmpSecs, stdPSecs );

		bTime = std::clock ();
		for ( i = 0; i < NUM_TRIES; ++i ) {
			res = boost::algorithm::boyer_moore_search 
				( haystack.begin (), haystack.end (), needle.begin (), needle.end (), Equal );
			if ( res != exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from boyer_moore_search(p)" );
				}
			}
		eTime = std::clock ();
		tmpSecs = timeDiffToSecs ( bTime, eTime );
		printRes ( "boyer_moore_search (w/predicate)          took ", tmpSecs, stdPSecs );

		bTime = std::clock ();
		boost::algorithm::boyer_moore <vec::const_iterator> bm2 ( needle.begin (), needle.end ());
		for ( i = 0; i < NUM_TRIES; ++i ) {
			res = bm2 (  haystack.begin (), haystack.end (), Equal );
			if ( res != exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from knuth_morris_pratt_search" );
				}
			}
		eTime = std::clock ();
		tmpSecs = timeDiffToSecs ( bTime, eTime );
		printRes ( "boyer-moore object (w/predicate)          took ", tmpSecs, stdPSecs );

		bTime = std::clock ();
		for ( i = 0; i < NUM_TRIES; ++i ) {
			res = boost::algorithm::boyer_moore_horspool_search 
				( haystack.begin (), haystack.end (), needle.begin (), needle.end (), Equal );
			if ( res != exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from boyer_moore_horspool_search(p)" );
				}
			}
		eTime = std::clock ();
		tmpSecs = timeDiffToSecs ( bTime, eTime );
		printRes ( "boyer_moore_horspool_search (w/predicate) took ", tmpSecs, stdPSecs );

		bTime = std::clock ();
		boost::algorithm::boyer_moore_horspool <vec::const_iterator> bmh2 ( needle.begin (), needle.end ());
		for ( i = 0; i < NUM_TRIES; ++i ) {
			res = bmh2 (  haystack.begin (), haystack.end (), Equal );
			if ( res != exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from knuth_morris_pratt_search" );
				}
			}
		eTime = std::clock ();
		tmpSecs = timeDiffToSecs ( bTime, eTime );
		printRes ( "boyer-moore-horspool object (w/predicate) took ", tmpSecs, stdPSecs );

	//	Now, the knuth_morris_pratt search
		bTime = std::clock ();
		for ( i = 0; i < NUM_TRIES; ++i ) {
			res = boost::algorithm::knuth_morris_pratt_search 
				( haystack.begin (), haystack.end (), needle.begin (), needle.end (), Equal);
			if ( res != exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from knuth_morris_pratt_search" );
				}
			}
		eTime = std::clock ();
		tmpSecs = timeDiffToSecs ( bTime, eTime );
		printRes ( "knuth_morris_pratt_search (w/predicate)   took ", tmpSecs, stdPSecs );

	//	Now, the knuth_morris_pratt search
		bTime = std::clock ();
		boost::algorithm::knuth_morris_pratt <vec::const_iterator> kmp2 ( needle.begin (), needle.end ());
		for ( i = 0; i < NUM_TRIES; ++i ) {
			res = kmp2 (  haystack.begin (), haystack.end (), Equal );
			if ( res != exp ) {
				std::cout << "On run # " << i << " expected " << exp - haystack.begin () << " got " << res - haystack.begin () << std::endl;
				throw std::runtime_error ( "Unexpected result from knuth_morris_pratt_search" );
				}
			}
		eTime = std::clock ();
		tmpSecs = timeDiffToSecs ( bTime, eTime );
		printRes ( "knuth_morris_pratt object (w/predicate)   took ", tmpSecs, stdPSecs );
		std::cout << std::endl;
		}
	}

int test_main( int , char* [] )
{
	std::vector<char> c1  = ReadFromFile ( "data-files/0001.corpus" );
	std::vector<char> p1b = ReadFromFile ( "data-files/0001b.pat" );
	std::vector<char> p1e = ReadFromFile ( "data-files/0001e.pat" );
	std::vector<char> p1n = ReadFromFile ( "data-files/0001n.pat" );
	std::vector<char> p1f = ReadFromFile ( "data-files/0001f.pat" );
	
	std::cout << std::ios::fixed << std::setprecision(4);
//	std::cout << "Corpus is " << c1.size () << " entries long\n";
	std::cout << "--- Beginning ---" << std::endl;
	check_one ( c1, p1b, 0 );		//	Find it at position zero
	std::cout << "---- Middle -----" << std::endl;
	check_one ( c1, p1f, -2 );		//	Don't know answer
	std::cout << "------ End ------" << std::endl;
	check_one ( c1, p1e, c1.size() - p1e.size ());	
	std::cout << "--- Not found ---" << std::endl;
	check_one ( c1, p1n, -1 );		//	Not found
	
	return 0;
	}
