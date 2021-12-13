#pragma once
#include <array>
#include <vector>
#include <algorithm>

#include "Invoker.hpp"
#include "Reference.hpp"
#include "Enumerable.hpp"

using namespace std;

template < typename T, size_t SIZE >
class Array;

template < typename ValueType >
using InitializerList = initializer_list< ValueType >;

template < typename ValueType, size_t SIZE >
class ReadOnlyArray : private array< ValueType, SIZE >
{
public:

	using Base = array< ValueType, SIZE >;
	using CIterator = array< ValueType, SIZE >::const_iterator;
	using CRIterator = array< ValueType, SIZE >::const_reverse_iterator;

	template < size_t LENGTH >
	ReadOnlyArray( const ReadOnlyArray< ValueType, LENGTH >& a_ReadOnlyArray )
	{
		memcpy( Base::data(), a_ReadOnlyArray.data(), ( a_ReadOnlyArray().size() < Base::size() ? a_ReadOnlyArray.size() : Base::size() ) * sizeof( ValueType ) );
	}

	template < size_t LENGTH >
	ReadOnlyArray( const Array< ValueType, LENGTH >& a_Array )
	{
		memcpy( Base::data(), a_Array.data(), ( a_Array.size() < Base::size() ? a_Array.size() : Base::size() ) * sizeof( ValueType ) );
	}

	ReadOnlyArray( const InitializerList< ValueType >& a_InitializerList )
	{
		memcpy( Base::data(), a_InitializerList.begin(), ( a_InitializerList.size() < Base::size() ? a_InitializerList.size() : Base::size() ) * sizeof( ValueType ) );
	}

	ReadOnlyArray( const InitializerList< ValueType >&& a_InitializerList )
	{
		memcpy( Base::data(), a_InitializerList.begin(), ( a_InitializerList.size() < Base::size() ? a_InitializerList.size() : Base::size() ) * sizeof( ValueType ) );
	}

	inline constexpr auto& At( size_t a_Position ) const
	{
		return Base::at( a_Position );
	}

	inline constexpr auto& Back() const
	{
		return Base::back();
	}

	template < size_t LENGTH >
	inline auto Combine( const ReadOnlyArray< ValueType, LENGTH >& a_ReadOnlyArray )
	{
		Array< ValueType, Base::size() + a_ReadOnlyArray.size() > Result;
		Result.MemCopy( 0, Base::data(), Base::size() );
		Result.MemCopy( Base::size(), a_ReadOnlyArray.data(), a_ReadOnlyArray.size() );
		return Result.AsReadOnly();
	}

	inline auto Data() const
	{
		return Base::data();
	}

	template < size_t LENGTH >
	inline bool Equals( const ReadOnlyArray< ValueType, LENGTH >& a_ReadOnlyArray ) const
	{
		if constexpr ( Base::size() != a_ReadOnlyArray.size() )
		{
			return false;
		}

		const char* Array1 = reinterpret_cast< const char* >( Base::data() );
		const char* Array2 = reinterpret_cast< const char* >( a_ReadOnlyArray.data() );

		for ( size_t i = 0; i < Base::size() * sizeof( ValueType ); ++i )
		{
			if ( Array1[ i ] != Array2[ i ] )
			{
				return false;
			}
		}

		return true;
	}

	template < size_t LENGTH >
	inline bool Equals( const Array< ValueType, LENGTH >& a_Array ) const
	{
		if constexpr ( Base::size() != a_Array.size() )
		{
			return false;
		}

		const char* Array1 = reinterpret_cast< const char* >( Base::data() );
		const char* Array2 = reinterpret_cast< const char* >( a_Array.data() );

		for ( size_t i = 0; i < Base::size() * sizeof( ValueType ); ++i )
		{
			if ( Array1[ i ] != Array2[ i ] )
			{
				return false;
			}
		}

		return true;
	}

	bool Exists( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( const auto& Entry : Underlying() )
		{
			if ( a_Predicate.Invoke( Entry ) )
			{
				return true;
			}
		}

		return false;
	}

