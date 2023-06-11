#include <iostream>

// All containers should inherit from IContainer<T>
// IContainer<T> should implement a way to get elements through virtualization
// All iterators should inherit from IIterator<T>
// Enumerator should be a special type of Iterator that uses type erasure so that all Iterators can be converted into an Enumerator

// IContainer should only be an interface, but it needs to somehow also be able to return primitive iterators.
// IContainer::Begin should return an Enumerator which is type erased. The implementation of constructing the Enumerator should be in the implementing container class.

#include "Array.hpp"

constexpr const Array< int, 2 > arr = { 2, 3, };
constexpr const std::array< int, 2 > ar0 = { 2, 3 };

int main()
{
	Array< int, 10 > ar = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
	ICollection< int >& arc = ar;

	//bool val = arc.Contains( 11 );
}