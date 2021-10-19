#pragma once
//#ifndef DEBUG
//#define DEBUG

#ifdef _revienta
# undef _revienta
#endif

// breakpoint en codigo
#ifdef NDEBUG 
	// release (no genera codigo en el binario)
	#define _revienta(cond) ((void)0)
	#define _if_debug(x)
	#define _if_not_debug(x) x
	//#if defined(_MSC_VER) // visual studio
	//	#include <intrin.h> // es necesario??
	//	#define _revienta(cond) { if (cond) __debugbreak(); }
	////#elif defined(__GNUC__) // gcc
	////	//#define _revienta(cond) { if (cond) { asm("int3"); } asm("nop"); }
	//#else // cualquier otro ==> assert
	//	#include <cassert>
	//	#define _revienta(cond) assert(!(cond));
	//#endif

#else 
	//#include <cassert>
	//#define _revienta(cond) assert(!(cond));
	#include <intrin.h> // es necesario??
	#define _revienta(cond) { if (cond) __debugbreak(); }
	#define _if_debug(x) x
	#define _if_not_debug(x)
#endif

#define _warning(cond) _revienta(cond)
#define _not_implemented _revienta(true)
#define _impossible _revienta(true)
#define _expects(cond) _revienta(!(cond))
#define _ensures(cond) _revienta(!(cond))
#define _if_el_debug(x1,x2) _if_debug(x1) _if_not_debug(x2)

void InitDebugFeatures();