	auto Find( const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = CBegin(); Iterator < CEnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return CReference< ValueType >( *Iterator );
			}
		}

		return CReference< ValueType >();
	}

	auto Find( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = CBegin(); Iterator < CEnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return CReference< ValueType >( *Iterator );
			}
		}

		return CReference< ValueType >();
	}

	auto Find( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				return CReference< ValueType >( *a_Begin );
			}
		}

		return CReference< ValueType >();
	}

	auto Find( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				return CReference< ValueType >( *a_Begin );
			}
		}

		return CReference< ValueType >();
	}

	auto FindIterator( const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = CBegin(); Iterator < CEnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator;
			}
		}

		return CEnd();
	}

	auto FindIterator( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = CBegin(); Iterator < CEnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator;
			}
		}

		return CEnd();
	}

	auto FindIterator( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				return a_Begin;
			}
		}

		return CEnd();
	}

	auto FindIterator( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				return a_Begin;
			}
		}

		return CEnd();
	}

	inline auto FindAll( const Predicate< const ValueType& >& a_Predicate )
	{
		Array< CReference< ValueType > > Result;

		for ( const auto& Entry : Underlying() )
		{
			if ( a_Predicate.Invoke( Entry ) )
			{
				Result.EmplaceBack( Entry );
			}
		}

		return Result;
	}

	inline auto FindAll( const Predicate< const ValueType& >& a_Predicate ) const
	{
		Array< CReference< ValueType > > Result;

		for ( const auto& Entry : Underlying() )
		{
			if ( a_Predicate.Invoke( Entry ) )
			{
				Result.EmplaceBack( Entry );
			}
		}

		return Result;
	}

	inline auto FindAll( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		Array< CReference< ValueType > > Result;

		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.EmplaceBack( *a_Begin );
			}
		}

		return Result;
	}

	inline auto FindAll( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		Array< CReference< ValueType > > Result;

		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.EmplaceBack( *a_Begin );
			}
		}

		return Result;
	}

	inline auto FindIteratorAll( const Predicate< const ValueType& >& a_Predicate )
	{
		Array< CIterator > Result;

		for ( auto Iterator = CBegin(); Iterator < CEnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				Result.EmplaceBack( Iterator );
			}
		}

		return Result;
	}

	inline auto FindIteratorAll( const Predicate< const ValueType& >& a_Predicate ) const
	{
		Array< CIterator > Result;

		for ( auto Iterator = CBegin(); Iterator < CEnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				Result.EmplaceBack( Iterator );
			}
		}

		return Result;
	}

	inline auto FindIteratorAll( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		Array< CIterator > Result;

		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.EmplaceBack( a_Begin );
			}
		}

		return Result;
	}

	inline auto FindIteratorAll( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		Array< CIterator > Result;

		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.EmplaceBack( a_Begin );
			}
		}

		return Result;
	}

	inline auto FindLast( const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = CRBegin(); Iterator < CREnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return CReference< ValueType >( *( Iterator.base() - 1 ) );
			}
		}

		return CReference< ValueType >();
	}

	inline auto FindLast( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = CRBegin(); Iterator < CREnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return CReference< ValueType >( *( Iterator.base() - 1 ) );
			}
		}

		return CReference< ValueType >();
	}

	inline auto FindLast( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = a_End - 1; Iterator >= a_Begin(); --Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return CReference< ValueType >( *Iterator );
			}
		}

		return CReference< ValueType >();
	}

	inline auto FindLast( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = a_End - 1; Iterator >= a_Begin(); --Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return CReference< ValueType >( *Iterator );
			}
		}

		return CReference< ValueType >();
	}

	inline auto FindIteratorLast( const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = CRBegin(); Iterator < CREnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator.base() - 1;
			}
		}

		return CEnd();
	}

	inline auto FindIteratorLast( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = CRBegin(); Iterator < CREnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator.base() - 1;
			}
		}

		return CEnd();
	}

	inline auto FindIteratorLast( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = a_End - 1; Iterator >= a_Begin(); --Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator;
			}
		}

		return CEnd();
	}

	inline auto FindIteratorLast( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = a_End - 1; Iterator >= a_Begin(); --Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator;
			}
		}

		return CEnd();
	}

	inline void ForEach( const Action< const ValueType& >& a_Action ) const
	{
		for ( const auto& Entry : Underlying() )
		{
			a_Action.Invoke( Entry );
		}
	}

	inline void ForEach( CIterator a_Begin, CIterator a_End, const Action< const ValueType& >& a_Action ) const
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			a_Action.Invoke( *a_Begin );
		}
	}

	inline auto& Front() const
	{
		return Base::front();
	}

	inline auto MaxSize() const
	{
		return Base::max_size();
	}

	inline auto Size() const
	{
		return Base::size();
	}

	template < size_t OFFSET, size_t LENGTH = ( SIZE > OFFSET ? SIZE - OFFSET : 0 ) >
	inline constexpr auto& SubArray()
	{
		return *reinterpret_cast< ReadOnlyArray< ValueType, LENGTH >* >( Base::data() + OFFSET );
	}

	template < size_t OFFSET, size_t LENGTH = ( SIZE > OFFSET ? SIZE - OFFSET : 0 ) >
	inline auto& SubArray() const
	{
		return *reinterpret_cast< const ReadOnlyArray< ValueType, LENGTH >* >( Base::data() + OFFSET );
	}

	inline void Swap( ReadOnlyArray< ValueType, SIZE >& a_ReadOnlyArray )
	{
		Base::swap( *reinterpret_cast< Base* >( &a_ReadOnlyArray ) );
	}

	inline bool TrueForAll( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( const auto& Entry : Underlying() )
		{
			if ( !a_Predicate.Invoke( Entry ) )
			{
				return false;
			}
		}

		return true;
	}

	inline bool TrueForAll( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( !a_Predicate.Invoke( *a_Begin ) )
			{
				return false;
			}
		}

		return true;
	}

	inline const auto& Underlying() const
	{
		return *static_cast< const Base* >( this );
	}

	#pragma region Operator
	inline constexpr auto& operator[]( size_t a_Position ) const
	{
		return Base::operator[]( a_Position );
	}
	#pragma endregion

	#pragma region Iterator
	inline CIterator CBegin() const
	{
		return Base::cbegin();
	}

	inline auto CRBegin() const
	{
		return Base::crbegin();
	}

	inline auto CEnd() const
	{
		return Base::cend();
	}

	inline auto CREnd() const
	{
		return Base::crend();
	}
	#pragma endregion

private:

	template < class, size_t > friend class Array;

};

template < typename ValueType, size_t SIZE = size_t( -1 ) >
class Array : private array< ValueType, SIZE >
{
	template < typename T >
	struct LessPtr
	{
		bool operator()( const T* Left, const T* Right ) const
		{
			return *Left < *Right;
		}
	};

public:

	using Base = array< ValueType, SIZE >;
	using Iterator = array< ValueType, SIZE >::iterator;
	using CIterator = array< ValueType, SIZE >::const_iterator;
	using RIterator = array< ValueType, SIZE >::reverse_iterator;
	using CRIterator = array< ValueType, SIZE >::const_reverse_iterator;

	Array() = default;

	template < size_t LENGTH >
	Array( const Array< ValueType, LENGTH >& a_Array )
	{
		a_Array.CopyTo( *this );
	}

	Array( const InitializerList< ValueType >& a_InitializerList )
	{
		memcpy( Base::data(), a_InitializerList.begin(), ( a_InitializerList.size() < Base::size() ? a_InitializerList.size() : Base::size() ) * sizeof( ValueType ) );
	}

	Array( const InitializerList< ValueType >&& a_InitializerList )
	{
		memcpy( Base::data(), a_InitializerList.begin(), ( a_InitializerList.size() < Base::size() ? a_InitializerList.size() : Base::size() ) * sizeof( ValueType ) );
	}

	inline Enumerable< ValueType > AsEnumerable()
	{
		return Enumerable< ValueType >( Base::begin(), Base::end(), Base::size() );
	}

	inline auto& AsReadOnly()
	{
		return *reinterpret_cast< ReadOnlyArray< ValueType, SIZE >* >( this );
	}

	inline auto& AsReadOnly() const
	{
		return *reinterpret_cast< const ReadOnlyArray< ValueType, SIZE >* >( this );
	}

	inline void Assign( const ValueType& a_Value )
	{
		Base::assign( a_Value );
	}

	inline void Assign( size_t a_Position, size_t a_Length, const ValueType& a_Value )
	{
		auto Begin = Base::data() + a_Position;
		auto End = Begin + a_Length;

		for ( ; Begin < End; ++Begin )
		{
			*Begin = a_Value;
		}
	}

	inline void Assign( Iterator a_Begin, Iterator a_End, const ValueType& a_Value )
	{
		while ( a_Begin < a_End )
		{
			*a_Begin++ = a_Value;
		}
	}

	inline auto& At( size_t a_Position )
	{
		return Base::at( a_Position );
	}

	inline constexpr auto& At( size_t a_Position ) const
	{
		return Base::at( a_Position );
	}

	inline auto& Back()
	{
		return Base::back();
	}

