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
class Array : public ICollection< T >
{
public:

	using IteratorType = ArrayIterator< T, _Size >;
	using CIteratorType = ArrayCIterator< T, _Size >;
	using RIteratorType = ArrayRIterator< T, _Size >;
	using CRIteratorType = ArrayCRIterator< T, _Size >;
	using BaseType = ICollection< T >;
	using UnderlyingType = std::array< T, _Size >;

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

	Enumerator< T > IBegin() const { return Enumerator< T >( m_Underlying.begin() ); }
	Enumerator< T > IEnd() const { return Enumerator< T >( m_Underlying.end() ); }

private:

	UnderlyingType m_Underlying;
};