#ifndef __STRING_HXX
#define __STRING_HXX

#include "ColSim/Core/Types.hxx"
#include "ColSim/Core/Memory.hxx"

#include <iostream>
#include <cstring>
#include <type_traits>

namespace ColSim {

class String {
private:
	static const USize MAX_STRING_SIZE = 1024; //!< Default length for strings.
	
	SByte data[MAX_STRING_SIZE];    //!< Array of characters/bytes
	USize length;                   //!< Length of string.

public:
	// ******************************
	//     Normal Constructors
	// ******************************

	String() : data{0}, length(0) {}
	explicit String(const char *const str) : data{0}, length(0) {
	    length = strlen(str);
		Mem::Copy(data, str, length);
	}

	~String() {}

	// ******************************
	//          Operators
	// ******************************
	
	inline String& operator+=(const String& other) {
		append(other);
		return *this;
	}
	inline String& operator+=(const char *const other) {
		append(other);
		return *this;
	}
	inline String& operator+=(const char c) {
		append(c);
		return *this;
	}
	
    inline friend String operator+(const String& str1,
								   const String& str2)
	{
		String str = str1;
		str += str2;
		return str;
	}
	inline friend String operator+(const String& str1,
								   const char *const str2)
	{
		String str = str1;
		str += str2;
		return str;
	}
	inline friend String operator+(const char *const str1,
								   const String& str2)
	{
		String str(str1);
		str += str2;
		return str;
	}
	inline friend String operator+(const String& str1,
								   const char c)
	{
		String str = str1;
		str += c;
		return str;
	}
	inline friend String operator+(const char c,
								   const String& str1)
	{
		String str;
	    str += c;
		str += str1;
		return str;
	}


	inline friend std::ostream& operator<<(std::ostream& stream,
									const String& str)
	{
		stream << str.data;
		return stream;
	}

	// ------------------------------
	// concatenation with numeric things
	// ------------------------------

	// integers
	template <class T,
			  class = typename std::enable_if<
				  std::is_arithmetic<T>::value>::type
			  >
    inline String& operator+=(T x) {
		append(x);
		return *this;
	}

    template <class T,
			  class = typename std::enable_if<
				  std::is_arithmetic<T>::value>::type
			  >
	inline friend String operator+(const String& str, T x) {
		String new_str = str;
		new_str += x;
		return new_str;
	}
	
	template <class T,
			  class = typename std::enable_if<
				  std::is_arithmetic<T>::value>::type
			  >
	inline friend String operator+(T x, const String& str) {
		String new_str;
		new_str += x;
		new_str += str;
		return new_str;
	}




    // ******************************
	//   Public Member Functions
	// ******************************

	inline USize Length() const { return length; }


	void append(const String& str);
	void append(const char *const str);
	void append(const char c);


	
    template <class T,
			  class = typename std::enable_if<
				  std::is_arithmetic<T>::value>::type
			  >
    void append(T x);



	inline const char* CStr() const {
		return reinterpret_cast<const char*>(data);
	}
	
	
	
}; // class String






// definition for generic function
template <class T, class>
void String::append(T x) {
	char buf[MAX_STRING_SIZE] = {0};
	
	if (std::is_integral<T>::value) {
		if (std::is_signed<T>::value) {
			Int64 val = static_cast<Int64>(x);
			sprintf(buf, "%lld", val);
			append(buf);
		} else {
			UInt64 val = static_cast<UInt64>(x);
			sprintf(buf, "%llu", val);
			append(buf);
		}
	} else { // floating point
		Double val = static_cast<Double>(x);
		sprintf(buf, "%lf", val);
		append(buf);
	}
}





	

} // namespace ColSim






#endif // __STRING_HXX