	inline auto& Back() const
	{
		return Base::back();
	}

	template < size_t LENGTH >
	inline auto Combine( const Array< ValueType, LENGTH >& a_Array ) const
	{
		Array< ValueType, Base::size() + a_Array.size() > Result;
		Result.MemCopy( 0, Base::data(), Base::size() );
		Result.MemCopy( Base::size(), a_Array.data(), a_Array.size() );
		return Result;
	}

	template <>
	inline auto Combine( const Array< ValueType >& a_Array ) const
	{
		Array< ValueType > Result;
		Result.resize( Base::size() + a_Array.size() );
		Result.MemCopy( 0, Base::data(), Base::size() );
		Result.MemCopy( Base::size(), a_Array.data(), a_Array.size() );
		return Result;
	}

	template < size_t LENGTH >
	inline void CopyTo( Array< ValueType, LENGTH >& a_Array ) const
	{
		memcpy( a_Array.data(), Base::data(), ( Base::size() < a_Array.size() ? Base::size() : a_Array.size() ) * sizeof( ValueType ) );
	}

	inline auto Data()
	{
		return Base::data();
	}

	inline auto Data() const
	{
		return Base::data();
	}

	template < size_t LENGTH >
	inline auto Difference( const Array< ValueType, LENGTH >& a_Array ) const
	{
		set< const ValueType*, LessPtr< ValueType > > SetA;
		set< const ValueType*, LessPtr< ValueType > > SetB;
		Array< ValueType > Result;

		for ( const auto& Entry : Underlying() )
		{
			SetA.insert( &Entry );
		}

		for ( const auto& Entry : a_Array.Underlying() )
		{
			SetB.insert( &Entry );
		}

		for ( auto Iterator = SetA.begin(); Iterator != SetA.end(); ++Iterator )
		{
			if ( SetB.insert( *Iterator ).second )
			{
				Result.push_back( **Iterator );
			}
		}

		return Result;
	}

	template < typename... T >
	inline auto& EmplaceAt( Iterator a_Where, T&&... a_Args )
	{
		return *( new ( &*a_Where ) ValueType( forward< T >( a_Args )... ) );
	}

	inline auto Empty() const
	{
		return Base::empty();
	}

	template < size_t LENGTH >
	inline bool Equals( const ReadOnlyArray< ValueType, LENGTH >& a_ReadOnlyArray ) const
	{
		if constexpr ( Base::size() != a_ReadOnlyArray.size() )
		{
			return false;
		}

		const char* Array1 = reinterpret_cast< const char* >( Base::data() );
		const char* Array2 = reinterpret_cast< const char* >( a_ReadOnlyArray.data() );

		for ( size_t i = 0; i < Base::size() * sizeof( ValueType ); ++i )
		{
			if ( Array1[ i ] != Array2[ i ] )
			{
				return false;
			}
		}

		return true;
	}

	template < size_t LENGTH >
	inline bool Equals( const Array< ValueType, LENGTH >& a_Array ) const
	{
		if constexpr ( Base::size() != a_Array.size() )
		{
			return false;
		}

		const char* Array1 = reinterpret_cast< const char* >( Base::data() );
		const char* Array2 = reinterpret_cast< const char* >( a_Array.data() );

		for ( size_t i = 0; i < Base::size() * sizeof( ValueType ); ++i )
		{
			if ( Array1[ i ] != Array2[ i ] )
			{
				return false;
			}
		}

		return true;
	}

	bool Exists( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( const auto& Entry : Underlying() )
		{
			if ( a_Predicate.Invoke( Entry ) )
			{
				return true;
			}
		}

		return false;
	}

	auto Find( const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = Begin(); Iterator < End(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Reference< ValueType >( *Iterator );
			}
		}

