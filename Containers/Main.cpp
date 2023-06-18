#include <iostream>
#include <vector>
#include <memory>

// All containers should inherit from IContainer<T>
// IContainer<T> should implement a way to get elements through virtualization
// All iterators should inherit from IIterator<T>
// Enumerator should be a special type of Iterator that uses type erasure so that all Iterators can be converted into an Enumerator

// IContainer should only be an interface, but it needs to somehow also be able to return primitive iterators.
// IContainer::Begin should return an Enumerator which is type erased. The implementation of constructing the Enumerator should be in the implementing container class.

// I need to enforce template types for containers to not be references.
// Collection<T> needs to be properly fleshed out.
// Need to make two variants of each type of iterator adapter for const and non-const so that we respect constness properly.
// Need to rename the adapters to be something better.

#include "Array.hpp"


int main()
{
	Array< int, 5 > a = { 1, 3, 3, 3, 5 };
	Collection< int > c( 10 );
	c.Add( 1 );
	c.Add( 2 );
	c.Add( 3 );
	c.Add( 4 );
	c.Add( 5 );
	c.Add( 6 );
	Collection<int> c0;
	c0 = c;
	//Deferred< int > df = a.FindAll( 3 );
	//df.FindAll( []( const int& v ) { return false; } );

	//std::vector< int > vec = { 1,2,3,4,5 };

	//for ( auto& val : df.FindAll( 3 ) )
	//{
	//	std::cout << val << std::endl;
	//}
}