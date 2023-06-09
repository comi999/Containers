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

	Array() = default;
	Array( const Array& ) = default;
	Array( Array&& ) = default;
	//Array( std::initializer_list< T > a_InitializerList );

	size_t Size() const { return m_Underlying.size(); }
	size_t MaxSize() const { return m_Underlying.max_size(); }
	IteratorType Begin() { return IteratorType( m_Underlying.begin() ); }
	CIteratorType Begin() const { return CIteratorType( m_Underlying.cbegin() ); }
	CIteratorType CBegin() const { return CIteratorType( m_Underlying.cbegin() ); }
	RIteratorType RBegin() { return RIteratorType( m_Underlying.rbegin() ); }
	CRIteratorType RBegin() const { return CRIteratorType( m_Underlying.crbegin() ); }
	CRIteratorType CRBegin() const { return CRIteratorType( m_Underlying.crbegin() ); }
	IteratorType End() { return IteratorType( m_Underlying.end() ); }
	CIteratorType End() const { return CIteratorType( m_Underlying.cend() ); }
	CIteratorType CEnd() const { return CIteratorType( m_Underlying.cend() ); }
	RIteratorType REnd() { return RIteratorType( m_Underlying.rend() ); }
	CRIteratorType REnd() const { return CRIteratorType( m_Underlying.crend() ); }
	CRIteratorType CREnd() const { return CRIteratorType( m_Underlying.crend() ); }

protected:

	EnumerableType IToEnumerable() { return EnumerableType( Size(), Begin(), End() ); }
	EnumeratorType IBegin() { return EnumeratorType( m_Underlying.begin() ); }
	EnumeratorType IEnd() { return EnumeratorType( m_Underlying.end() ); }
	EnumeratorType IRBegin() { return EnumeratorType( m_Underlying.rbegin() ); }
	EnumeratorType IREnd() { return EnumeratorType( m_Underlying.rend() ); }

	size_t ISize() const { return m_Underlying.size(); }
	size_t IMaxSize() const { return m_Underlying.max_size(); }
	T* IData() { return m_Underlying.data(); }
	T& IAt( size_t a_Index ) { return m_Underlying.at( a_Index ); }

private:

	UnderlyingType m_Underlying;
};

//template < typename T, size_t _Size >
//Array< T, _Size >::Array( std::initializer_list< T > a_InitializerList )
//{
//
//}