		return Reference< ValueType >();
	}

	auto Find( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = CBegin(); Iterator < CEnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return CReference< ValueType >( *Iterator );
			}
		}

		return CReference< ValueType >();
	}

	auto Find( Iterator a_Begin, Iterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				return Reference< ValueType >( *a_Begin );
			}
		}

		return Reference< ValueType >();
	}

	auto Find( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				return CReference< ValueType >( *a_Begin );
			}
		}

		return CReference< ValueType >();
	}

	auto FindIterator( const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = Begin(); Iterator < End(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator;
			}
		}

		return End();
	}

	auto FindIterator( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = CBegin(); Iterator < CEnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator;
			}
		}

		return CEnd();
	}

	auto FindIterator( Iterator a_Begin, Iterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				return a_Begin;
			}
		}

		return End();
	}

	auto FindIterator( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				return a_Begin;
			}
		}

		return CEnd();
	}

	inline auto FindAll( const Predicate< const ValueType& >& a_Predicate )
	{
		Array< Reference< ValueType > > Result;

		for ( const auto& Entry : Underlying() )
		{
			if ( a_Predicate.Invoke( Entry ) )
			{
				Result.EmplaceBack( Entry );
			}
		}

		return Result;
	}

	inline auto FindAll( const Predicate< const ValueType& >& a_Predicate ) const
	{
		Array< CReference< ValueType > > Result;

		for ( const auto& Entry : Underlying() )
		{
			if ( a_Predicate.Invoke( Entry ) )
			{
				Result.EmplaceBack( Entry );
			}
		}

		return Result;
	}

	inline auto FindAll( Iterator a_Begin, Iterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		Array< Reference< ValueType > > Result;

		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.EmplaceBack( *a_Begin );
			}
		}

		return Result;
	}

	inline auto FindAll( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		Array< CReference< ValueType > > Result;

		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.EmplaceBack( *a_Begin );
			}
		}

		return Result;
	}

	inline auto FindIteratorAll( const Predicate< const ValueType& >& a_Predicate )
	{
		Array< Iterator > Result;

		for ( auto Iterator = Begin(); Iterator < End(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				Result.EmplaceBack( Iterator );
			}
		}

		return Result;
	}

	inline auto FindIteratorAll( const Predicate< const ValueType& >& a_Predicate ) const
	{
		Array< CIterator > Result;

		for ( auto Iterator = CBegin(); Iterator < CEnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				Result.EmplaceBack( Iterator );
			}
		}

		return Result;
	}

	inline auto FindIteratorAll( Iterator a_Begin, Iterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		Array< Iterator > Result;

		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.EmplaceBack( a_Begin );
			}
		}

		return Result;
	}

	inline auto FindIteratorAll( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		Array< CIterator > Result;

		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.EmplaceBack( a_Begin );
			}
		}

		return Result;
	}

	inline auto FindLast( const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = RBegin(); Iterator < REnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Reference< ValueType >( *( Iterator.base() - 1 ) );
			}
		}

		return Reference< ValueType >();
	}

	inline auto FindLast( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = CRBegin(); Iterator < CREnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return CReference< ValueType >( *( Iterator.base() - 1 ) );
			}
		}

		return CReference< ValueType >();
	}

	inline auto FindLast( Iterator a_Begin, Iterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = a_End - 1; Iterator >= a_Begin(); --Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Reference< ValueType >( *Iterator );
			}
		}

		return Reference< ValueType >();
	}

	inline auto FindLast( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = a_End - 1; Iterator >= a_Begin(); --Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return CReference< ValueType >( *Iterator );
			}
		}

		return CReference< ValueType >();
	}

	inline auto FindIteratorLast( const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = RBegin(); Iterator < REnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator.base() - 1;
			}
		}

		return End();
	}

	inline auto FindIteratorLast( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = CRBegin(); Iterator < CREnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator.base() - 1;
			}
		}

		return CEnd();
	}

	inline auto FindIteratorLast( Iterator a_Begin, Iterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = a_End - 1; Iterator >= a_Begin(); --Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator;
			}
		}

		return End();
	}

	inline auto FindIteratorLast( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = a_End - 1; Iterator >= a_Begin(); --Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator;
			}
		}

		return CEnd();
	}

	inline void ForEach( const Action< ValueType& >& a_Action )
	{
		for ( auto& Entry : Underlying() )
		{
			a_Action.Invoke( Entry );
		}
	}

	inline void ForEach( const Action< const ValueType& >& a_Action ) const
	{
		for ( const auto& Entry : Underlying() )
		{
			a_Action.Invoke( Entry );
		}
	}

	inline void ForEach( Iterator a_Begin, Iterator a_End, const Action< ValueType& >& a_Action )
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			a_Action.Invoke( *Iterator );
		}
	}

	inline void ForEach( CIterator a_Begin, CIterator a_End, const Action< const ValueType& >& a_Action ) const
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			a_Action.Invoke( *Iterator );
		}
	}

	inline auto& Front()
	{
		return Base::front();
	}

	inline constexpr auto& Front() const
	{
		return Base::front();
	}

	template < size_t LENGTH >
	inline auto Intersection( const Array< ValueType, LENGTH >& a_Array ) const
	{
		set< const ValueType*, LessPtr< ValueType > > SetA;
		set< const ValueType*, LessPtr< ValueType > > SetB;
		Array< ValueType > Result;

		for ( const auto& Entry : Underlying() )
		{
			SetA.insert( &Entry );
		}

		for ( const auto& Entry : a_Array.Underlying() )
		{
			SetB.insert( &Entry );
		}

		bool SetAIsLargest = SetA.size() > SetB.size();
		decltype( SetA )& Smallest = !SetAIsLargest ? SetA : SetB;
		decltype( SetA )& Largest = SetAIsLargest ? SetA : SetB;
		auto Begin = !SetAIsLargest ? SetA.begin() : SetB.begin();
		auto End = !SetAIsLargest ? SetA.end() : SetB.end();

		for ( ; Begin != End; ++Begin )
		{
			if ( !Largest.insert( *Begin ).second )
			{
				Result.push_back( **Begin );
			}
		}

		return Result;
	}

	inline constexpr auto MaxSize() const
	{
		return Base::max_size();
	}

	inline void MemCopy( const void* a_Source, size_t a_Offset, size_t a_Length )
	{
		memcpy( reinterpret_cast< void* >( Base::data() ) + a_Offset, a_Source, a_Length );
	}

	inline void MemSet( char a_Value, size_t a_Offset, size_t a_Size )
	{
		memset( reinterpret_cast< void* >( Base::data() ) + a_Offset, static_cast< int >( a_Value ), a_Size );
	}

	inline void Reverse()
	{
		reverse( Base::begin(), Base::end() );
	}

	inline void Reverse( Iterator a_Begin, Iterator a_End )
	{
		reverse( a_Begin, a_End );
	}

	inline constexpr auto Size() const
	{
		return Base::size();
	}

	template < bool ASCENDING = true >
	inline void Sort()
	{
		sort( Base::begin(), Base::end(), []( const void* Left, const void* Right )
			   {
				   if constexpr ( ASCENDING )
				   {
					   return ( *reinterpret_cast< const ValueType* >( Left ) > *reinterpret_cast< const ValueType* >( Right ) );
				   }
				   else
				   {
					   return *reinterpret_cast< const ValueType* >( Left ) < *reinterpret_cast< const ValueType* >( Right );
				   }
			   } );
	}

	template < bool ASCENDING = true >
	inline void Sort( Iterator a_Begin, Iterator a_End )
	{
		sort( a_Begin, a_End, []( const void* Left, const void* Right )
			   {
				   if constexpr ( ASCENDING )
				   {
					   return *reinterpret_cast< const ValueType* >( Left ) > *reinterpret_cast< const ValueType* >( Right );
				   }
				   else
				   {
					   return *reinterpret_cast< const ValueType* >( Left ) < *reinterpret_cast< const ValueType* >( Right );
				   }
			   } );
	}

	inline void Sort( const Predicate< const ValueType&, const ValueType& >& a_Comparer )
	{
		sort( Base::begin(), Base::end(), a_Comparer );
	}

	inline void Sort( Iterator a_Begin, Iterator a_End, const Predicate< const ValueType&, const ValueType& >& a_Comparer )
	{
		sort( a_Begin, a_End, a_Comparer );
	}

	template < size_t OFFSET, size_t LENGTH = ( SIZE > OFFSET ? SIZE - OFFSET : 0 ) >
	inline auto& SubArray()
	{
		return *reinterpret_cast< Array< ValueType, LENGTH >* >( Base::data() + OFFSET );
	}

	template < size_t OFFSET, size_t LENGTH = ( SIZE > OFFSET ? SIZE - OFFSET : 0 ) >
	inline auto& SubArray() const
	{
		return *reinterpret_cast< const Array< ValueType, LENGTH >* >( Base::data() + OFFSET );
	}

	inline void Swap( Array< ValueType, SIZE >& a_Array )
	{
		return Base::swap( a_Array.Underlying() );
	}

	template < size_t LENGTH >
	inline auto Symmetry( const Array< ValueType, LENGTH >& a_Array ) const
	{
		set< const ValueType*, LessPtr< ValueType > > SetA;
		set< const ValueType*, LessPtr< ValueType > > SetB;
		Array< ValueType > Result;

		for ( const auto& Entry : Underlying() )
		{
			SetA.insert( &Entry );
		}

		for ( const auto& Entry : a_Array.Underlying() )
		{
			SetB.insert( &Entry );
		}

		auto IteratorA = SetA.begin();
		auto IteratorB = SetB.begin();

		while ( IteratorA != SetA.end() || IteratorB != SetB.end() )
		{
			while ( IteratorA != SetA.end() && ( IteratorB == SetB.end() || **( IteratorA ) < **IteratorB ) )
			{
				Result.push_back( **( IteratorA++ ) );
			}

			while ( IteratorB != SetB.end() && ( IteratorA == SetA.end() || **( IteratorB ) < **IteratorA ) )
			{
				Result.push_back( **( IteratorB++ ) );
			}

			if ( IteratorA != SetA.end() ) ++IteratorA;
			if ( IteratorB != SetB.end() ) ++IteratorB;
		}

		return Result;
	}

	inline bool TrueForAll( const Predicate< ValueType& >& a_Predicate )
	{
		for ( auto& Entry : Underlying() )
		{
			if ( !a_Predicate.Invoke( Entry ) )
			{
				return false;
			}
		}

		return true;
	}

	inline bool TrueForAll( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( const auto& Entry : Underlying() )
		{
			if ( !a_Predicate.Invoke( Entry ) )
			{
				return false;
			}
		}

		return true;
	}

	inline bool TrueForAll( Iterator a_Begin, Iterator a_End, const Predicate< ValueType& >& a_Predicate )
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( !a_Predicate.Invoke( *a_Begin ) )
			{
				return false;
			}
		}

		return true;
	}

	inline bool TrueForAll( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( !a_Predicate.Invoke( *a_Begin ) )
			{
				return false;
			}
		}

		return true;
	}

	inline auto& Underlying()
	{
		return *static_cast< Base* >( this );
	}

	inline const auto& Underlying() const
	{
		return *static_cast< const Base* >( this );
	}

	template < size_t LENGTH >
	inline auto Union( const Array< ValueType, LENGTH >& a_Array ) const
	{
		set< const ValueType*, LessPtr< ValueType > > UnionSet;
		Array< ValueType > Result;

		for ( const auto& Entry : Underlying() )
		{
			UnionSet.insert( &Entry );
		}

		for ( const auto& Entry : a_Array.Underlying() )
		{
			UnionSet.insert( &Entry );
		}
		
		for ( const auto& Entry : UnionSet )
		{
			Result.push_back( *Entry );
		}

		return Result;
	}

	inline auto Zero()
	{
		return static_cast< ValueType* >( memset( Base::data(), 0, sizeof( Base ) ) );
	}

	inline auto Zero( size_t a_Position )
	{
		return static_cast< ValueType* >( memset( Base::data() + a_Position, 0, sizeof( ValueType ) ) );
	}

	inline auto Zero( size_t a_Start, size_t a_Length )
	{
		return static_cast< ValueType* >( memset( Base::data() + a_Start, 0, sizeof( ValueType ) * a_Length ) );
	}

	#pragma region Operator
	inline auto& operator[]( size_t a_Position )
	{
		return Base::operator[]( a_Position );
	}

	inline auto& operator[]( size_t a_Position ) const
	{
		return Base::operator[]( a_Position );
	}
	#pragma endregion

	#pragma region Iterator
	inline Iterator Begin()
	{
		return Base::begin();
	}

	inline CIterator Begin() const
	{
		return Base::begin();
	}

	inline CIterator CBegin()
	{
		return Base::cbegin();
	}

	inline CIterator CBegin() const
	{
		return Base::cbegin();
	}

	inline RIterator RBegin()
	{
		return Base::rbegin();
	}

	inline CRIterator RBegin() const
	{
		return Base::rbegin();
	}

	inline CRIterator CRBegin()
	{
		return Base::crbegin();
	}

	inline CRIterator CRBegin() const
	{
		return Base::crbegin();
	}

	inline Iterator End()
	{
		return Base::end();
	}

	inline CIterator End() const
	{
		return Base::end();
	}

	inline CIterator CEnd()
	{
		return Base::cend();
	}

	inline CIterator CEnd() const
	{
		return Base::cend();
	}

	inline RIterator REnd()
	{
		return Base::rend();
	}

	inline CRIterator REnd() const
	{
		return Base::rend();
	}

	inline CRIterator CREnd()
	{
		return Base::crend();
	}

	inline CRIterator CREnd() const
	{
		return Base::crend();
	}
	#pragma endregion

