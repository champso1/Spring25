#include "ColSim/Core/String.hxx"
#include "ColSim/Core/Memory.hxx"

#include <exception>
#include <cstring>
#include <cstdio>


namespace ColSim {

// ******************************
//    Public Member Functions
// ******************************

void String::append(const String& str) {
	if ((length + str.length) >= MAX_STRING_SIZE)
		throw std::length_error("String too long.");

	Mem::Copy(data + length, str.data, str.length);
	length += str.length;
}

void String::append(const char *const str) {
	USize strLen = strlen(str);
	if ((length + strLen) >= MAX_STRING_SIZE) 
		throw std::length_error("String too long.");
	
	Mem::Copy(data + length, str, strLen);
	length += strLen;
}

void String::append(const char c) {
	if ((length + 1) >= MAX_STRING_SIZE)
		throw new std::length_error("String too long.");

	data[length] = c;
	length++;
}


	
} /// namespace ColSim
