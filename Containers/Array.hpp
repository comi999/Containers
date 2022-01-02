#pragma once
#include <array>
#include <vector>
#include <algorithm>

#include "Invoker.hpp"
#include "Reference.hpp"
#include "Enumerable.hpp"
#include "ContainerTraits.hpp"

using namespace std;

template < typename T >
class RefArray;

template < typename T >
class ReadOnlyRefArray;

template < typename T, size_t SIZE >
class Array;

template < typename T, size_t SIZE >
class ReadOnlyArray;

template < typename ValueType >
using InitializerList = initializer_list< ValueType >;

template < typename ValueType >
class RefArray : private vector< Reference< ValueType > >
{
	using Base = vector< Reference< ValueType > >;

public:

	class Iterator : public Base::iterator
	{
	public:

		using iterator_category = random_access_iterator_tag;
		using difference_type   = ptrdiff_t;
		using value_type        = ValueType;
		using pointer           = value_type*;
		using reference         = value_type&;

	private:

		Iterator( Reference< ValueType >* a_Value )
			: m_Value( a_Value )
		{ }

	public:

		Iterator( const Iterator& a_Iterator )
			: m_Value( a_Iterator.m_Value )
		{ }

		Iterator( const Iterator&& a_Iterator )
			: m_Value( a_Iterator.m_Value )
		{ }

		inline Iterator& operator++()
		{
			++m_Value;
			return *this;
		}

		inline Iterator operator++( int )
		{
			return Iterator( m_Value++ );
		}

		inline Iterator& operator--( )
		{
			--m_Value;
			return *this;
		}

		inline Iterator operator--( int )
		{
			return Iterator( m_Value-- );
		}

		inline Iterator& operator+=( int a_Places )
		{
			m_Value += a_Places;
			return *this;
		}

		inline Iterator& operator-=( int a_Places )
		{
			m_Value -= a_Places;
			return *this;
		}

		inline Iterator operator+( int a_Places ) const
		{
			return Iterator( m_Value + a_Places );
		}

		inline Iterator operator-( int a_Places ) const
		{
			return Iterator( m_Value - a_Places );
		}

		inline int operator-( const Iterator& a_Iterator ) const
		{
			return m_Value - a_Iterator.m_Value;
		}

		inline Iterator& operator=( const Iterator& a_Iterator ) const
		{
			m_Value = a_Iterator.m_Value;
			return *this;
		}

		inline ValueType& operator*()
		{
			return m_Value->Get();
		}

		inline const ValueType& operator*() const
		{
			return m_Value->Get();
		}

		inline ValueType* operator->()
		{
			return m_Value->Get();
		}

		inline const ValueType* operator->() const
		{
			return m_Value->Get();
		}

		inline bool operator==( const Iterator& a_Iterator ) const
		{
			return m_Value == a_Iterator.m_Value;
		}

		inline bool operator!=( const Iterator& a_Iterator ) const
		{
			return m_Value != a_Iterator.m_Value;
		}

		inline bool operator>( const Iterator& a_Iterator ) const
		{
			return m_Value > a_Iterator.m_Value;
		}

		inline bool operator>=( const Iterator& a_Iterator ) const
		{
			return m_Value >= a_Iterator.m_Value;
		}

		inline bool operator<( const Iterator& a_Iterator ) const
		{
			return m_Value < a_Iterator.m_Value;
		}

		inline bool operator<=( const Iterator& a_Iterator ) const
		{
			return m_Value <= a_Iterator.m_Value;
		}

	private:

		Reference< ValueType >* m_Value;

	};
	
	class CIterator
	{

	};

	template < size_t LENGTH >
	RefArray( ValueType( *a_Array )[ LENGTH ] )
	{

	}

	RefArray( InitializerList< ValueType >& a_InitializerList )
	{
	}

	RefArray( InitializerList< ValueType >&& a_InitializerList )
	{

	}

	template < typename Iter, typename = ContainerTraits::EnableIfIterType< Iter, ValueType > >
	RefArray( Iter a_Begin, Iter a_End )
	{
		if constexpr ( _Is_random_iter_v< Iter > )
		{
			Base::reserve( a_End - a_Begin );
		}

		for ( ; a_Begin != a_End; ++a_Begin )
		{
			Base::emplace_back( *a_Begin );
		}
	}

	template < typename Container, typename = ContainerTraits::EnableIfContainer< Container, ValueType > >
	RefArray( Container& a_Container )
	{
		Base::reserve( ContainerTraits::Size( a_Container ) );

		auto Beg = ContainerTraits::Begin( a_Container );
		auto End = ContainerTraits::End( a_Container );

		for ( ; Beg != End; ++Beg  )
		{
			Base::emplace_back( *Beg );
		}
	}

	template < size_t LENGTH >
	inline void Append( ValueType( *a_Array )[ LENGTH ] )
	{

	}

	template < typename Container, typename = ContainerTraits::EnableIfContainer< Container, ValueType > >
	inline void Append( Container& a_Container )
	{

	}

	template < typename Iter, typename = ContainerTraits::EnableIfIterType< Iter, ValueType > >
	inline void Append( Iter a_Begin, const Iter& a_End )
	{

	}

	inline auto AsEnumerable()
	{

	}

	inline auto AsEnumerable() const
	{

	}

	inline void Assign( const ValueType& a_ValueType )
	{
		for ( auto Beg = Base::begin(); Beg != Base::end(); ++Beg )
		{
			Beg->Get() = a_ValueType;
		}
	}

	inline void Assign( Iterator a_Begin, const Iterator& a_End, const ValueType& a_ValueType )
	{
		for ( ; a_Begin != a_End; ++a_Begin )
		{
			a_Begin->Get() = a_ValueType;
		}
	}

	inline auto& At( size_t a_Position )
	{
		return Base::at( a_Position ).Get();
	}

	inline const auto& At( size_t a_Position ) const
	{
		return Base::at( a_Position ).Get();
	}

	inline auto& Back()
	{
		return Base::back().Get();
	}

