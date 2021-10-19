#ifdef DEBUG
# include <csignal>
# ifdef __linux__
#   include <fenv.h>
# endif
#endif
#include "Debug.h"


void InitDebugFeatures() 
{
	#ifdef DEBUG
	# ifdef __linux__
			feenableexcept(FE_OVERFLOW | FE_DIVBYZERO | FE_INVALID);
	# endif
			//  signal(SIGTSTP,[](int) {
			//  });
	#endif
}


