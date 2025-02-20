#include "Colsim/Core/Memory.hxx"


#include <cstdlib>
#include <cstring>
#include <exception>


namespace ColSim {

namespace Mem {

	void* Alloc(USize numBytes) {
		void* ptr = malloc(numBytes);
		if (ptr == nullptr)
		    exit(EXIT_FAILURE);
		
		return ptr;
	}


	void* Calloc(USize numElems, UInt32 elemSize) {
		void* ptr = calloc(numElems, elemSize);
		if (ptr == nullptr) 
		    exit(EXIT_FAILURE);
		
		return ptr;
	}


	void Free(void** ptr) {
		if (*ptr == nullptr)
			return; // don't error out in this case
		
		free(*ptr);
		*ptr = nullptr;
	}



	void* Realloc(void** ptr, UInt32 size) {
		if (*ptr == nullptr)
			return nullptr;
		
		void* newPtr = realloc(*ptr, size);
		if (newPtr == nullptr)
		    exit(EXIT_FAILURE);
		
		*ptr = newPtr;
		return newPtr;
	}

	void Copy(void* dest, const void* src, USize numBytes) {
		if ((dest == nullptr) || (src == nullptr))
			return;
		
	    memcpy(dest, src, numBytes);
	}

	
} // namespace Mem

} // namespace ColSim
