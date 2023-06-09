#include <iostream>

// All containers should inherit from IContainer<T>
// IContainer<T> should implement a way to get elements through virtualization
// All iterators should inherit from IIterator<T>
// Enumerator should be a special type of Iterator that uses type erasure so that all Iterators can be converted into an Enumerator

// IContainer should only be an interface, but it needs to somehow also be able to return primitive iterators.
// IContainer::Begin should return an Enumerator which is type erased. The implementation of constructing the Enumerator should be in the implementing container class.

#include "Array.hpp"

#include <forward_list>
#include <vector>

int main()
{
	Array<int, 3> ar;// = { 1, 2, 3 };
	ar.At( 0 ) = 1;
	ar.At( 1 ) = 2;
	ar.At( 2 ) = 3;

	IContiguousCollection<int>& arr = ar;
	auto enumer = arr.ToEnumerable();
	auto b0 = enumer.Begin() + 1;
	auto v1 = *b0;
	*( enumer.Begin() + 0 ) = 1;
	*( enumer.Begin() + 1 ) = 2;
	*( enumer.Begin() + 2 ) = 3;
	auto beg = enumer.CBegin();
	auto end = enumer.CEnd();
	CEnumerable< int > er1( 3, beg, end );

	for ( auto& c : er1 )
	{
		std::cout << c << std::endl;
	}
}