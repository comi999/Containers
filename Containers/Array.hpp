#pragma once
#include <array>
#include "Container.hpp"

template < typename T, size_t _Size >
using ArrayIterator = RandomAccessIterator< typename std::array< T, _Size >::iterator >;

template < typename T, size_t _Size >
using ArrayCIterator = RandomAccessIterator< typename std::array< T, _Size >::const_iterator >;

template < typename T, size_t _Size >
using ArrayRIterator = RandomAccessIterator< typename std::array< T, _Size >::reverse_iterator >;

template < typename T, size_t _Size >
using ArrayCRIterator = RandomAccessIterator< typename std::array< T, _Size >::const_reverse_iterator >;

template < typename T, size_t _Size >
class Array : public IContiguousCollection< T >
{
public:

	using BaseType = IContiguousCollection< T >;
	using UnderlyingType = std::array< T, _Size >;
	using IteratorType = ArrayIterator< T, _Size >;
	using CIteratorType = ArrayCIterator< T, _Size >;
	using RIteratorType = ArrayRIterator< T, _Size >;
	using CRIteratorType = ArrayCRIterator< T, _Size >;
	using EnumeratorType = typename BaseType::EnumeratorType;
	using CEnumeratorType = typename BaseType::CEnumeratorType;
	using EnumerableType = typename BaseType::EnumerableType;
	using CEnumerableType = typename BaseType::CEnumerableType;

	constexpr Array() = default;
	constexpr Array( const Array& ) = default;
	constexpr Array( Array&& ) = default;
	constexpr Array( const T& a_InitializerValue );
	constexpr Array( const InitializerList< T >& a_InitializerList );
	constexpr Array( InitializerList< T >&& a_InitializerList );
	constexpr Array( const T( &a_Array )[ _Size ] );
	constexpr Array( T( && a_Array )[ _Size ] );

private:

	template < size_t... Idxs >
	constexpr Array( const T& a_InitializerValue, std::index_sequence< Idxs... > );

	template < size_t... Idxs >
	constexpr Array( const T( &a_Array )[ _Size ], std::index_sequence< Idxs... > );

	template < size_t... Idxs >
	constexpr Array( T( &&a_Array )[ _Size ], std::index_sequence< Idxs... > );

	template < size_t... Idxs >
	constexpr Array( const InitializerList< T >& a_InitializerList, std::index_sequence< Idxs... > );

	template < size_t... Idxs >
	constexpr Array( InitializerList< T >&& a_InitializerList, std::index_sequence< Idxs... > );

public:

	constexpr Array& operator=( const Array& ) = default;
	constexpr Array& operator=( Array&& ) = default;
	constexpr const T& operator[]( size_t a_Offset ) const { return m_Underlying[ a_Offset ]; }
	constexpr T& operator[]( size_t a_Offset ) { return m_Underlying[ a_Offset ]; }

	constexpr const T& At( size_t a_Offset ) const { return m_Underlying.at( a_Offset ); }
	constexpr T& At( size_t a_Offset ) { return m_Underlying.at( a_Offset ); }
	constexpr const T& Back() const { return m_Underlying.back(); }
	constexpr T& Back() { return m_Underlying.back(); }
	constexpr const T* Data() const { return m_Underlying.data(); }
	constexpr T* Data() { return m_Underlying.data(); }
	constexpr bool Empty() const { return m_Underlying.empty(); }
	constexpr void Fill( const T& a_Value ) { m_Underlying.fill( a_Value ); }
	constexpr const T& Front() const { return m_Underlying.front(); }
	constexpr T& Front() { return m_Underlying.front(); }
	constexpr size_t MaxSize() const { return m_Underlying.max_size(); }
	constexpr size_t Size() const { return m_Underlying.size(); }
	constexpr void Swap( const Array& a_Array ) { m_Underlying.swap( a_Array.m_Underlying ); }

	template < size_t _Index, size_t _Count >
	constexpr const Array< T, _Count >& SubArray() const;

	template < size_t _Index, size_t _Count >
	constexpr Array< T, _Count > SubArray();

	constexpr IteratorType Begin() { return m_Underlying.begin(); }
	constexpr CIteratorType Begin() const { return m_Underlying.cbegin(); }
	constexpr CIteratorType CBegin() const { return m_Underlying.cbegin(); }
	constexpr RIteratorType RBegin() { return m_Underlying.rbegin(); }
	constexpr CRIteratorType RBegin() const { return m_Underlying.crbegin(); }
	constexpr CRIteratorType CRBegin() const { return m_Underlying.crbegin(); }
	constexpr IteratorType End() { return m_Underlying.end(); }
	constexpr CIteratorType End() const { return m_Underlying.cend(); }
	constexpr CIteratorType CEnd() const { return m_Underlying.cend(); }
	constexpr RIteratorType REnd() { return m_Underlying.rend(); }
	constexpr CRIteratorType REnd() const { return m_Underlying.crend(); }
	constexpr CRIteratorType CREnd() const { return m_Underlying.crend(); }

	constexpr const UnderlyingType& GetUnderlying() const { return m_Underlying; }
	constexpr UnderlyingType& GetUnderlying() { return m_Underlying; }

protected:

