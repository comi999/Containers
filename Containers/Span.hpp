#pragma once
#include "Container.hpp"

template < typename T >
class IContiguousCollection;

template < typename T >
using SpanIterator = RandomAccessIterator< T* >;

template < typename T >
using SpanCIterator = RandomAccessIterator< const T* >;

template < typename T >
using SpanRIterator = RandomAccessIterator< std::reverse_iterator< T* > >;

template < typename T >
using SpanCRIterator = RandomAccessIterator< std::reverse_iterator< const T* > >;

template < typename T >
class Span : public IContiguousCollection< T >
{
public:

	using ValueType = T;
	using SizeType = size_t;
	using DifferenceType = std::ptrdiff_t;
	using BaseType = IContiguousCollection< T >;

	using IteratorType = SpanIterator< T >;
	using CIteratorType = SpanCIterator< T >;
	using RIteratorType = SpanRIterator< T >;
	using CRIteratorType = SpanCRIterator< T >;

	using EnumeratorType = typename BaseType::EnumeratorType;
	using CEnumeratorType = typename BaseType::CEnumeratorType;
	using EnumerableType = typename BaseType::EnumerableType;
	using CEnumerableType = typename BaseType::CEnumerableType;
	
	constexpr Span() = default;
	constexpr Span( const Span& ) = default;
	constexpr Span( Span&& ) = default;
	constexpr Span( T* a_Data, size_t a_Size );
	
	template < size_t _Size >
	constexpr Span( T( &a_Array )[ _Size ] );

	template < size_t _Size >
	constexpr Span( T( *a_Array )[ _Size ] );

	Span( IContiguousCollection< T >& a_ContiguousCollection );

	IteratorType Begin() { return IteratorType( m_Data ); }
	CIteratorType Begin() const { return CIteratorType( m_Data ); }
	CIteratorType CBegin() const { return CIteratorType( m_Data ); }
	RIteratorType RBegin() { return RIteratorType( std::reverse_iterator< T* >( m_Data + m_Size ) ); }
	CRIteratorType RBegin() const { return CRIteratorType( std::reverse_iterator< const T* >( m_Data + m_Size ) ); }
	CRIteratorType CRBegin() const { return CRIteratorType( std::reverse_iterator< const T* >( m_Data + m_Size ) ); }
	IteratorType End() { return IteratorType( m_Data + m_Size ); }
	CIteratorType End() const { return CIteratorType( m_Data + m_Size ); }
	CIteratorType CEnd() const { return CIteratorType( m_Data + m_Size ); }
	RIteratorType REnd() { return RIteratorType( std::reverse_iterator< T* >( m_Data ) ); }
	CRIteratorType REnd() const { return CRIteratorType( std::reverse_iterator< const T* >( m_Data ) ); }
	CRIteratorType CREnd() const { return CRIteratorType( std::reverse_iterator< const T* >( m_Data ) ); }

	constexpr SizeType Size() const { return m_Size; }
	constexpr SizeType MaxSize() const { return -1; }

	constexpr const T& operator[]( size_t a_Offset ) const { return m_Data[ a_Offset ]; }
	constexpr T& operator[]( size_t a_Offset ) { return m_Data[ a_Offset ]; }

	// Gotta implement more functions here.

protected:

	EnumerableType IToEnumerable() { return EnumerableType( Size(), Begin(), End() ); }
	size_t ISize() const { return Size(); }
	size_t IMaxSize() const { return MaxSize(); }
	EnumeratorType IBegin() { return Begin(); }
	EnumeratorType IEnd() { return End(); }
	EnumeratorType IRBegin() { return RBegin(); }
	EnumeratorType IREnd() { return REnd(); }
	T* IData() { return m_Data; }
	T& IAt( size_t a_Index ) { return m_Data[ a_Index ]; }

private:

	T* m_Data;
	size_t m_Size;
};

template < typename T >
constexpr Span< T >::Span( T* a_Data, size_t a_Size )
	: m_Data( a_Data )
	, m_Size( a_Size )
{}

template < typename T >
template < size_t _Size >
constexpr Span< T >::Span( T( &a_Array )[ _Size ] )
	: m_Data( a_Array )
	, m_Size( _Size )
{}

template < typename T >
template < size_t _Size >
constexpr Span< T >::Span( T( *a_Array )[ _Size ] )
	: m_Data( *a_Array )
	, m_Size( _Size )
{}

template < typename T >
Span< T >::Span( IContiguousCollection< T >& a_ContiguousCollection )
	: m_Data( a_ContiguousCollection.Data() )
	, m_Size( a_ContiguousCollection.Size() )
{}

namespace std
{
	template < typename T > auto begin( Span< T >& a_Span ) { return a_Span.Begin(); }
	template < typename T > auto begin( const Span< T >& a_Span ) { return a_Span.Begin(); }
	template < typename T > auto cbegin( const Span< T >& a_Span ) { return a_Span.CBegin(); }
	template < typename T > auto rbegin( Span< T >& a_Span ) { return a_Span.RBegin(); }
	template < typename T > auto rbegin( const Span< T >& a_Span ) { return a_Span.RBegin(); }
	template < typename T > auto crbegin( const Span< T >& a_Span ) { return a_Span.CRBegin(); }
	template < typename T > auto end( Span< T >& a_Span ) { return a_Span.End(); }
	template < typename T > auto end( const Span< T >& a_Span ) { return a_Span.End(); }
	template < typename T > auto cend( const Span< T >& a_Span ) { return a_Span.CEnd(); }
	template < typename T > auto rend( Span< T >& a_Span ) { return a_Span.REnd(); }
	template < typename T > auto rend( const Span< T >& a_Span ) { return a_Span.REnd(); }
	template < typename T > auto crend( const Span< T >& a_Span ) { return a_Span.CREnd(); }
}