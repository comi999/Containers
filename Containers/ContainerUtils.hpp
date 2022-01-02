#include "Array.hpp"

using namespace std;

namespace ContainerUtils
{
	template < typename ValueType, typename ContainerA, typename ContainerB >
	auto Difference( ContainerA& a_ContainerA, ContainerB& a_ContainerB )
	{
		set< ValueType*, LessPtr< ValueType > > SetA;
		set< ValueType*, LessPtr< ValueType > > SetB;
		RefArray< ValueType > Result;

		for ( auto& Entry : Underlying() )
		{
			SetA.insert( &Entry );
		}

		auto End = ContainerTraits::End( a_Container );

		for ( auto Beg = ContainerTraits::Begin( a_Container ); Beg != End; ++Beg )
		{
			SetB.insert( &*Beg );
		}

		for ( auto Iterator = SetA.begin(); Iterator != SetA.end(); ++Iterator )
		{
			if ( SetB.insert( *Iterator ).second )
			{
				Result.emplace_back( **Iterator );
			}
		}

		return Result;
	}
};