#include <iostream>

// All containers should inherit from IContainer<T>
// IContainer<T> should implement a way to get elements through virtualization
// All iterators should inherit from IIterator<T>
// Enumerator should be a special type of Iterator that uses type erasure so that all Iterators can be converted into an Enumerator

// IContainer should only be an interface, but it needs to somehow also be able to return primitive iterators.
// IContainer::Begin should return an Enumerator which is type erased. The implementation of constructing the Enumerator should be in the implementing container class.

#include "Array.hpp"

#include <forward_list>

using iter = typename std::forward_list< int >::iterator;
using fitr = ForwardIterator< iter >;

int main()
{
	std::forward_list< int > fl = { 1, 2, 3, 4 };
	fitr beg( fl.begin() );
	fitr end( fl.end() );
	Enumerable< int > enm( 4, beg.ToEnumerator(), end.ToEnumerator() );

	auto enumerator = enm.Begin();
	*enumerator = 4;
	enumerator += 3;
	*enumerator = 3;
	--enumerator;
	*enumerator = 10;
	enumerator[ -1 ] = 11;
}