private:

	template < class, size_t > friend class Array;

};

template < typename ValueType >
class Array< ValueType, size_t( -1 ) > : private vector< ValueType >
{

	template < typename T >
	struct LessPtr
	{
		bool operator()( const T* Left, const T* Right ) const
		{
			return *Left < *Right;
		}
	};

public:

	using Base = vector< ValueType >;
	using Iterator = vector< ValueType >::iterator;
	using CIterator = vector< ValueType >::const_iterator;
	using RIterator = vector< ValueType >::reverse_iterator;
	using CRIterator = vector< ValueType >::const_reverse_iterator;

	Array() = default;

	template < size_t LENGTH >
	Array( const Array< ValueType, LENGTH >& a_Array )
	{
		a_Array.CopyTo( *this );
	}

	Array( const InitializerList< ValueType >& a_InitializerList )
	{
		Base::resize( a_InitializerList.size() );
		memcpy( Base::data(), a_InitializerList.begin(), a_InitializerList.size() * sizeof( ValueType ) );
	}

	Array( const InitializerList< ValueType >&& a_InitializerList )
	{
		Base::resize( a_InitializerList.size() );
		memcpy( Base::data(), a_InitializerList.begin(), a_InitializerList.size() * sizeof( ValueType ) );
	}

	inline void Assign( const ValueType& a_Value )
	{
		Base::assign( a_Value );
	}

	inline void Assign( size_t a_Position, size_t a_Length, const ValueType& a_Value )
	{
		auto Begin = Base::data() + a_Position;
		auto End = Begin + a_Length;

		for ( ; Begin < End; ++Begin )
		{
			*Begin = a_Value;
		}
	}

	inline void Assign( Iterator a_Begin, Iterator a_End, const ValueType& a_Value )
	{
		while ( a_Begin < a_End )
		{
			*a_Begin++ = a_Value;
		}
	}
	
	inline auto& At( size_t a_Position )
	{
		return Base::at( a_Position );
	}

	inline auto& At( size_t a_Position ) const
	{
		return Base::at( a_Position );
	}

	inline auto& Back()
	{
		return Base::back();
	}

	inline auto& Back() const
	{
		return Base::back();
	}