	inline const auto& Back() const
	{
		return Base::back().Get();
	}

	inline auto Capacity() const
	{
		return Base::capacity();
	}

	inline void Clear()
	{
		Base::clear();
	}

	inline auto Combine( RefArray< ValueType >& a_RefArray )
	{
		RefArray< ValueType > Result;
		Result.reserve( Base::size() + a_RefArray.size() );

		for ( auto Beg = Base::begin(); Beg != Base::end(); ++Beg )
		{
			Result.push_back( *Beg );
		}

		for ( auto Beg = a_RefArray.begin(); Beg != a_RefArray.end(); ++Beg )
		{
			Result.push_back( *Beg );
		}

		return Result;
	}

	inline void CopyTo( RefArray< ValueType >& a_RefArray )
	{

	}

	inline auto Data()
	{
		return Base::data();
	}

	inline auto Data() const
	{
		return Base::data();
	}
	
	inline auto Difference( RefArray< ValueType >& a_RefArray )
	{

	}

	inline bool Empty() const
	{
		return Base::empty();
	}

	inline bool Equals( const RefArray< ValueType >& a_RefArray ) const
	{

	}

	inline bool Exists( const Predicate< const ValueType& >& a_Predicate ) const
	{

	}

	inline auto Find( const ValueType& a_Value )
	{
		// return Iterator
	}

	inline auto Find( const ValueType& a_Value ) const
	{
		// return CIterator
	}

	inline auto Find( const Predicate< const ValueType& >& a_Predicate )
	{
		// return Iterator
	}

	inline auto Find( const Predicate< const ValueType& >& a_Predicate ) const
	{
		// return CIterator
	}

	inline auto Find( Iterator a_Begin, const Iterator& a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		// return Iterator
	}

	inline auto Find( CIterator a_Begin, const CIterator& a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		// return CIterator
	}

	inline auto FindAll( const Predicate< const ValueType& >& a_Predicate )
	{

	}

	inline auto FindAll( const Predicate< const ValueType& >& a_Predicate ) const
	{

	}

	inline auto FindAll( Iterator a_Begin, const Iterator& a_End, const Predicate< const ValueType& >& a_Predicate )
	{

	}

