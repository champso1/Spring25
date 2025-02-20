#ifndef __MEMORY_HXX
#define __MEMORY_HXX

#include "ColSim/Core/Types.hxx"


namespace ColSim {

namespace Mem {

	extern void* Alloc(USize numBytes);
	extern void* Calloc(USize elemSize, UInt32 numElems);
	extern void  Free(void** ptr);
	extern void* Realloc(void** ptr, USize size);
	extern void  Copy(void* dest, const void* src, USize numBytes);

}

}


#endif // __MEMORY_HXX