	inline auto Capacity() const
	{
		return Base::capacity();
	}

	inline void Clear()
	{
		Base::clear();
	}

	template < size_t LENGTH >
	inline auto Combine( const Array< ValueType, LENGTH >& a_Array ) const
	{
		Array< ValueType > Result;
		Result.resize( Base::size() + a_Array.size() );
		Result.MemCopy( 0, Base::data(), Base::size() );
		Result.MemCopy( Base::size(), a_Array.data(), a_Array.size() );
		return Result;
	}

	template < size_t LENGTH >
	inline void CopyTo( Array< ValueType, LENGTH >& a_Array ) const
	{
		memcpy( a_Array.data(), Base::data(), ( Base::size() < a_Array.size() ? Base::size() : a_Array.size() ) * sizeof( ValueType ) );
	}

	inline auto Data()
	{
		return Base::data();
	}

	inline auto Data() const
	{
		return Base::data();
	}

	template < size_t LENGTH >
	inline auto Difference( const Array< ValueType, LENGTH >& a_Array ) const
	{
		set< const ValueType*, LessPtr< ValueType > > SetA;
		set< const ValueType*, LessPtr< ValueType > > SetB;
		Array< ValueType > Result;

		for ( const auto& Entry : Underlying() )
		{
			SetA.insert( &Entry );
		}

		for ( const auto& Entry : a_Array.Underlying() )
		{
			SetB.insert( &Entry );
		}

		for ( auto Iterator = SetA.begin(); Iterator != SetA.end(); ++Iterator )
		{
			if ( SetB.insert( *Iterator ).second )
			{
				Result.push_back( **Iterator );
			}
		}

		return Result;
	}

	template < typename... T >
	inline Iterator Emplace( CIterator a_Where, T&&... a_Args )
	{
		return Base::emplace( a_Where, forward< T >( a_Args )... );
	}

	template < typename... T >
	inline void EmplaceAt( Iterator a_Where, T&&... a_Args )
	{
		new ( &*a_Where ) ValueType( forward< T >( a_Args )... );
	}

	template < typename... T >
	inline void EmplaceBack( T&&... a_Args )
	{
		Base::emplace_back( forward< T >( a_Args )... );
	}

	inline auto Empty() const
	{
		return Base::empty();
	}

	inline Iterator Erase( CIterator a_Where )
	{
		return Base::erase( a_Where );
	}

	template < size_t LENGTH >
	inline bool Equals( const ReadOnlyArray< ValueType, LENGTH >& a_ReadOnlyArray ) const
	{
		if ( Base::size() != a_ReadOnlyArray.size() )
		{
			return false;
		}

		const char* Array1 = reinterpret_cast< const char* >( Base::data() );
		const char* Array2 = reinterpret_cast< const char* >( a_ReadOnlyArray.data() );

		for ( size_t i = 0; i < Base::size() * sizeof( ValueType ); ++i )
		{
			if ( Array1[ i ] != Array2[ i ] )
			{
				return false;
			}
		}

		return true;
	}

	template < size_t LENGTH >
	inline bool Equals( const Array< ValueType, LENGTH >& a_Array ) const
	{
		if ( Base::size() != a_Array.size() )
		{
			return false;
		}

		const char* Array1 = reinterpret_cast< const char* >( Base::data() );
		const char* Array2 = reinterpret_cast< const char* >( a_Array.data() );

		for ( size_t i = 0; i < Base::size() * sizeof( ValueType ); ++i )
		{
			if ( Array1[ i ] != Array2[ i ] )
			{
				return false;
			}
		}

		return true;
	}

	bool Exists( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( const auto& Entry : Underlying() )
		{
			if ( a_Predicate.Invoke( Entry ) )
			{
				return true;
			}
		}

		return false;
	}

	auto Find( const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = Begin(); Iterator < End(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Reference< ValueType >( *Iterator );
			}
		}