	inline auto FindAll( CIterator a_Begin, const CIterator& a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{

	}

	inline auto FindLast( const ValueType& a_Value )
	{

	}

	inline auto FindLast( const ValueType& a_Value ) const
	{

	}

	inline auto FindLast( const Predicate< const ValueType& >& a_Predicate )
	{

	}

	inline auto FindLast( const Predicate< const ValueType& >& a_Predicate ) const
	{

	}

	inline auto FindLast( Iterator a_Begin, const Iterator& a_End, const Predicate< const ValueType& >& a_Predicate )
	{

	}

	inline auto FindLast( Iterator a_Begin, const Iterator& a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{

	}

	inline void ForEach( const Action< ValueType& >& a_Action )
	{

	}

	inline void ForEach( const Action< const ValueType& >& a_Action ) const
	{

	}

	inline void ForEach( Iterator a_Begin, const Iterator& a_End, const Action< ValueType& >& a_Action )
	{

	}

	inline void ForEach( CIterator a_Begin, const CIterator& a_End, const Action< const ValueType& >& a_Action ) const
	{

	}

	inline auto& Front()
	{

	}

	inline const auto& Front() const
	{

	}

	inline size_t IndexOf( const ValueType& a_Value ) const
	{

	}

	inline void Insert( const Iterator& a_Where, ValueType& a_Value )
	{

	}

	inline auto Intersection( const RefArray< ValueType >& a_RefArray ) const
	{

	}
};

template < typename ValueType >
class ReadOnlyRefArray : private vector< CReference< ValueType > >
{

};

template < typename ValueType, size_t SIZE >
class ReadOnlyArray : private array< ValueType, SIZE >
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
	using CIterator = typename array< ValueType, SIZE >::const_iterator;
	using CRIterator = typename array< ValueType, SIZE >::const_reverse_iterator;

	template < size_t LENGTH >
	ReadOnlyArray( ValueType( *a_Array )[ LENGTH ] )
	{
		size_t Size = LENGTH > SIZE ? SIZE : LENGTH;
		size_t Index = 0;

		for ( auto Beg = Base::begin(); Index < Size; ++Index, ++Beg )
		{
			*Beg = a_Array[ Index ];
		}
	}

	template < typename Container >
	ReadOnlyArray( Container& a_Container )
	{
		typename Base::iterator BegA( Base::begin() );
		typename Base::iterator EndA( Base::end() );
		auto BegB = ContainerTraits::Begin( a_Container );
		auto EndB = ContainerTraits::End( a_Container );
		
		for ( ; BegA != EndA && BegB != EndB; ++BegA, ++BegB )
		{
			*BegA = *BegB;
		}
	}

	ReadOnlyArray( const InitializerList< ValueType >& a_InitializerList )
	{
		size_t Size = a_InitializerList.size() < SIZE ? a_InitializerList.size() : SIZE;
		auto BegA = Base::begin();
		auto BegB = a_InitializerList.begin();

		for ( size_t Index = 0; Index < Size; ++Index, ++BegA, ++BegB )
		{
			*BegA = *BegB;
		}
	}

	ReadOnlyArray( const InitializerList< ValueType >&& a_InitializerList )
	{
		size_t Size = a_InitializerList.size() < SIZE ? a_InitializerList.size() : SIZE;
		auto BegA = Base::begin();
		auto BegB = a_InitializerList.begin();

		for ( size_t Index = 0; Index < Size; ++Index, ++BegA, ++BegB )
		{
			*BegA = *BegB;
		}
	}
	
	template < typename Iter >
	ReadOnlyArray( Iter a_Begin, const Iter& a_End )
	{
		typename Base::iterator Beg( Base::begin() );
		typename Base::iterator End( Base::end() );

		for ( ; a_Begin != a_End && Beg != End; ++a_Begin, ++Beg )
		{
			*Beg = *a_Begin;
		}
	}

	inline auto AsReadOnlyEnumerable() const
	{
		return ReadOnlyEnumerable< ValueType >( *this );
	}

	inline auto AsReadOnlyRefArray() const
	{
		return ReadOnlyRefArray( *this );
	}

	inline constexpr auto& At( size_t a_Position ) const
	{
		return Base::at( a_Position );
	}

	inline constexpr auto& Back() const
	{
		return Base::back();
	}

	inline constexpr auto Capacity() const
	{
		return Base::size();
	}

	template < typename Container >
	auto Combine( Container& a_Container ) const
	{
		ReadOnlyRefArray< ValueType > Result;
		Result.reserve( ContainerTraits::Size( a_Container ) + Base::size() );

		auto BegA = Base::begin();
		auto EndA = Base::end();
		auto BegB = ContainerTraits::Begin( a_Container );
		auto EndB = ContainerTraits::End( a_Container );

		for ( ; BegA != EndA; ++BegA )
		{
			Result.push_back( *BegA );
		}

		for ( ; BegB != EndB; ++BegB )
		{
			Result.push_back( *BegB );
		}

		return Result;
	}

	template < typename Container >
	auto CopyTo( Container& a_Container ) const
	{
		auto BegA = Base::begin();
		auto EndA = Base::end();
		auto BegB = ContainerTraits::Begin( a_Container );
		auto EndB = ContainerTraits::End( a_Container );

		for ( ; BegA != EndA && BegB != EndB; ++BegA, ++BegB )
		{
			*BegB = *BegA;
		}
	}

	inline auto Data() const
	{
		return Base::data();
	}

	template < typename Container >
	inline auto Difference( Container& a_Container ) const
	{
		set< const ValueType*, LessPtr< ValueType > > SetA;
		set< const ValueType*, LessPtr< ValueType > > SetB;
		ReadOnlyRefArray< ValueType > Result;

		for ( const auto& Entry : Underlying() )
		{
			SetA.insert( &Entry );
		}

		auto Beg = ContainerTraits::Begin( a_Container );
		auto End = ContainerTraits::End( a_Container );

		for ( ; Beg != End; ++Beg )
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

	template < typename Container >
	inline bool Equals( Container& a_Container ) const
	{
		if constexpr ( Base::size() != ContainerTraits::Size( a_Container ) )
		{
			return false;
		}

		auto BegA = Base::begin();
		auto BegB = ContainerTraits::Begin( a_Container );

		for ( size_t Size = Base::size(); Size > 0; --Size, ++BegA, ++BegB )
		{
			if ( *BegA != *BegB )
			{
				return false;
			}
		}

		return true;
	}

	bool Exists( const ValueType& a_Value ) const
	{
		for ( const auto& Entry : Underlying() )
		{
			if ( Entry == a_Value )
			{
				return true;
			}
		}

		return false;
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

	auto Find( const ValueType& a_Value ) const
	{
		auto End = CEnd();

		for ( auto Beg = CBegin(); Beg != End; ++Beg )
		{
			if ( *Beg == a_Value )
			{
				return Beg;
			}
		}

		return End;
	}

	auto Find( const Predicate< const ValueType& >& a_Predicate ) const
	{
		auto End = CEnd();

		for ( auto Beg = CBegin(); Beg != End; ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return Beg;
			}
		}

		return End;
	}

	auto Find( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
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

	auto FindAll( const Predicate< const ValueType& >& a_Predicate ) const
	{
		Array< CIterator > Result;
		auto End = CEnd();

		for ( auto Beg = CBegin(); Beg != End; ++Beg )
		{
			if ( a_Predicate.Invoke( Beg ) )
			{
				Result.PushBack( Beg );
			}
		}

		return Result;
	}

	auto FindAll( CIterator a_Begin, CIterator a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		Array< CIterator > Result;

		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.PushBack( a_Begin );
			}
		}

		return Result;
	}

	auto FindLast( const ValueType& a_Value ) const
	{
		auto End = CREnd();

		for ( auto Beg = CRBegin(); Beg != End; ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return Beg.base() - 1;
			}
		}

		return CEnd();
	}

	auto FindLast( const Predicate< const ValueType& >& a_Predicate ) const
	{
		auto End = CREnd();

		for ( auto Beg = CRBegin(); Beg != End; ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return Beg.base() - 1;
			}
		}

