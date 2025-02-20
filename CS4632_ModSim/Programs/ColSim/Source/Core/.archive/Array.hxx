#ifndef __ARRAY_HXX
#define __ARRAY_HXX


#include "ColSim/Core/Types.hxx"
#include "ColSim/Core/Memory.hxx"

#include <initializer_list>
#include <exception>
#include <iostream>


namespace ColSim {


template <typename T>
class Array {
private:
	static const USize INITIAL_SIZE = 32; //< Default initial size for memory allocation.
	static constexpr const USize DATA_SIZE = sizeof(T);

	T* data;          //< Actual data.
	USize length;     //< Number of stored elements.
	USize size;       //< Capacity; number of *allocated* -- not stored -- elements.

public:

	// ******************************
	//    Normal (Con/De)structors
	// ******************************
	
	Array();
	Array(T* data, USize length);
	Array(std::initializer_list<T> list);

	~Array();

	
	// ******************************
	//     Copy/Move Constructors  
	// ******************************

	Array(const Array<T>& other);
	Array(Array<T>&& other) noexcept;

	
	// ******************************
	//          Operators
	// ******************************
	
	Array<T>& operator=(const Array<T>& other);
	// do not allow move assignment
	Array<T>& operator=(Array<T>&& other) noexcept = delete;
	
	Array<T>& operator+=(const Array<T>& arr);
	Array<T>& operator+=(const T elem);

	template <typename U>
	friend Array<U> operator+(const Array<U>& arr1, const Array<U>& arr2);

	
	T&    operator[](UInt32 idx);
	T     operator[](UInt32 idx) const;


	
	class Iterator {
	private:
		T* ptr;

	public:
		explicit Iterator(T* _ptr) : ptr(_ptr) {}

		T& operator*()  const { return *ptr; }
		T* operator->() const { return  ptr; }

		// pre increment
		Iterator& operator++() {
			++ptr;
			return *this;
		}
		// post increment
		Iterator operator++(int) {
			Iterator temp = *this;
			++ptr;
			return temp;
		}

		Bool operator==(const Iterator& other) const {
			return (ptr == other.ptr);
		}
		Bool operator!=(const Iterator& other) const {
			return (ptr != other.ptr);
		}
	}; // class Iterator
	

	Iterator begin() {
		return Iterator(data);
	}
	Iterator end() {
		return Iterator(data + length);
	}




	// ******************************
	//    Public Member Functions
	// ******************************

	
	void append(T elem);
    void append(const Array<T>& arr);
	void append(std::initializer_list<T> list);
	
	void remove(UInt32 idx);

	Array<T> slice(UInt32 begin, UInt32 end) const;


private:
	// ******************************
	//    Private Member Functions
	// ******************************
	
	void resize();

	// useful for functions that have already
	// resized the array to required size
	void appendNoBoundCheck(T elem);
	
}; // class Array



// As a template class, the implementation follows
// here in the header file.



// ******************************
//    Normal (Con/De)structors
// ******************************


template <typename T>
Array<T>::Array() : length(0), size(INITIAL_SIZE)
{
	data = (T*)Mem::Calloc(size, DATA_SIZE);
}


template <typename T>
Array<T>::Array(T* data, USize length) : length(length) {
	USize init_size = INITIAL_SIZE;
	while (init_size < length) init_size *= 2;
		
	size = init_size;
	data = (T*)Mem::Calloc(size, DATA_SIZE);
}

template <typename T>
Array<T>::Array(std::initializer_list<T> list) {
	USize init_size = INITIAL_SIZE;
	USize list_size = list.size();
	while (init_size < list_size) init_size *= 2;
		
	size = init_size;
	length = list_size;
	data = (T*)Mem::Calloc(size, DATA_SIZE);
	
	
	auto it = list.begin();
	for (int i=0; i<list_size; i++) {
		data[i] = *it++;
	}
}

template <typename T>
Array<T>::~Array() {
	Mem::Free((void**)(&data));
}



// ******************************
//    Copy/Move Constructors
// ******************************

template <typename T>
Array<T>::Array(const Array<T>& other) {
	length = other.length;
	size = other.size;
	Mem::Free(&data);
    data = Mem::Calloc(size, DATA_SIZE);
	Mem::Copy(data, other.data, length);
}

template <typename T>
Array<T>::Array(Array<T>&& other) noexcept {
	// identical to copy constructor
	// but will just free the other data
	length = other.length;
	size = other.size;
	Mem::Free(&data);
    data = Mem::Calloc(size, DATA_SIZE);
	Mem::Copy(data, other.data, length);
	Mem::Free(&other.data);
}




// ******************************
//           Operators
// ******************************

template <typename T>
Array<T>& Array<T>::operator=(const Array<T>& other) {
	// effectively the same as the copy constructor
	length = other.length;
	size = other.size;
	Mem::Free(&data);
    data = Mem::Calloc(size, DATA_SIZE);
	Mem::Copy(data, other.data, length);
	// also return a reference to self
	return *this;
}

template <typename T>
Array<T>& Array<T>::operator+=(const Array<T>& arr) {
	append(arr);
	return *this;
}

template <typename T>
Array<T>& Array<T>::operator+=(const T elem) {
	append(elem);
	return *this;
}

template <typename U>
Array<U> operator+(const Array<U>& arr1, const Array<U>& arr2) {
	Array<U> arr;
	arr.append(arr1);
	arr.append(arr2);
	return arr;
}

template <typename T>
T& Array<T>::operator[](UInt32 idx) {
	if (length == 0) throw std::out_of_range("Array has no elements");
	if (idx >= length) return data[length-1];
	return data[idx];
}

template <typename T>
T Array<T>::operator[](UInt32 idx) const {
	if (length == 0) throw std::out_of_range("Array has no elements");
	if (idx >= length) return data[length-1];
	return data[idx];
}




// ******************************
//    Public Member Functions
// ******************************

template <typename T>
void Array<T>::append(T elem) {
	// simple if statement would suffice here but too bad!
	while ((length + 1) >= size)
		resize();

	data[length] = elem;
	length += 1;
}

template <typename T>
void Array<T>::append(const Array<T>& arr) {
	USize reqSize = length + arr.length;
	while (reqSize >= size)
		resize();

	// we have already resized
	for (const T& elem : arr)
		appendNoBoundCheck(elem);
}

template <typename T>
void Array<T>::append(std::initializer_list<T> list) {
	USize reqSize = length + list.size();
	while (reqSize >= size)
		resize();

	for (const T elem : list) append(elem);
}


template <typename T>	
void Array<T>::remove(UInt32 idx) {
	if (idx >= length)
		throw std::out_of_range("Index out of range.");
	
	Mem::Copy(data + idx,
			  data + idx + 1,
			  length - idx - 1);
	length -= 1;
}

template <typename T>
Array<T> Array<T>::slice(UInt32 beginIdx, UInt32 endIdx) const {
	if ((begin >= end) || (begin >= length) || (end >= length))
	    throw std::out_of_range("Invalid begin/end arguments");

	Array<T> newArr;
	Mem::Copy(newArr.data, data + beginIdx, endIdx - beginIdx);
	return newArr;
}





// ******************************
//   Private Member Functions
// ******************************

template <typename T>
void Array<T>::resize() {
	size *= 2;
	data = Mem::Realloc(&data, size);
}

template <typename T>
void Array<T>::appendNoBoundCheck(T elem) {
	data[length] = elem;
	length += 1;
}


} // namespace ColSim

#endif // __ARRAY_HXX