		return Reference< ValueType >();
	}

	auto Find( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = CBegin(); Iterator < CEnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return CReference< ValueType >( *Iterator );
			}
		}

		return CReference< ValueType >();
	}

	auto Find( Iterator a_Begin, Iterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				return Reference< ValueType >( *a_Begin );
			}
		}

		return Reference< ValueType >();
	}

	auto Find( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				return CReference< ValueType >( *a_Begin );
			}
		}

		return CReference< ValueType >();
	}

	auto FindIterator( const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = Begin(); Iterator < End(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator;
			}
		}

		return End();
	}

	auto FindIterator( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = CBegin(); Iterator < CEnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator;
			}
		}

		return CEnd();
	}

	auto FindIterator( Iterator a_Begin, Iterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				return a_Begin;
			}
		}

		return End();
	}

	auto FindIterator( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				return a_Begin;
			}
		}

		return CEnd();
	}

	inline auto FindAll( const Predicate< const ValueType& >& a_Predicate )
	{
		Array< Reference< ValueType > > Result;

		for ( const auto& Entry : Underlying() )
		{
			if ( a_Predicate.Invoke( Entry ) )
			{
				Result.EmplaceBack( Entry );
			}
		}

		return Result;
	}

	inline auto FindAll( const Predicate< const ValueType& >& a_Predicate ) const
	{
		Array< CReference< ValueType > > Result;

		for ( const auto& Entry : Underlying() )
		{
			if ( a_Predicate.Invoke( Entry ) )
			{
				Result.EmplaceBack( Entry );
			}
		}

		return Result;
	}

	inline auto FindAll( Iterator a_Begin, Iterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		Array< Reference< ValueType > > Result;

		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.EmplaceBack( *a_Begin );
			}
		}

		return Result;
	}

	inline auto FindAll( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		Array< CReference< ValueType > > Result;

		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.EmplaceBack( *a_Begin );
			}
		}

		return Result;
	}

	inline auto FindIteratorAll( const Predicate< const ValueType& >& a_Predicate )
	{
		Array< Iterator > Result;

		for ( auto Iterator = Begin(); Iterator < End(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				Result.EmplaceBack( Iterator );
			}
		}

		return Result;
	}

	inline auto FindIteratorAll( const Predicate< const ValueType& >& a_Predicate ) const
	{
		Array< CIterator > Result;

		for ( auto Iterator = CBegin(); Iterator < CEnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				Result.EmplaceBack( Iterator );
			}
		}

		return Result;
	}

	inline auto FindIteratorAll( Iterator a_Begin, Iterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		Array< Iterator > Result;

		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.EmplaceBack( a_Begin );
			}
		}

		return Result;
	}

	inline auto FindIteratorAll( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		Array< CIterator > Result;

		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.EmplaceBack( a_Begin );
			}
		}

		return Result;
	}

	inline auto FindLast( const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = RBegin(); Iterator < REnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Reference< ValueType >( *( Iterator.base() - 1 ) );
			}
		}

		return Reference< ValueType >();
	}

	inline auto FindLast( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = CRBegin(); Iterator < CREnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return CReference< ValueType >( *( Iterator.base() - 1 ) );
			}
		}

		return CReference< ValueType >();
	}

	inline auto FindLast( Iterator a_Begin, Iterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = a_End - 1; Iterator >= a_Begin(); --Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Reference< ValueType >( *Iterator );
			}
		}

		return Reference< ValueType >();
	}

	inline auto FindLast( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = a_End - 1; Iterator >= a_Begin(); --Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return CReference< ValueType >( *Iterator );
			}
		}

		return CReference< ValueType >();
	}

	inline auto FindIteratorLast( const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = RBegin(); Iterator < REnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator.base() - 1;
			}
		}

		return End();
	}

	inline auto FindIteratorLast( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = CRBegin(); Iterator < CREnd(); ++Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator.base() - 1;
			}
		}

		return CEnd();
	}

	inline auto FindIteratorLast( Iterator a_Begin, Iterator a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Iterator = a_End - 1; Iterator >= a_Begin(); --Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator;
			}
		}

		return End();
	}

	inline auto FindIteratorLast( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Iterator = a_End - 1; Iterator >= a_Begin(); --Iterator )
		{
			if ( a_Predicate.Invoke( *Iterator ) )
			{
				return Iterator;
			}
		}

		return CEnd();
	}

	inline void ForEach( const Action< ValueType& >& a_Action )
	{
		for ( auto& Entry : Underlying() )
		{
			a_Action.Invoke( Entry );
		}
	}

	inline void ForEach( const Action< const ValueType& >& a_Action ) const
	{
		for ( const auto& Entry : Underlying() )
		{
			a_Action.Invoke( Entry );
		}
	}

	inline void ForEach( Iterator a_Begin, Iterator a_End, const Action< ValueType& >& a_Action )
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			a_Action.Invoke( *Iterator );
		}
	}

	inline void ForEach( CIterator a_Begin, CIterator a_End, const Action< const ValueType& >& a_Action ) const
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			a_Action.Invoke( *Iterator );
		}
	}

	inline auto& Front()
	{
		return Base::front();
	}

	inline const auto& Front() const
	{
		return Base::front();
	}

	inline Iterator Insert( CIterator a_Where, InitializerList< ValueType >&& a_InitializerList )
	{
		return Base::insert( a_Where, a_InitializerList );
	}

	inline Iterator Insert( CIterator a_Where, const InitializerList< ValueType >& a_InitializerList )
	{
		return Base::insert( a_Where, a_InitializerList );
	}

	template < typename Iter >
	inline Iterator Insert( CIterator a_Where, Iter a_Start, Iter a_End )
	{
		return Base::insert( a_Where, a_Start, a_End );
	}

	inline Iterator Insert( CIterator a_Where, size_t a_Count, ValueType&& a_Value )
	{
		return Base::insert( a_Where, a_Count, a_Value );
	}

	inline Iterator Insert( CIterator a_Where, const ValueType& a_Value )
	{
		return Base::insert( a_Where, a_Value );
	}

	inline Iterator Insert( CIterator a_Where, ValueType&& a_Value )
	{
		return Base::insert( a_Where, a_Value );
	}

	template < size_t LENGTH >
	inline auto Intersection( const Array< ValueType, LENGTH >& a_Array ) const
	{
		set< const ValueType*, LessPtr< ValueType > > SetA;
		set< const ValueType*, LessPtr< ValueType > > SetB;
		Array< ValueType > Result;

		for ( const auto& Entry : Underlying() )
		{
			SetA.insert( &Entry );
		}

		for ( const auto& Entry : a_Array.Underlying() )
		{
			SetB.insert( &Entry );
		}

		bool SetAIsLargest = SetA.size() > SetB.size();
		decltype( SetA )& Smallest = !SetAIsLargest ? SetA : SetB;
		decltype( SetA )& Largest = SetAIsLargest ? SetA : SetB;
		auto Begin = !SetAIsLargest ? SetA.begin() : SetB.begin();
		auto End = !SetAIsLargest ? SetA.end() : SetB.end();

		for ( ; Begin != End; ++Begin )
		{
			if ( !Largest.insert( *Begin ).second )
			{
				Result.push_back( **Begin );
			}
		}

		return Result;
	}

	inline auto MaxSize() const
	{
		return Base::max_size();
	}

	inline void MemCopy( const void* a_Source, size_t a_Offset, size_t a_Length )
	{
		memcpy( reinterpret_cast< void* >( Base::data() ) + a_Offset, a_Source, a_Length );
	}

	inline void MemSet( char a_Value, size_t a_Offset, size_t a_Size )
	{
		memset( reinterpret_cast< void* >( Base::data() ) + a_Offset, static_cast< int >( a_Value ), a_Size );
	}

	inline void PopBack()
	{
		Base::pop_back();
	}

	inline void PushBack( ValueType&& a_Value )
	{
		Base::push_back( a_Value );
	}

	inline void PushBack( const ValueType& a_Value )
	{
		Base::push_back( a_Value );
	}

	inline void Reserve( size_t a_NewCapacity )
	{
		Base::reserve( a_NewCapacity );
	}

	inline void Resize( size_t a_NewSize )
	{
		Base::resize( a_NewSize );
	}

	inline void Reverse()
	{
		reverse( Base::begin(), Base::end() );
	}

	inline void Reverse( Iterator a_Begin, Iterator a_End )
	{
		reverse( a_Begin, a_End );
	}

	inline void ShrinkToFit()
	{
		Base::shrink_to_fit();
	}

	inline auto Size() const
	{
		return Base::size();
	}

	template < bool ASCENDING = true >
	inline void Sort()
	{
		sort( Base::begin(), Base::end(), []( const void* Left, const void* Right )
			  {
				  if constexpr ( ASCENDING )
				  {
					  return ( *reinterpret_cast< const ValueType* >( Left ) > * reinterpret_cast< const ValueType* >( Right ) );
				  }
				  else
				  {
					  return *reinterpret_cast< const ValueType* >( Left ) < *reinterpret_cast< const ValueType* >( Right );
				  }
			  } );
	}

	template < bool ASCENDING = true >
	inline void Sort( Iterator a_Begin, Iterator a_End )
	{
		sort( a_Begin, a_End, []( const void* Left, const void* Right )
			  {
				  if constexpr ( ASCENDING )
				  {
					  return *reinterpret_cast< const ValueType* >( Left ) > * reinterpret_cast< const ValueType* >( Right );
				  }
				  else
				  {
					  return *reinterpret_cast< const ValueType* >( Left ) < *reinterpret_cast< const ValueType* >( Right );
				  }
			  } );
	}

	inline void Sort( const Predicate< const ValueType&, const ValueType& >& a_Comparer )
	{
		sort( Base::begin(), Base::end(), a_Comparer );
	}

	inline void Sort( Iterator a_Begin, Iterator a_End, const Predicate< const ValueType&, const ValueType& >& a_Comparer )
	{
		sort( a_Begin, a_End, a_Comparer );
	}

	template < size_t OFFSET, size_t LENGTH >
	inline auto& SubArray()
	{
		return *reinterpret_cast< Array< ValueType, LENGTH >* >( Base::data() + OFFSET );
	}

	template < size_t OFFSET, size_t LENGTH >
	inline const auto& SubArray() const
	{
		return *reinterpret_cast< const Array< ValueType, LENGTH >* >( Base::data() + OFFSET );
	}

	inline void Swap( Array< ValueType >& a_Array )
	{
		return Base::swap( a_Array.Underlying() );
	}

	template < size_t LENGTH >
	inline auto Symmetry( const Array< ValueType, LENGTH >& a_Array ) const
	{
		set< const ValueType*, LessPtr< ValueType > > SetA;
		set< const ValueType*, LessPtr< ValueType > > SetB;
		Array< ValueType > Result;

		for ( const auto& Entry : Underlying() )
		{
			SetA.insert( &Entry );
		}

		for ( const auto& Entry : a_Array.Underlying() )
		{
			SetB.insert( &Entry );
		}

		auto IteratorA = SetA.begin();
		auto IteratorB = SetB.begin();

		while ( IteratorA != SetA.end() || IteratorB != SetB.end() )
		{
			while ( IteratorA != SetA.end() && ( IteratorB == SetB.end() || **( IteratorA ) < **IteratorB ) )
			{
				Result.push_back( **( IteratorA++ ) );
			}

			while ( IteratorB != SetB.end() && ( IteratorA == SetA.end() || **( IteratorB ) < **IteratorA ) )
			{
				Result.push_back( **( IteratorB++ ) );
			}

			if ( IteratorA != SetA.end() ) ++IteratorA;
			if ( IteratorB != SetB.end() ) ++IteratorB;
		}

		return Result;
	}

	inline bool TrueForAll( const Predicate< ValueType& >& a_Predicate )
	{
		for ( auto& Entry : Underlying() )
		{
			if ( !a_Predicate.Invoke( Entry ) )
			{
				return false;
			}
		}

		return true;
	}

	inline bool TrueForAll( const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( const auto& Entry : Underlying() )
		{
			if ( !a_Predicate.Invoke( Entry ) )
			{
				return false;
			}
		}

		return true;
	}

	inline bool TrueForAll( Iterator a_Begin, Iterator a_End, const Predicate< ValueType& >& a_Predicate )
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( !a_Predicate.Invoke( *a_Begin ) )
			{
				return false;
			}
		}

		return true;
	}

	inline bool TrueForAll( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( !a_Predicate.Invoke( *a_Begin ) )
			{
				return false;
			}
		}

		return true;
	}

	inline auto& Underlying()
	{
		return *static_cast< Base* >( this );
	}

	inline const auto& Underlying() const
	{
		return *static_cast< const Base* >( this );
	}

	template < size_t LENGTH >
	inline auto Union( const Array< ValueType, LENGTH >& a_Array ) const
	{
		set< const ValueType*, LessPtr< ValueType > > UnionSet;
		Array< ValueType > Result;

		for ( const auto& Entry : Underlying() )
		{
			UnionSet.insert( &Entry );
		}

		for ( const auto& Entry : a_Array.Underlying() )
		{
			UnionSet.insert( &Entry );
		}

		for ( const auto& Entry : UnionSet )
		{
			Result.push_back( *Entry );
		}

		return Result;
	}

	inline auto Zero()
	{
		return static_cast< ValueType* >( memset( Base::data(), 0, Base::size() * sizeof( ValueType ) ) );
	}

	inline auto Zero( size_t a_Position )
	{
		return static_cast< ValueType* >( memset( Base::data() + a_Position, 0, sizeof( ValueType ) ) );
	}

	inline auto Zero( size_t a_Start, size_t a_Length )
	{
		return static_cast< ValueType* >( memset( Base::data() + a_Start, 0, sizeof( ValueType ) * a_Length ) );
	}

	#pragma region Operator
	auto& operator=( Base&& a_Array )
	{
		return Base::operator=( a_Array );
	}

	auto& operator=( const Base& a_Array )
	{
		return Base::operator=( a_Array );
	}

	auto& operator=( InitializerList< ValueType >&& a_InitializerList )
	{
		return Base::operator=( a_InitializerList );
	}

	auto& operator=( const InitializerList< ValueType >& a_InitializerList )
	{
		return Base::operator=( a_InitializerList );
	}

	inline auto& operator[]( size_t a_Position )
	{
		return Base::operator[]( a_Position );
	}

	inline auto& operator[]( size_t a_Position ) const
	{
		return Base::operator[]( a_Position );
	}
	#pragma endregion

	#pragma region Iterator
	inline Iterator Begin()
	{
		return Base::begin();
	}

	inline Iterator Begin() const
	{
		return Base::begin();
	}

	inline RIterator RBegin()
	{
		return Base::rbegin();
	}

	inline RIterator RBegin() const
	{
		return Base::rbegin();
	}

	inline CIterator CBegin()
	{
		return Base::cbegin();
	}

	inline CIterator CBegin() const
	{
		return Base::cbegin();
	}

	inline CRIterator CRBegin()
	{
		return Base::crbegin();
	}

	inline CRIterator CRBegin() const
	{
		return Base::crbegin();
	}

	inline Iterator End()
	{
		return Base::end();
	}

	inline Iterator End() const
	{
		return Base::end();
	}

	inline RIterator REnd()
	{
		return Base::rend();
	}

	inline RIterator REnd() const
	{
		return Base::rend();
	}

	inline CIterator CEnd()
	{
		return Base::cend();
	}

	inline CIterator CEnd() const
	{
		return Base::cend();
	}

	inline CRIterator CREnd()
	{
		return Base::crend();
	}

	inline CRIterator CREnd() const
	{
		return Base::crend();
	}
	#pragma endregion

private:

	template < class, size_t > friend class Array;

};