	EnumerableType IToEnumerable() { return EnumerableType( Size(), Begin(), End() ); }
	EnumeratorType IBegin() { return IteratorType( m_Underlying.begin() ); }
	EnumeratorType IEnd() { return IteratorType( m_Underlying.end() ); }
	EnumeratorType IRBegin() { return RIteratorType( m_Underlying.rbegin() ); }
	EnumeratorType IREnd() { return RIteratorType( m_Underlying.rend() ); }
	size_t ISize() const { return m_Underlying.size(); }
	size_t IMaxSize() const { return m_Underlying.max_size(); }
	T* IData() { return m_Underlying.data(); }
	T& IAt( size_t a_Index ) { return m_Underlying.at( a_Index ); }

private:

	UnderlyingType m_Underlying = {};
};

namespace std
{
	template < size_t _Index, typename T, size_t _Size >
	constexpr const T& get( const Array< T, _Size >& a_Array )
	{
		return std::get< _Index >( a_Array.GetUnderlying() );
	}

	template < size_t _Index, typename T, size_t _Size >
	constexpr const T&& get( const Array< T, _Size >&& a_Array )
	{
		return std::get< _Index >( std::move( a_Array.GetUnderlying() ) );
	}

	template < size_t _Index, typename T, size_t _Size >
	constexpr T& get( Array< T, _Size >& a_Array )
	{
		return std::get< _Index >( a_Array.GetUnderlying() );
	}

	template < size_t _Index, typename T, size_t _Size >
	constexpr T&& get( Array< T, _Size >&& a_Array )
	{
		return std::get< _Index >( std::move( a_Array.GetUnderlying() ) );
	}

	template < typename T, size_t _Size >
	constexpr void swap( Array< T, _Size >& a_Left, Array< T, _Size >& a_Right )
	{
		std::swap( a_Left.GetUnderlying(), a_Right.GetUnderlying() );
	}

	template < typename T, size_t _Size >
	struct tuple_size< Array< T, _Size > > :
		std::integral_constant< size_t, _Size >
	{};

	template < size_t _Index, typename T, size_t _Size >
	struct tuple_element< _Index, Array< T, _Size > >
	{
		using type = T;
	};
}

template < typename T, size_t _Size >
constexpr Array< T, _Size >::Array( const T& a_InitializerValue )
	: Array( a_InitializerValue, std::make_index_sequence< _Size >{} )
{}

template < typename T, size_t _Size >
constexpr Array< T, _Size >::Array( const T( &a_Array )[ _Size ] )
	: Array( a_Array, std::make_index_sequence< _Size >{} )
{}

template < typename T, size_t _Size >
constexpr Array< T, _Size >::Array( T( &&a_Array )[ _Size ] )
	: Array( std::move( a_Array ), std::make_index_sequence< _Size >{} )
{}

template < typename T, size_t _Size >
constexpr Array< T, _Size >::Array( const InitializerList< T >& a_InitializerList )
	: Array( a_InitializerList, std::make_index_sequence< _Size >{} )
{}

template < typename T, size_t _Size >
constexpr Array< T, _Size >::Array( InitializerList< T >&& a_InitializerList )
	: Array( std::move( a_InitializerList ), std::make_index_sequence< _Size >{} )
{}

template < typename T, size_t _Size >
template < size_t... Idxs >
constexpr Array< T, _Size >::Array( const T& a_InitializerValue, std::index_sequence< Idxs... > )
{
	( ( m_Underlying[ Idxs ] = a_InitializerValue ), ... );
}

template < typename T, size_t _Size >
template < size_t... Idxs >
constexpr Array< T, _Size >::Array( const T( &a_Array )[ _Size ], std::index_sequence< Idxs... > )
{
	( ( m_Underlying[ Idxs ] = a_Array[ Idxs ] ), ... );
}

template < typename T, size_t _Size >
template < size_t... Idxs >
constexpr Array< T, _Size >::Array( T( &&a_Array )[ _Size ], std::index_sequence< Idxs... > )
{
	( ( m_Underlying[ Idxs ] = std::move( a_Array[ Idxs ] ) ), ... );
}

template < typename T, size_t _Size >
template < size_t... Idxs >
constexpr Array< T, _Size >::Array( const InitializerList< T >& a_InitializerList, std::index_sequence< Idxs... > )
{
	( ( m_Underlying[ Idxs ] = *( a_InitializerList.begin() + Idxs ) ), ... );
}

template < typename T, size_t _Size >
template < size_t... Idxs >
constexpr Array< T, _Size >::Array( InitializerList< T >&& a_InitializerList, std::index_sequence< Idxs... > )
{
	( ( m_Underlying[ Idxs ] = std::move( *( a_InitializerList.begin() + Idxs ) ) ), ... );
}

template < typename T, size_t _Size >
template < size_t _Index, size_t _Count >
constexpr const Array< T, _Count >& Array< T, _Size >::SubArray() const
{
	return *reinterpret_cast< Array< T, _Count >* >( m_Underlying.data() + _Index );
}

template < typename T, size_t _Size >
template < size_t _Index, size_t _Count >
constexpr Array< T, _Count > Array< T, _Size >::SubArray()
{
	return *reinterpret_cast< Array< T, _Count >* >( m_Underlying.data() + _Index );
}