		return CEnd();
	}

	auto FindLast( const CIterator& a_Begin, const CIterator& a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Beg = a_End - 1; Beg >= a_Begin(); --Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return Beg;
			}
		}

		return CEnd();
	}

	void ForEach( const Action< const ValueType& >& a_Action ) const
	{
		for ( const auto& Entry : Underlying() )
		{
			a_Action.Invoke( Entry );
		}
	}

	void ForEach( CIterator a_Begin, CIterator a_End, const Action< const ValueType& >& a_Action ) const
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

	int IndexOf( const ValueType& a_Value ) const
	{
		auto Beg = CBegin();

		for ( int Index = 0; Index < Base::size(); ++Index, ++Beg )
		{
			if ( *Beg == a_Value )
			{
				return Index;
			}
		}

		return -1;
	}

	int IndexOf( const CIterator& a_Where ) const
	{
		if ( a_Where == CEnd() )
		{
			return -1;
		}

		return a_Where - CBegin();
	}

	int IndexOf( const Predicate< const ValueType& >& a_Predicate ) const
	{
		auto Beg = CBegin();

		for ( int Index = 0; Index < Base::size(); ++Index, ++Beg )
		{
			if ( a_Predicate( *Beg ) )
			{
				return Index;
			}
		}

		return -1;
	}

	template < typename Container >
	auto Intersection( Container& a_Container ) const
	{
		set< const ValueType*, LessPtr< ValueType > > SetA;
		set< const ValueType*, LessPtr< ValueType > > SetB;
		ReadOnlyRefArray Result;

		for ( const auto& Entry : Underlying() )
		{
			SetA.insert( &Entry );
		}

		auto End = ContainerTraits::End( a_Container );

		for ( auto Beg = ContainerTraits::Begin( a_Container ); Beg != End; ++Beg )
		{
			SetB.insert( &*Beg );
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
				Result.emplace_back( **Begin );
			}
		}

		return Result;
	}

	template < typename T, typename = enable_if_t< is_integral_v< T >, void > >
	bool IsValidIndex( T a_Index ) const
	{
		if constexpr ( is_signed_v< T > )
		{
			if ( a_Index < 0 )
			{
				return false;
			}
		}

		if ( a_Index >= Base::size() )
		{
			return false;
		}

		return true;
	}

	inline auto MaxSize() const
	{
		return Base::max_size();
	}

	inline auto Partition( const CIterator& a_Begin, const CIterator& a_End ) const
	{
		return ReadOnlyRefArray< ValueType >( a_Begin, a_End );
	}

	inline void Reverse()
	{
		reverse( Base::begin(), Base::end() );
	}

	inline void Reverse( const CIterator& a_Begin, const CIterator& a_End )
	{
		reverse( *reinterpret_cast< typename Base::iterator* >( &a_Begin ), *reinterpret_cast< typename Base::iterator* >( &a_End ) );
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

	inline void Swap( CIterator& a_IndexA, CIterator& a_IndexB )
	{
		swap( **reinterpret_cast< typename Base::iterator* >( &a_IndexA ), **reinterpret_cast< typename Base::iterator* >( &a_IndexB ) );
	}

	inline void Swap( ReadOnlyArray< ValueType, SIZE >& a_ReadOnlyArray )
	{
		Base::swap( *reinterpret_cast< Base* >( &a_ReadOnlyArray ) );
	}

	template < typename Container >
	inline auto Symmetry( Container& a_Container ) const
	{
		set< const ValueType*, LessPtr< ValueType > > SetA;
		set< const ValueType*, LessPtr< ValueType > > SetB;
		ReadOnlyRefArray< ValueType > Result;

		for ( const auto& Entry : Underlying() )
		{
			SetA.insert( &Entry );
		}

		auto End = ContainerTraits::End( a_Container );

		for ( auto Beg = ContainerTraits::Begin( a_Container ); Beg != End; ++Beg )
		{
			SetB.insert( &*Beg );
		}

		auto IteratorA = SetA.begin();
		auto IteratorB = SetB.begin();

		while ( IteratorA != SetA.end() || IteratorB != SetB.end() )
		{
			while ( IteratorA != SetA.end() && ( IteratorB == SetB.end() || **( IteratorA ) < **IteratorB ) )
			{
				Result.emplace_back( **( IteratorA++ ) );
			}

			while ( IteratorB != SetB.end() && ( IteratorA == SetA.end() || **( IteratorB ) < **IteratorA ) )
			{
				Result.emplace_back( **( IteratorB++ ) );
			}

			if ( IteratorA != SetA.end() ) ++IteratorA;
			if ( IteratorB != SetB.end() ) ++IteratorB;
		}

		return Result;
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

	inline bool TrueForAll( CIterator a_Begin, const CIterator& a_End, const Predicate< const ValueType& >& a_Predicate ) const
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

	template < typename Container >
	inline auto Union( Container& a_Container ) const
	{
		set< const ValueType*, LessPtr< ValueType > > UnionSet;
		ReadOnlyRefArray< ValueType > Result;

		for ( const auto& Entry : Underlying() )
		{
			UnionSet.insert( &Entry );
		}

		auto End = ContainerTraits::End( a_Container );

		for ( auto Beg = ContainerTraits::Begin( a_Container ); Beg != End; ++Beg )
		{
			UnionSet.insert( &*Beg );
		}

		for ( const auto& Entry : UnionSet )
		{
			Result.EmplaceBack( *Entry );
		}

		return Result;
	}

	#pragma region Operator
	inline constexpr auto& operator[]( size_t a_Position ) const
	{
		return Base::operator[]( a_Position );
	}
	#pragma endregion

	#pragma region Iterator
	inline auto Begin() const
	{
		return Base::begin();
	}

	inline auto CBegin() const
	{
		return Base::cbegin();
	}

	inline auto RBegin() const
	{
		return Base::rbegin();
	}

	inline auto CRBegin() const
	{
		return Base::crbegin();
	}

	inline auto End() const
	{
		return Base::end();
	}

	inline auto CEnd() const
	{
		return Base::cend();
	}

	inline auto REnd() const
	{
		return Base::rend();
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
	using Iterator = typename array< ValueType, SIZE >::iterator;
	using CIterator = typename array< ValueType, SIZE >::const_iterator;
	using RIterator = typename array< ValueType, SIZE >::reverse_iterator;
	using CRIterator = typename array< ValueType, SIZE >::const_reverse_iterator;

	Array() = default;

	template < size_t LENGTH >
	Array( ValueType( *a_Array )[ LENGTH ] )
	{
		size_t Size = LENGTH > SIZE ? SIZE : LENGTH;
		size_t Index = 0;

		for ( auto Beg = Base::begin(); Index < Size; ++Index, ++Beg )
		{
			*Beg = a_Array[ Index ];
		}
	}

	Array( InitializerList< ValueType >& a_InitializerList )
	{
		size_t Size = a_InitializerList.size() < SIZE ? a_InitializerList.size() : SIZE;
		auto BegA = Base::begin();
		auto BegB = a_InitializerList.begin();

		for ( size_t Index = 0; Index < Size; ++Index, ++BegA, ++BegB )
		{
			*BegA = *BegB;
		}
	}

	Array( InitializerList< ValueType >&& a_InitializerList )
	{
		size_t Size = a_InitializerList.size() < SIZE ? a_InitializerList.size() : SIZE;
		auto BegA = Base::begin();
		auto BegB = a_InitializerList.begin();

		for ( size_t Index = 0; Index < Size; ++Index, ++BegA, ++BegB )
		{
			*BegA = *BegB;
		}
	}

	template < typename Container >
	Array( Container& a_Container )
	{
		auto BegC = ContainerTraits::Begin( a_Container );
		auto EndC = ContainerTraits::End( a_Container );
		Iterator Beg( Base::begin() );
		Iterator End( Base::end() );

		for ( ; BegC != EndC && Beg != End; ++BegC, ++Beg )
		{
			*Beg = *BegC;
		}
	}

	template < typename Iter >
	Array( Iter a_Begin, const Iter& a_End )
	{
		Iterator Beg( Base::begin() );
		Iterator End( Base::end() );

		for ( ; a_Begin != a_End && Beg != End; ++a_Begin, ++Beg )
		{
			*Beg = *a_Begin;
		}
	}

	inline auto AsEnumerable()
	{
		return Enumerable< ValueType >( Base::begin(), Base::end(), Base::size() );
	}

	inline auto AsEnumerable() const
	{
		return ReadOnlyEnumerable< ValueType >( Base::begin(), Base::end(), Base::size() );
	}

	inline auto AsReadOnlyEnumerable() const
	{
		return ReadOnlyEnumerable< ValueType >( Base::begin(), Base::end(), Base::size() );
	}

	inline auto AsRefArray()
	{
		return RefArray< ValueType >( *this );
	}

	inline auto AsRefArray() const
	{
		return ReadOnlyRefArray< ValueType >( *this );
	}

	inline auto AsReadOnlyRefArray() const
	{
		return ReadOnlyRefArray< ValueType >( *this );
	}

	inline auto& AsReadOnly()
	{
		return *reinterpret_cast< ReadOnlyArray< ValueType, SIZE >* >( this );
	}

	inline auto& AsReadOnly() const
	{
		return *reinterpret_cast< const ReadOnlyArray< ValueType, SIZE >* >( this );
	}

	inline void Assign( ValueType& a_Value )
	{
		Base::assign( a_Value );
	}

	inline void Assign( const ValueType& a_Value )
	{
		Base::assign( a_Value );
	}

	inline void Assign( Iterator a_Begin, const Iterator& a_End, ValueType& a_Value )
	{
		while ( a_Begin < a_End )
		{
			*a_Begin++ = a_Value;
		}
	}

	inline void Assign( Iterator a_Begin, const Iterator& a_End, const ValueType& a_Value )
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

	inline auto Capacity() const
	{
		return Base::size();
	}

	template < typename Container >
	inline auto Combine( Container& a_Container )
	{
		RefArray< ValueType > Result;
		Result.reserve( Base::size() + ContainerTraits::Size( a_Container ) );
		auto EndA = End();

		for ( auto Beg = Begin(); Beg != EndA; ++Beg )
		{
			Result.emplace_back( *Beg );
		}

		auto EndB = ContainerTraits::End( a_Container );

		for ( auto Beg = ContainerTraits::Begin( a_Container ); Beg != EndB; ++Beg )
		{
			Result.emplace_back( *Beg );
		}

		return Result;
	}

	template < typename Container >
	inline auto Combine( Container& a_Container ) const
	{
		ReadOnlyRefArray< ValueType > Result;
		Result.reserve( Base::size() + ContainerTraits::Size( a_Container ) );
		auto EndA = End();

		for ( auto Beg = Begin(); Beg != EndA; ++Beg )
		{
			Result.emplace_back( *Beg );
		}

		auto EndB = ContainerTraits::End( a_Container );

		for ( auto Beg = ContainerTraits::Begin( a_Container ); Beg != EndB; ++Beg )
		{
			Result.emplace_back( *Beg );
		}

		return Result;
	}

	template < typename Container >
	inline void CopyTo( Container& a_Container )
	{
		auto BegA = Begin();
		auto BegB = ContainerTraits::Begin( a_Container );
		size_t Size = ContainerTraits::Size( a_Container ) < Base::size() ? ContainerTraits::Size( a_Container ) : Base::size();

		for ( ; Size > 0; --Size, ++BegA, ++BegB )
		{
			*BegB = *BegA;
		}
	}

	template < typename Container >
	inline void CopyTo( Container& a_Container ) const
	{
		auto BegA = Begin();
		auto BegB = ContainerTraits::Begin( a_Container );
		size_t Size = ContainerTraits::Size( a_Container ) < Base::size() ? ContainerTraits::Size( a_Container ) : Base::size();

		for ( ; Size > 0; --Size, ++BegA, ++BegB )
		{
			*BegB = *BegA;
		}
	}

	inline auto Data()
	{
		return Base::data();
	}

	inline auto Data() const
	{
		return Base::data();
	}

	template < typename Container >
	inline auto Difference( Container& a_Container )
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

	template < size_t LENGTH >
	inline auto Difference( const Array< ValueType, LENGTH >& a_Array ) const
	{
		set< ValueType*, LessPtr< ValueType > > SetA;
		set< ValueType*, LessPtr< ValueType > > SetB;
		ReadOnlyRefArray< ValueType > Result;

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

	template < typename... T >
	inline auto& EmplaceAt( const Iterator& a_Where, T&&... a_Args )
	{
		return *( new ( &*a_Where ) ValueType( forward< T >( a_Args )... ) );
	}

	template < typename... T >
	inline auto& EmplaceZeroedAt( const Iterator& a_Where, T&&... a_Args )
	{
		return *( new ( &*a_Where ) ValueType( forward< T >( a_Args ) ) );
	}

	inline auto Empty() const
	{
		return false;
	}

	template < typename Container >
	inline bool Equals( Container& a_Container ) const
	{
		if constexpr ( Base::size() != ContainerTraits::Size( a_Container ) )
		{
			return false;
		}

		auto BegA = Base::begin();
		auto BegB = ContainerTraits::Begin( a_Container );

		for ( size_t Size = Base::size(); Size > 0; --Size, ++BegA, ++BegB )
		{
			if ( *BegA != *BegB )
			{
				return false;
			}
		}

		return true;
	}

	bool Exists( const ValueType& a_Value ) const
	{
		for ( const auto& Entry : Underlying() )
		{
			if ( Entry == a_Value )
			{
				return true;
			}
		}

		return false;
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

	auto Find( const ValueType& a_Value )
	{
		auto End = Base::end();

		for ( auto Beg = Base::begin(); Beg != End; ++Beg )
		{
			if ( *Beg == a_Value )
			{
				return Beg;
			}
		}

		return End;
	}

	auto Find( const ValueType& a_Value ) const
	{
		auto End = Base::end();

		for ( auto Beg = Base::begin(); Beg != End; ++Beg )
		{
			if ( *Beg == a_Value )
			{
				return Beg;
			}
		}

		return End;
	}

	auto Find( const Predicate< const ValueType& >& a_Predicate )
	{
		auto End = Base::end();

		for ( auto Beg = Base::begin(); Beg != End; ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return Beg;
			}
		}

		return End;
	}

	auto Find( const Predicate< const ValueType& >& a_Predicate ) const
	{
		auto End = Base::end();

		for ( auto Beg = Base::begin(); Beg != End; ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return Beg;
			}
		}

		return End;
	}

	auto Find( Iterator a_Begin, const Iterator& a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				return a_Begin;
			}
		}

		return Base::end();
	}

	auto Find( CIterator a_Begin, const CIterator& a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				return a_Begin;
			}
		}

		return Base::end();
	}

	inline auto FindAll( const Predicate< const ValueType& >& a_Predicate )
	{
		Array< Iterator > Result;
		auto End = Base::end();

		for ( auto Beg = Base::begin(); Beg != End; ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				Result.push_back( Beg );
			}
		}

		return Result;
	}

	inline auto FindAll( const Predicate< const ValueType& >& a_Predicate ) const
	{
		Array< CIterator > Result;
		auto End = Base::end();

		for ( auto Beg = Base::begin(); Beg != End; ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				Result.push_back( Beg );
			}
		}

		return Result;
	}

	inline auto FindAll( Iterator a_Begin, const Iterator& a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		Array< Iterator > Result;

		for ( ; a_Begin != a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.push_back( a_Begin );
			}
		}

		return Result;
	}

	inline auto FindAll( CIterator a_Begin, const CIterator& a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		Array< CIterator > Result;

		for ( ; a_Begin != a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.push_back( a_Begin );
			}
		}

		return Result;
	}

	inline auto FindLast( const Predicate< const ValueType& >& a_Predicate )
	{
		auto End = Base::rend();

		for ( auto Beg = Base::rbeg(); Beg < End; ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return Beg.base() - 1;
			}
		}

		return End;
	}

	inline auto FindLast( const Predicate< const ValueType& >& a_Predicate ) const
	{
		auto End = Base::rend();

		for ( auto Beg = Base::rbeg(); Beg < End; ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return Beg.base() - 1;
			}
		}

		return End;
	}

	inline auto FindLast( const Iterator& a_Begin, const Iterator& a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( auto Beg = a_End - 1; Beg >= a_Begin; --Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return Beg;
			}
		}

		return Base::end();
	}

	inline auto FindLast( const CIterator& a_Begin, const CIterator& a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( auto Beg = a_End - 1; Beg >= a_Begin; --Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return Beg;
			}
		}

		return Base::end();
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

	inline void ForEach( Iterator a_Begin, const Iterator& a_End, const Action< ValueType& >& a_Action )
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			a_Action.Invoke( *a_Begin );
		}
	}

	inline void ForEach( CIterator a_Begin, const CIterator& a_End, const Action< const ValueType& >& a_Action ) const
	{
		for ( ; a_Begin < a_End; ++a_Begin )
		{
			a_Action.Invoke( *a_Begin );
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

	int IndexOf( const ValueType& a_Value ) const
	{
		auto Beg = CBegin();

		for ( int Index = 0; Index < Base::size(); ++Index, ++Beg )
		{
			if ( *Beg == a_Value )
			{
				return Index;
			}
		}

		return -1;
	}

	int IndexOf( const CIterator& a_Where ) const
	{
		if ( a_Where == CEnd() )
		{
			return -1;
		}

		return a_Where - CBegin();
	}

	int IndexOf( const Predicate< const ValueType& >& a_Predicate ) const
	{
		auto Beg = CBegin();

		for ( int Index = 0; Index < Base::size(); ++Index, ++Beg )
		{
			if ( a_Predicate( *Beg ) )
			{
				return Index;
			}
		}

		return -1;
	}

	inline void InsertAt( Iterator& a_Where, ValueType& a_Value )
	{
		*a_Where = a_Value;
	}

	inline void InsertAt( Iterator& a_Where, const ValueType& a_Value )
	{
		*a_Where = a_Value;
	}

	template < typename Container >
	inline auto Intersection( Container& a_Container )
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

		bool SetAIsLargest = SetA.size() > SetB.size();
		decltype( SetA )& Smallest = !SetAIsLargest ? SetA : SetB;
		decltype( SetA )& Largest = SetAIsLargest ? SetA : SetB;
		auto Begin = !SetAIsLargest ? SetA.begin() : SetB.begin();
		auto End = !SetAIsLargest ? SetA.end() : SetB.end();

		for ( ; Begin != End; ++Begin )
		{
			if ( !Largest.insert( *Begin ).second )
			{
				Result.emplace_back( **Begin );
			}
		}

		return Result;
	}

	template < typename Container >
	inline auto Intersection( Container& a_Container ) const
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

		bool SetAIsLargest = SetA.size() > SetB.size();
		decltype( SetA )& Smallest = !SetAIsLargest ? SetA : SetB;
		decltype( SetA )& Largest = SetAIsLargest ? SetA : SetB;
		auto Begin = !SetAIsLargest ? SetA.begin() : SetB.begin();
		auto End = !SetAIsLargest ? SetA.end() : SetB.end();

		for ( ; Begin != End; ++Begin )
		{
			if ( !Largest.insert( *Begin ).second )
			{
				Result.emplace_back( **Begin );
			}
		}

		return Result;
	}

	template < typename T, typename = enable_if_t< is_integral_v< T >, void > >
	bool IsValidIndex( T a_Index ) const
	{
		if constexpr ( is_signed_v< T > )
		{
			if ( a_Index < 0 )
			{
				return false;
			}
		}

		if ( a_Index >= Base::size() )
		{
			return false;
		}

		return true;
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

	inline auto Partition( size_t a_Offset, size_t a_Length )
	{
		return RefArray< ValueType >( Begin() + a_Offset, Begin() + a_Offset + a_Length );
	}

	inline auto Partition( size_t a_Offset, size_t a_Length ) const
	{
		return ReadOnlyRefArray< ValueType >( Begin() + a_Offset, Begin() + a_Offset + a_Length );
	}

	inline void Reverse()
	{
		reverse( Base::begin(), Base::end() );
	}

	inline void Reverse( const CIterator& a_Begin, const CIterator& a_End )
	{
		reverse( *reinterpret_cast< typename Base::iterator* >( &a_Begin ), *reinterpret_cast< typename Base::iterator* >( &a_End ) );
	}

	inline constexpr auto Size() const
	{
		return Base::size();
	}

	template < bool ASCENDING = true >
	inline void Sort()
	{
		sort( Base::begin(), Base::end(), []( const ValueType& Left, const ValueType& Right )
			  {
				  if constexpr ( ASCENDING )
				  {
					  return Left < Right;
				  }
				  else
				  {
					  return Left > Right;
				  }
			  } );
	}

	template < bool ASCENDING = true >
	inline void Sort( const CIterator& a_Begin, const CIterator& a_End )
	{
		auto Beg = *reinterpret_cast< typename Base::iterator* >( &a_Begin );
		auto End = *reinterpret_cast< typename Base::iterator* >( &a_End );

		sort( Beg, End, []( const ValueType& Left, const ValueType& Right )
			  {
				  if constexpr ( ASCENDING )
				  {
					  return Left < Right;
				  }
				  else
				  {
					  return Left > Right;
				  }
			  } );
	}

	inline void Sort( const Predicate< const ValueType&, const ValueType& >& a_Comparer )
	{
		sort( Base::begin(), Base::end(), a_Comparer );
	}

	inline void Sort( const CIterator& a_Begin, const CIterator& a_End, const Predicate< const ValueType&, const ValueType& >& a_Comparer )
	{
		auto Beg = *reinterpret_cast< typename Base::iterator* >( &a_Begin );
		auto End = *reinterpret_cast< typename Base::iterator* >( &a_End );
		sort( Beg, End, a_Comparer );
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

	inline void Swap( CIterator& a_IndexA, CIterator& a_IndexB )
	{
		swap( *reinterpret_cast< typename Base::iterator* >( &a_IndexA ), *reinterpret_cast< typename Base::iterator* >( &a_IndexB ) );
	}
	
	inline void Swap( Array< ValueType, SIZE >& a_Array )
	{
		return Base::swap( a_Array.Underlying() );
	}

	template < typename Container >
	inline auto Symmetry( Container& a_Container )
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

	template < typename Container >
	inline auto Symmetry( Container& a_Container ) const
	{
		set< ValueType*, LessPtr< ValueType > > SetA;
		set< ValueType*, LessPtr< ValueType > > SetB;
		ReadOnlyRefArray< ValueType > Result;

		for ( auto& Entry : Underlying() )
		{
			SetA.insert( &Entry );
		}

		auto End = ContainerTraits::End( a_Container );

		for ( auto Beg = ContainerTraits::Begin( a_Container ); Beg != End; ++Beg )
		{
			SetB.insert( &*Beg );
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
	inline auto Union( Array< ValueType, LENGTH >& a_Array )
	{
		set< ValueType*, LessPtr< ValueType > > UnionSet;
		Array< Reference< ValueType > > Result;

		for ( auto& Entry : Underlying() )
		{
			UnionSet.insert( &Entry );
		}

		for ( auto& Entry : a_Array.Underlying() )
		{
			UnionSet.insert( &Entry );
		}

		for ( auto& Entry : UnionSet )
		{
			Result.emplace_back( *Entry );
		}

		return Result;
	}

	template < size_t LENGTH >
	inline auto Union( const Array< ValueType, LENGTH >& a_Array ) const
	{
		set< const ValueType*, LessPtr< ValueType > > UnionSet;
		Array< CReference< ValueType > > Result;

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
			Result.emplace_back( *Entry );
		}

		return Result;
	}

	inline void ZeroAll()
	{
		memset( Base::data(), 0, sizeof( Base ) );
	}

	inline auto Zero( size_t a_Position )
	{
		return static_cast< ValueType* >( memset( Base::data() + a_Position, 0, sizeof( ValueType ) ) );
	}

	inline void Zero( size_t a_Start, size_t a_Length )
	{
		memset( Base::data() + a_Start, 0, sizeof( ValueType ) * a_Length );
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

	template < class, size_t > friend class ReadOnlyArry;
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
	using Iterator = typename vector< ValueType >::iterator;
	using CIterator = typename vector< ValueType >::const_iterator;
	using RIterator = typename vector< ValueType >::reverse_iterator;
	using CRIterator = typename vector< ValueType >::const_reverse_iterator;

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

	template < typename Iter, typename = ContainerTraits::EnableIfIterType< Iter, ValueType > >
	Array( Iter a_Begin, Iter a_End )
	{
		if constexpr ( _Is_random_iter_v< Iter > )
		{
			Base::reserve( a_End - a_Begin );
		}

		Base::insert( Base::begin(), a_Begin, a_End );
	}

	template < typename Container, typename = ContainerTraits::EnableIfContainer< Container, ValueType > >
	Array( const Container& a_Container )
	{
		auto Beg = ContainerTraits::Begin( a_Container );
		auto End = ContainerTraits::End( a_Container );

		if constexpr ( ContainerTraits::IsSizeable< Container > )
		{
			Base::reserve( ContainerTraits::Size( a_Container ) );
		}

		Base::insert( Base::begin(), Beg, End );
	}

	inline auto AsEnumerable()
	{
		return Enumerable< ValueType >( Begin(), End() );
	}

	inline auto AsEnumerable() const
	{
		return CEnumerable< ValueType >( Begin(), End() );
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
	inline auto Combine( Array< ValueType, LENGTH >& a_Array )
	{
		Array< Reference< ValueType > > Result;
		Result.reserve( Base::size() + a_Array.size() );

		for ( auto Iterator = Begin(); Iterator != End(); ++Iterator )
		{
			Result.emplace_back( *Iterator );
		}

		for ( auto Iterator = a_Array.Begin(); Iterator != a_Array.End(); ++Iterator )
		{
			Result.emplace_back( *Iterator );
		}

		return Result;
	}

	template < size_t LENGTH >
	inline auto Combine( const Array< ValueType, LENGTH >& a_Array ) const
	{
		Array< CReference< ValueType > > Result;
		Result.reserve( Base::size() + a_Array.size() );

		for ( auto Iterator = Begin(); Iterator != End(); ++Iterator )
		{
			Result.emplace_back( *Iterator );
		}

		for ( auto Iterator = a_Array.Begin(); Iterator != a_Array.End(); ++Iterator )
		{
			Result.emplace_back( *Iterator );
		}

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
	inline auto Difference( Array< ValueType, LENGTH >& a_Array )
	{
		set< ValueType*, LessPtr< ValueType > > SetA;
		set< ValueType*, LessPtr< ValueType > > SetB;
		Array< Reference< ValueType > > Result;

		for ( auto& Entry : Underlying() )
		{
			SetA.insert( &Entry );
		}

		for ( auto& Entry : a_Array.Underlying() )
		{
			SetB.insert( &Entry );
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

	template < size_t LENGTH >
	inline auto Difference( const Array< ValueType, LENGTH >& a_Array ) const
	{
		set< const ValueType*, LessPtr< ValueType > > SetA;
		set< const ValueType*, LessPtr< ValueType > > SetB;
		Array< CReference< ValueType > > Result;

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
				Result.emplace_back( **Iterator );
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
	inline auto Intersection( Array< ValueType, LENGTH >& a_Array )
	{
		set< ValueType*, LessPtr< ValueType > > SetA;
		set< ValueType*, LessPtr< ValueType > > SetB;
		Array< Reference< ValueType > > Result;

		for ( auto& Entry : Underlying() )
		{
			SetA.insert( &Entry );
		}

		for ( auto& Entry : a_Array.Underlying() )
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
				Result.emplace_back( **Begin );
			}
		}

		return Result;
	}

	template < size_t LENGTH >
	inline auto Intersection( const Array< ValueType, LENGTH >& a_Array ) const
	{
		set< const ValueType*, LessPtr< ValueType > > SetA;
		set< const ValueType*, LessPtr< ValueType > > SetB;
		Array< CReference< ValueType > > Result;

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
				Result.emplace_back( **Begin );
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

	inline auto Partition( size_t a_Offset, size_t a_Length )
	{
		return Array< ValueType >( Begin() + a_Offset, Begin() + a_Offset + a_Length );
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
		sort( Base::begin(), Base::end(), []( const ValueType& Left, const ValueType& Right )
			  {
				  if constexpr ( ASCENDING )
				  {
					  return Left < Right;
				  }
				  else
				  {
					  return Left > Right;
				  }
			  } );
	}

	template < bool ASCENDING = true >
	inline void Sort( Iterator a_Begin, Iterator a_End )
	{
		sort( a_Begin, a_End, []( const ValueType& Left, const ValueType& Right )
			  {
				  if constexpr ( ASCENDING )
				  {
					  return Left < Right;
				  }
				  else
				  {
					  return Left > Right;
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
	inline auto Symmetry( Array< ValueType, LENGTH >& a_Array )
	{
		set< ValueType*, LessPtr< ValueType > > SetA;
		set< ValueType*, LessPtr< ValueType > > SetB;
		Array< Reference< ValueType > > Result;

		for ( auto& Entry : Underlying() )
		{
			SetA.insert( &Entry );
		}

		for ( auto& Entry : a_Array.Underlying() )
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

	template < size_t LENGTH >
	inline auto Symmetry( const Array< ValueType, LENGTH >& a_Array ) const
	{
		set< const ValueType*, LessPtr< ValueType > > SetA;
		set< const ValueType*, LessPtr< ValueType > > SetB;
		Array< CReference< ValueType > > Result;

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
				Result.emplace_back( **( IteratorA++ ) );
			}

			while ( IteratorB != SetB.end() && ( IteratorA == SetA.end() || **( IteratorB ) < **IteratorA ) )
			{
				Result.emplace_back( **( IteratorB++ ) );
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
	inline auto Union( Array< ValueType, LENGTH >& a_Array )
	{
		set< ValueType*, LessPtr< ValueType > > UnionSet;
		Array< Reference< ValueType > > Result;

		for ( auto& Entry : Underlying() )
		{
			UnionSet.insert( &Entry );
		}

		for ( auto& Entry : a_Array.Underlying() )
		{
			UnionSet.insert( &Entry );
		}

		for ( auto& Entry : UnionSet )
		{
			Result.emplace_back( *Entry );
		}

		return Result;
	}

	template < size_t LENGTH >
	inline auto Union( const Array< ValueType, LENGTH >& a_Array ) const
	{
		set< const ValueType*, LessPtr< ValueType > > UnionSet;
		Array< CReference< ValueType > > Result;

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
			Result.emplace_back( *Entry );
		}

		return Result;
	}

	inline void ZeroAll()
	{
		memset( Base::data(), 0, Base::size() * sizeof( ValueType ) );
	}

	inline auto Zero( size_t a_Position )
	{
		return static_cast< ValueType* >( memset( Base::data() + a_Position, 0, sizeof( ValueType ) ) );
	}

	inline void Zero( size_t a_Start, size_t a_Length )
	{
		memset( Base::data() + a_Start, 0, sizeof( ValueType ) * a_Length );
	}

	#pragma region Operator
	auto& operator=( const Base&& a_Array )
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

	template < class, size_t > friend class ReadOnlyArry;
	template < class, size_t > friend class Array;

};