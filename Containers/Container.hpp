#pragma once
#include <any>

#define ENUMERATOR_ERROR_MESSAGE_FORWARD_ITERATOR "Iterator must be at least a forward iterator."
#define ENUMERATOR_ERROR_MESSAGE_BIDIRECTIONAL_ITERATOR "Iterator must be at least a bidirectional iterator."
#define ENUMERATOR_ERROR_MESSAGE_BOUNDING_ENUMERABLE_BIDIRECTIONAL "Non-bidirectional iterators require a bounding Enumerable."
#define ENUMERATOR_ERROR_MESSAGE_BOUNDING_ENUMERABLE_RANDOM_ACCESS "Non-random access iterators require a bounding Enumerable."
#define ENUMERATOR_ERROR_MESSAGE_OUT_OF_RANGE "Iterator outside of bounding range."
#define ENUMERATOR_ERROR_MESSAGE_NO_VTABLE "No virtual table set."

template < typename It >
static constexpr bool IsIterator()
{
	return std::_Is_iterator_v< It >;
}

template < typename It >
static constexpr bool IsConstIterator()
{
	if constexpr ( IsIterator< It >() )
	{
		return std::is_const_v< decltype( *std::declval< It >() ) >;
	}

	return false;
}

template < typename It >
static constexpr bool IsForwardIterator()
{
	if constexpr ( IsIterator< It >() ) { return std::is_base_of_v< std::forward_iterator_tag, typename std::iterator_traits< It >::iterator_category >; }
	else return false;
}

template < typename It >
static constexpr bool IsBidirectionalIterator()
{
	if constexpr ( IsIterator< It >() ) { return std::is_base_of_v< std::bidirectional_iterator_tag, typename std::iterator_traits< It >::iterator_category >; }
	else return false;
}

template < typename It >
static constexpr bool IsRandomAccessIterator()
{
	if constexpr ( IsIterator< It >() ) { return std::is_base_of_v< std::random_access_iterator_tag, typename std::iterator_traits< It >::iterator_category >; }
	else return false;
}

template < typename T >
class Enumerator;

template < typename T >
using CEnumerator = Enumerator< const T >;

template < typename T >
class IIterator
{
public:

	using ValueType = std::decay_t< T >;
	using ReferenceType = T&;
	using PointerType = T*;
	using EnumeratorType = Enumerator< T >;
	using CEnumeratorType = CEnumerator< T >;

	EnumeratorType ToEnumerator() { return IToEnumerator(); }
	CEnumeratorType ToEnumerator() const { return const_cast< IIterator* >( this )->IToEnumerator(); }
	CEnumeratorType ToCEnumerator() const { return const_cast< IIterator* >( this )->IToEnumerator(); }
	ReferenceType operator*() const { return IDereference(); };
	PointerType operator->() const { return IArrow(); };

protected:

	virtual EnumeratorType IToEnumerator() = 0;
	virtual ReferenceType IDereference() const = 0;
	virtual PointerType IArrow() const = 0;
};

template < typename T >
class IForwardIterator : public IIterator< T >
{
public:

	using BaseType = IIterator< T >;
	using TraitsType = std::iterator_traits< IForwardIterator >;
	using CategoryType = typename TraitsType::iterator_category;
	using DifferenceType = typename TraitsType::difference_type;
	using ValueType = typename TraitsType::value_type;
	using ReferenceType = typename TraitsType::reference;
	using PointerType = typename TraitsType::pointer;
	using EnumeratorType = typename BaseType::EnumeratorType;
	using CEnumeratorType = typename BaseType::CEnumeratorType;

	IForwardIterator& operator++() { this->IIncrement(); return *this; }

protected:

	virtual void IIncrement() = 0;
};

template < typename T >
class IBidirectionalIterator : public IForwardIterator< T >
{
public:

	using BaseType = IForwardIterator< T >;
	using TraitsType = std::iterator_traits< IBidirectionalIterator >;
	using CategoryType = typename TraitsType::iterator_category;
	using DifferenceType = typename TraitsType::difference_type;
	using ValueType = typename TraitsType::value_type;
	using ReferenceType = typename TraitsType::reference;
	using PointerType = typename TraitsType::pointer;
	using EnumeratorType = typename BaseType::EnumeratorType;
	using CEnumeratorType = typename BaseType::CEnumeratorType;

	IBidirectionalIterator& operator++() { this->IIncrement(); return *this; }
	IBidirectionalIterator& operator--() { this->IDecrement(); return *this; }

protected:

	virtual void IDecrement() = 0;
};

template < typename T >
class IRandomAccessIterator : public IBidirectionalIterator< T >
{
public:

	using BaseType = IBidirectionalIterator< T >;
	using TraitsType = std::iterator_traits< IRandomAccessIterator >;
	using CategoryType = typename TraitsType::iterator_category;
	using DifferenceType = typename TraitsType::difference_type;
	using ValueType = typename TraitsType::value_type;
	using ReferenceType = typename TraitsType::reference;
	using PointerType = typename TraitsType::pointer;
	using EnumeratorType = typename BaseType::EnumeratorType;
	using CEnumeratorType = typename BaseType::CEnumeratorType;

	IRandomAccessIterator& operator++() { this->IIncrement(); return *this; }
	IRandomAccessIterator& operator+=( DifferenceType a_Offset ) { this->ISeek( a_Offset ); return *this; }
	IRandomAccessIterator& operator--() { this->IDecrement(); return *this; }
	IRandomAccessIterator& operator-=( DifferenceType a_Offset ) { this->ISeek( -a_Offset ); return *this; }
	ReferenceType operator[]( DifferenceType a_Offset ) const { return this->IAt( a_Offset ); }

protected:

	virtual void ISeek( int64_t a_Offset ) = 0;
	virtual ReferenceType IAt( DifferenceType a_Offset ) const = 0;
};

template < typename _Underlying >
class ForwardIterator : public IForwardIterator< std::remove_reference_t< typename std::iterator_traits< _Underlying >::reference > >
{
public:

	using BaseType = IForwardIterator< std::remove_reference_t< typename std::iterator_traits< _Underlying >::reference > >;
	using TraitsType = std::iterator_traits< ForwardIterator >;
	using CategoryType = typename TraitsType::iterator_category;
	using DifferenceType = typename TraitsType::difference_type;
	using ValueType = typename TraitsType::value_type;
	using ReferenceType = typename TraitsType::reference;
	using PointerType = typename TraitsType::pointer;
	using UnderlyingType = _Underlying;
	using EnumeratorType = typename BaseType::EnumeratorType;
	using CEnumeratorType = typename BaseType::CEnumeratorType;

	ForwardIterator() = default;
	ForwardIterator( const ForwardIterator& ) = default;
	ForwardIterator( ForwardIterator&& ) = default;
	ForwardIterator( const UnderlyingType& a_Underlying ) : m_Underlying( a_Underlying ) {}
	ForwardIterator& operator=( const ForwardIterator& ) = default;
	ForwardIterator& operator=( ForwardIterator&& ) = default;
	ReferenceType operator*() const { return *m_Underlying; }
	PointerType operator->() const { if constexpr ( !std::is_pointer_v< UnderlyingType > ) { return m_Underlying.operator->(); } else { return m_Underlying; } }
	ForwardIterator& operator++() { ( void )++m_Underlying; return *this; }
	ForwardIterator operator++( int ) { return ForwardIterator( m_Underlying++ ); }
	bool operator==( const ForwardIterator& a_Iterator ) const { return m_Underlying == a_Iterator.m_Underlying; }
	bool operator!=( const ForwardIterator& a_Iterator ) const { return m_Underlying != a_Iterator.m_Underlying; }
	operator UnderlyingType& () { return m_Underlying; }
	operator const UnderlyingType& () const { return m_Underlying; }

protected:

	EnumeratorType IToEnumerator() { return EnumeratorType( m_Underlying ); }
	ReferenceType IDereference() const { return this->operator*(); }
	PointerType IArrow() const { return this->operator->(); }
	void IIncrement() { ( void )++m_Underlying; }

	UnderlyingType m_Underlying;
};

template < typename _Underlying >
class BidirectionalIterator : public IBidirectionalIterator< std::remove_reference_t< typename std::iterator_traits< _Underlying >::reference > >
{
public:

	using BaseType = IBidirectionalIterator< std::remove_reference_t< typename std::iterator_traits< _Underlying >::reference > >;
	using TraitsType = std::iterator_traits< BidirectionalIterator >;
	using CategoryType = typename TraitsType::iterator_category;
	using DifferenceType = typename TraitsType::difference_type;
	using ValueType = typename TraitsType::value_type;
	using ReferenceType = typename TraitsType::reference;
	using PointerType = typename TraitsType::pointer;
	using UnderlyingType = _Underlying;
	using EnumeratorType = typename BaseType::EnumeratorType;
	using CEnumeratorType = typename BaseType::CEnumeratorType;

	BidirectionalIterator() = default;
	BidirectionalIterator( const BidirectionalIterator& ) = default;
	BidirectionalIterator( BidirectionalIterator&& ) = default;
	BidirectionalIterator( const UnderlyingType& a_Underlying ) : m_Underlying( a_Underlying ) {}
	BidirectionalIterator& operator=( const BidirectionalIterator& ) = default;
	BidirectionalIterator& operator=( BidirectionalIterator&& ) = default;
	ReferenceType operator*() const { return *m_Underlying; }
	PointerType operator->() const { if constexpr ( !std::is_pointer_v< UnderlyingType > ) { return m_Underlying.operator->(); } else { return m_Underlying; } }
	BidirectionalIterator& operator++() { ( void )++this->m_Underlying; return *this; }
	BidirectionalIterator operator++( int ) { return BidirectionalIterator( this->m_Underlying++ ); }
	BidirectionalIterator& operator--() { ( void )--this->m_Underlying; return *this; }
	BidirectionalIterator operator--( int ) { return BidirectionalIterator( this->m_Underlying-- ); }
	bool operator==( const BidirectionalIterator& a_Iterator ) const { return this->m_Underlying == a_Iterator.m_Underlying; }
	bool operator!=( const BidirectionalIterator& a_Iterator ) const { return this->m_Underlying != a_Iterator.m_Underlying; }
	operator UnderlyingType& () { return this->m_Underlying; }
	operator const UnderlyingType& () const { return this->m_Underlying; }

protected:

	EnumeratorType IToEnumerator() { return EnumeratorType( m_Underlying ); }
	ReferenceType IDereference() const { return this->operator*(); }
	PointerType IArrow() const { return this->operator->(); }
	void IIncrement() { ( void )++m_Underlying; }
	void IDecrement() { ( void )--m_Underlying; }

	UnderlyingType m_Underlying;
};

template < typename _Underlying >
class RandomAccessIterator : public IRandomAccessIterator< std::remove_reference_t< typename std::iterator_traits< _Underlying >::reference > >
{
public:

	using BaseType = IRandomAccessIterator< std::remove_reference_t< typename std::iterator_traits< _Underlying >::reference > >;
	using TraitsType = std::iterator_traits< RandomAccessIterator >;
	using CategoryType = typename TraitsType::iterator_category;
	using DifferenceType = typename TraitsType::difference_type;
	using ValueType = typename TraitsType::value_type;
	using ReferenceType = typename TraitsType::reference;
	using PointerType = typename TraitsType::pointer;
	using UnderlyingType = _Underlying;
	using EnumeratorType = typename BaseType::EnumeratorType;
	using CEnumeratorType = typename BaseType::CEnumeratorType;

	RandomAccessIterator() = default;
	RandomAccessIterator( const RandomAccessIterator& ) = default;
	RandomAccessIterator( RandomAccessIterator&& ) = default;
	RandomAccessIterator( const UnderlyingType& a_Underlying ) : m_Underlying( a_Underlying ) {}
	RandomAccessIterator& operator=( const RandomAccessIterator& ) = default;
	RandomAccessIterator& operator=( RandomAccessIterator&& ) = default;
	ReferenceType operator*() const { return *m_Underlying; }
	PointerType operator->() const { if constexpr ( !std::is_pointer_v< UnderlyingType > ) { return m_Underlying.operator->(); } else { return m_Underlying; } }
	RandomAccessIterator& operator++() { ++this->m_Underlying; return *this; }
	RandomAccessIterator operator++( int ) { return RandomAccessIterator( this->m_Underlying++ ); }
	RandomAccessIterator& operator+=( DifferenceType a_Offset ) { this->m_Underlying += a_Offset; return *this; }
	RandomAccessIterator operator+( DifferenceType a_Offset ) const { return RandomAccessIterator( this->m_Underlying + a_Offset ); }
	RandomAccessIterator& operator--() { --this->m_Underlying; return *this; }
	RandomAccessIterator operator--( int ) { return RandomAccessIterator( this->m_Underlying-- ); }
	RandomAccessIterator& operator-=( DifferenceType a_Offset ) { this->m_Underlying -= a_Offset; return *this; }
	RandomAccessIterator operator-( DifferenceType a_Offset ) const { return RandomAccessIterator( this->m_Underlying - a_Offset ); }
	DifferenceType operator-( const RandomAccessIterator& a_Iterator ) const { return this->m_Underlying - a_Iterator.m_Underlying; }
	bool operator>( const RandomAccessIterator& a_Iterator ) const { return this->m_Underlying > a_Iterator.m_Underlying; }
	bool operator>=( const RandomAccessIterator& a_Iterator ) const { return this->m_Underlying >= a_Iterator.m_Underlying; }
	bool operator<( const RandomAccessIterator& a_Iterator ) const { return this->m_Underlying < a_Iterator.m_Underlying; }
	bool operator<=( const RandomAccessIterator& a_Iterator ) const { return this->m_Underlying <= a_Iterator.m_Underlying; }
	bool operator==( const RandomAccessIterator& a_Iterator ) const { return this->m_Underlying == a_Iterator.m_Underlying; }
	bool operator!=( const RandomAccessIterator& a_Iterator ) const { return this->m_Underlying != a_Iterator.m_Underlying; }
	ReferenceType operator[]( DifferenceType a_Offset ) const { return this->m_Underlying[ a_Offset ]; }
	operator UnderlyingType& () { return this->m_Underlying; }
	operator const UnderlyingType& () const { return this->m_Underlying; }

protected:

	EnumeratorType IToEnumerator() { return EnumeratorType( m_Underlying ); }
	ReferenceType IDereference() const { return this->operator*(); }
	PointerType IArrow() const { return this->operator->(); }
	void IIncrement() { ( void )++m_Underlying; }
	void IDecrement() { ( void )--m_Underlying; }
	void ISeek( int64_t a_Offset ) { m_Underlying += a_Offset; }
	ReferenceType IAt( DifferenceType a_Offset ) const { return m_Underlying[ a_Offset ]; }

	UnderlyingType m_Underlying;
};

template < typename T >
class Enumerable;

template < typename T >
using CEnumerable = Enumerable< const T >;

enum class EnumeratorOperation
{
	Reverse,
	PreIncrement,
	PostIncrement,
	PreDecrement,
	PostDecrement,
	Addition,
	AdditionEqual,
	Subtraction,
	SubtractionEqual,
	Difference,
	Dereference,
	Arrow,
	Greater,
	GreaterEqual,
	Lesser,
	LesserEqual,
	Index,
	Equal,
	Unequal
};

using EnumeratorVTable = bool( * )( EnumeratorOperation, std::any&, std::any*, uintptr_t, uintptr_t, uintptr_t );

namespace std
{
	template < typename It >
	struct is_enumerator { static constexpr bool value = false; };

	template < typename It >
	struct is_enumerator< Enumerator< It > > { static constexpr bool value = true; };

	template < typename It >
	static constexpr bool is_enumerator_v = is_enumerator< It >::value;
}

template < typename T >
class Enumerator : public IRandomAccessIterator< T >
{
public:

	using BaseType = IRandomAccessIterator< T >;
	using TraitsType = std::iterator_traits< Enumerator< T > >;
	using CategoryType = typename TraitsType::iterator_category;
	using DifferenceType = typename TraitsType::difference_type;
	using ValueType = typename TraitsType::value_type;
	using ReferenceType = typename TraitsType::reference;
	using PointerType = typename TraitsType::pointer;
	using EnumeratorType = typename BaseType::EnumeratorType;
	using CEnumeratorType = typename BaseType::CEnumeratorType;

	using VTableType = EnumeratorVTable;
	using EnumerableType = Enumerable< ValueType >;

	Enumerator() = default;
	Enumerator( const Enumerator& ) = default;
	Enumerator( Enumerator&& ) = default;
	Enumerator& operator=( const Enumerator& ) = default;
	Enumerator& operator=( Enumerator&& ) = default;

	template < typename = std::enable_if_t< std::is_const_v< T > > >
	Enumerator( const Enumerator< std::decay_t< T > >& a_Enumerator ) 
		: m_VTable( a_Enumerator.m_VTable )
		, m_Iterator( a_Enumerator.m_Iterator )
		, m_Enumerable( a_Enumerator.m_Enumerable )
	{}

	template < typename = std::enable_if_t< std::is_const_v< T > > >
	Enumerator( Enumerator< std::decay_t< T > >&& a_Enumerator )
		: m_VTable( a_Enumerator.m_VTable )
		, m_Iterator( std::move( a_Enumerator.m_Iterator ) )
		, m_Enumerable( a_Enumerator.m_Enumerable )
	{}

private:

	template < typename It >
	static constexpr bool has_compatible_constness_v = std::is_const_v< T > || !std::is_const_v< std::remove_reference_t< decltype( *std::declval< It >() ) > >;

	template < typename It >
	static constexpr bool is_same_v = std::is_same_v< std::decay_t< T >, std::decay_t< decltype( *std::declval< It >() ) > >;

	template < typename It >
	static constexpr bool is_compatible_iterator_v = !std::is_enumerator_v< std::remove_reference_t< It > > && has_compatible_constness_v< std::remove_reference_t< It > > && is_same_v< std::remove_reference_t< It > > && !std::is_const_v< std::remove_reference_t< It > >;

public:

	template < typename It, typename = std::enable_if_t< is_compatible_iterator_v< It > > >
	Enumerator( It& a_Iterator )
		: m_VTable( Operator< It, false > )
		, m_Iterator( a_Iterator )
		, m_Enumerable( nullptr )
	{}

	template < typename It, typename = std::enable_if_t< is_compatible_iterator_v< It > > >
	Enumerator( It&& a_Iterator )
		: m_VTable( Operator< It, false > )
		, m_Iterator( std::forward< It >( a_Iterator ) )
		, m_Enumerable( nullptr )
	{}

private:

	Enumerator( VTableType a_VTable, std::any&& a_Iterator, const EnumerableType* a_Enumerable )
		: m_VTable( a_VTable )
		, m_Iterator( std::forward< std::any >( a_Iterator ) )
		, m_Enumerable( a_Enumerable )
	{}

public:

	Enumerator ToEnumerator() { return Enumerator( *this ); }

	Enumerator Reverse() const
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		VTableType Table;
		m_VTable(
			EnumeratorOperation::Reverse,
			const_cast< std::any& >( m_Iterator ),
			0,
			( uintptr_t )&Table,
			0,
			0
		);

		std::any Copy = m_Iterator;
		return Enumerator( Table, std::move( Copy ), m_Enumerable );
	}

	template < typename It, typename = std::enable_if_t< is_compatible_iterator_v< It > > >
	Enumerator& operator=( It& a_Iterator );

	template < typename It, typename = std::enable_if_t< is_compatible_iterator_v< It > > >
	Enumerator& operator=( It&& a_Iterator );

	Enumerator& operator++()
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		m_VTable( 
			EnumeratorOperation::PreIncrement,
			m_Iterator,
			0, 
			0, 
			0,
			0 
		);

		return *this;
	}

	Enumerator operator++( int )
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		std::any Result;
		VTableType Table;

		m_VTable( 
			EnumeratorOperation::PostIncrement,
			m_Iterator,
			&Result, 
			( uintptr_t )&Table, 
			0, 
			0
		);

		return Enumerator( Table, std::move( Result ), m_Enumerable );
	}

	Enumerator& operator--()
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		m_VTable( 
			EnumeratorOperation::PreDecrement,
			m_Iterator, 
			0, 
			( uintptr_t )m_Enumerable, 
			0,
			0
		);

		return *this;
	}

	Enumerator operator--( int )
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		std::any Result;
		VTableType Table;

		m_VTable( 
			EnumeratorOperation::PostDecrement,
			m_Iterator,
			&Result, 
			( uintptr_t )&Table,
			( uintptr_t )m_Enumerable, 
			0
		);

		return Enumerator( Table, std::move( Result ), m_Enumerable );
	}

	Enumerator operator+( DifferenceType a_Offset ) const
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		std::any Result;
		VTableType Table;

		if ( a_Offset < 0 )
		{
			m_VTable(
				EnumeratorOperation::Subtraction,
				m_Iterator,
				&Result,
				( uintptr_t )-a_Offset,
				( uintptr_t )&Table,
				( uintptr_t )m_Enumerable
			);
		}
		else
		{
			m_VTable(
				EnumeratorOperation::Addition,
				m_Iterator,
				&Result,
				( uintptr_t )a_Offset,
				( uintptr_t )&Table,
				0
			);
		}

		return Enumerator( Table, std::move( Result ), m_Enumerable );
	}	
	
	Enumerator& operator+=( DifferenceType a_Offset )
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		if ( a_Offset < 0 )
		{
			m_VTable(
				EnumeratorOperation::SubtractionEqual,
				const_cast< std::any& >( m_Iterator ),
				0,
				( uintptr_t )-a_Offset,
				( uintptr_t )m_Enumerable,
				0
			);
		}
		else
		{
			m_VTable(
				EnumeratorOperation::AdditionEqual,
				const_cast< std::any& >( m_Iterator ),
				0,
				( uintptr_t )a_Offset,
				0,
				0
			);
		}

		return *this;
	}

	Enumerator operator-( DifferenceType a_Offset ) const
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		std::any Result;
		VTableType Table;

		if ( a_Offset < 0 )
		{
			m_VTable(
				EnumeratorOperation::Addition,
				m_Iterator,
				&Result,
				( uintptr_t )-a_Offset,
				( uintptr_t )&Table,
				0
			);
		}
		else
		{
			m_VTable(
				EnumeratorOperation::Subtraction,
				m_Iterator,
				&Result,
				( uintptr_t )a_Offset,
				( uintptr_t )&Table,
				( uintptr_t )m_Enumerable
			);
		}

		return Enumerator( Table, std::move( Result ), m_Enumerable );
	}

	Enumerator& operator-=( DifferenceType a_Offset )
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		if ( a_Offset < 0 )
		{
			m_VTable(
				EnumeratorOperation::AdditionEqual,
				m_Iterator,
				0,
				( uintptr_t )-a_Offset,
				0,
				0
			);
		}
		else
		{
			m_VTable(
				EnumeratorOperation::SubtractionEqual,
				m_Iterator,
				0,
				( uintptr_t )a_Offset,
				( uintptr_t )m_Enumerable,
				0
			);
		}

		return *this;
	}

	DifferenceType operator-( const Enumerator& a_Enumerator ) const
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		DifferenceType Difference;

		m_VTable(
			EnumeratorOperation::Difference,
			m_Iterator,
			&a_Enumerator.m_Iterator,
			( uintptr_t )m_Enumerable,
			( uintptr_t )&Difference,
			0
		);

		return Difference;
	}

	ReferenceType operator*() const
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		PointerType Result;

		m_VTable(
			EnumeratorOperation::Dereference,
			m_Iterator,
			0,
			0,
			0,
			( uintptr_t )&Result
		);

		return *Result;
	}

	PointerType operator->() const
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		PointerType Result;

		m_VTable(
			EnumeratorOperation::Arrow,
			m_Iterator,
			0,
			0,
			0,
			( uintptr_t )&Result
		);

		return Result;
	}

	bool operator>( const Enumerator< T >& a_Enumerator ) const
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		return m_VTable( 
			EnumeratorOperation::Greater, 
			m_Iterator,
			&a_Enumerator.m_Iterator,
			0, 
			0, 
			0
		);
	}

	bool operator>=( const Enumerator< T >& a_Enumerator ) const
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		return m_VTable(
			EnumeratorOperation::GreaterEqual,
			m_Iterator,
			&a_Enumerator.m_Iterator,
			0,
			0,
			0
		);
	}

	bool operator<( const Enumerator< T >& a_Enumerator ) const
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		return m_VTable(
			EnumeratorOperation::Lesser,
			m_Iterator,
			&a_Enumerator.m_Iterator,
			0,
			0,
			0
		);
	}

	bool operator<=( const Enumerator< T >& a_Enumerator ) const
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		return m_VTable(
			EnumeratorOperation::LesserEqual,
			m_Iterator,
			&a_Enumerator.m_Iterator,
			0,
			0,
			0
		);
	}

	ReferenceType operator[]( DifferenceType a_Offset ) const
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		PointerType Result;

		m_VTable(
			EnumeratorOperation::Index,
			m_Iterator,
			0,
			( uintptr_t )( uint64_t )a_Offset,
			( uintptr_t )m_Enumerable,
			( uintptr_t )&Result
		);

		return *Result;
	}

	bool operator==( const Enumerator< T >& a_Enumerator ) const
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		return m_VTable(
			EnumeratorOperation::Equal,
			m_Iterator,
			&a_Enumerator.m_Iterator,
			0,
			0,
			0
		);
	}

	bool operator!=( const Enumerator< T >& a_Enumerator ) const
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		return m_VTable(
			EnumeratorOperation::Unequal,
			m_Iterator,
			&a_Enumerator.m_Iterator,
			0,
			0,
			0
		);
	}

	const std::any& AsAny() const { return m_Iterator; }
	const std::type_info& GetType() const { return m_Iterator.type(); }

protected:
	
	EnumeratorType IToEnumerator() { return *this; }
	ReferenceType IDereference() const { return this->operator*(); }
	PointerType IArrow() const { return this->operator->(); }
	void IIncrement() { ( void )++*this; }
	void IDecrement() { ( void )--* this; }
	void ISeek( DifferenceType a_Offset ) { *this += a_Offset; }
	ReferenceType IAt( DifferenceType a_Offset ) const { return this->operator[]( a_Offset ); }

private:

	template < class > friend class Enumerator;
	template < class > friend class Enumerable;

	template < typename It, bool _Reversed >
	static bool Operator( EnumeratorOperation a_Operation, std::any& a_Iterator0, std::any* a_Iterator1, uintptr_t a_Arg0, uintptr_t a_Arg1, uintptr_t a_Arg2 )
	{
		_ASSERT_EXPR( a_Iterator0.has_value(), "Cannot perform an operation on an empty enumerator." );
		_ASSERT_EXPR( IsForwardIterator< It >(), "Iterator type must be at least forward iterator." );

		switch ( a_Operation )
		{
		case EnumeratorOperation::Reverse:
		{
			*( VTableType* )a_Arg0 = Operator< It, !_Reversed >;
			break;
		}
		case EnumeratorOperation::PreIncrement:
		{
			if constexpr ( !_Reversed )
			{
				Operator_PreIncrement< It >( ( It& )a_Iterator0 );
			}
			else
			{
				Operator_PreDecrement< It >( ( It& )a_Iterator0, ( const EnumerableType* )a_Arg0 );
			}

			break;
		}
		case EnumeratorOperation::PostIncrement:
		{
			if constexpr ( !_Reversed )
			{
				It Result = Operator_PostIncrement< It >( ( It& )a_Iterator0 );
				( It& )*a_Iterator1 = std::move( Result );
				*( VTableType* )a_Arg0 = Operator< It, _Reversed >;
			}
			else
			{
				It Result = Operator_PostDecrement< It >( ( It& )a_Iterator0, ( const EnumerableType* )a_Arg0 );
				( It& )*a_Iterator1 = std::move( Result );
				*( VTableType* )a_Arg0 = Operator< It, _Reversed >;
			}

			break;
		}
		case EnumeratorOperation::PreDecrement:
		{
			if constexpr ( !_Reversed )
			{
				Operator_PreDecrement< It >( ( It& )a_Iterator0, ( const EnumerableType* )a_Arg0 );
			}
			else
			{
				Operator_PreIncrement< It >( ( It& )a_Iterator0 );
			}

			break;
		}
		case EnumeratorOperation::PostDecrement:
		{
			if constexpr ( !_Reversed )
			{
				It Result = Operator_PostDecrement< It >( ( It& )a_Iterator0, ( const EnumerableType* )a_Arg1 );
				( It& )*a_Iterator1 = std::move( Result );
				*( VTableType* )a_Arg0 = Operator< It, _Reversed >;
			}
			else
			{
				It Result = Operator_PostIncrement< It >( ( It& )a_Iterator0 );
				( It& )*a_Iterator1 = std::move( Result );
				*( VTableType* )a_Arg0 = Operator< It, _Reversed >;
			}

			break;
		}
		case EnumeratorOperation::Addition:
		{
			if constexpr ( !_Reversed )
			{
				It Result = Operator_Addition< It >( ( It& )a_Iterator0, ( size_t )a_Arg0 );
				*a_Iterator1 = std::any( std::move( Result ) );
				*( VTableType* )a_Arg1 = Operator< It, _Reversed >;
			}
			else
			{
				It Result = Operator_Subtraction< It >( ( It& )a_Iterator0, ( size_t )a_Arg0, ( const EnumerableType* )a_Arg2 );
				*a_Iterator1 = std::any( std::move( Result ) );
				*( VTableType* )a_Arg1 = Operator< It, _Reversed >;
			}

			break;
		}
		case EnumeratorOperation::AdditionEqual:
		{
			if constexpr ( !_Reversed )
			{
				Operator_AdditionEqual< It >( ( It& )a_Iterator0, ( size_t )a_Arg0 );
			}
			else
			{
				Operator_SubtractionEqual< It >( ( It& )a_Iterator0, ( size_t )a_Arg0, ( const EnumerableType* )a_Arg1 );
			}

			break;
		}
		case EnumeratorOperation::Subtraction:
		{
			if constexpr ( !_Reversed )
			{
				It Result = Operator_Subtraction< It >( ( It& )a_Iterator0, ( size_t )a_Arg0, ( const EnumerableType* )a_Arg2 );
				*a_Iterator1 = std::any( std::move( Result ) );
				*( VTableType* )a_Arg1 = Operator< It, _Reversed >;
			}
			else
			{
				It Result = Operator_Addition< It >( ( It& )a_Iterator0, ( size_t )a_Arg0 );
				*a_Iterator1 = std::any( std::move( Result ) );
				*( VTableType* )a_Arg1 = Operator< It, _Reversed >;
			}

			break;
		}
		case EnumeratorOperation::SubtractionEqual:
		{
			if constexpr ( !_Reversed )
			{
				Operator_SubtractionEqual< It >( ( It& )a_Iterator0, ( size_t )a_Arg0, ( const EnumerableType* )a_Arg1 );
			}
			else
			{
				Operator_AdditionEqual< It >( ( It& )a_Iterator0, ( size_t )a_Arg0 );
			}

			break;
		}
		case EnumeratorOperation::Difference:
		{
			if constexpr ( !_Reversed )
			{
				*( int64_t* )a_Arg1 = Operator_Difference< It >( ( It& )a_Iterator0, ( It& )*a_Iterator1, ( const EnumerableType* )a_Arg0 );
			}	
			else
			{
				*( int64_t* )a_Arg1 = -Operator_Difference< It >( ( It& )a_Iterator0, ( It& )*a_Iterator1, ( const EnumerableType* )a_Arg0 );
			}

			break;
		}
		case EnumeratorOperation::Dereference:
		{
			if constexpr ( !_Reversed )
			{
				*( T** )a_Arg2 = const_cast< T* >( &Operator_Dereference< It >( ( It& )a_Iterator0 ) );
			}
			else
			{
				It Result = Operator_Subtraction< It >( ( It& )a_Iterator0, 1, ( const EnumerableType* )a_Arg0 );
				*( T** )a_Arg2 = const_cast< T* >( &Operator_Dereference< It >( Result ) );
			}
			
			break;
		}
		case EnumeratorOperation::Arrow:
		{
			if constexpr ( !_Reversed )
			{
				*( T** )a_Arg2 = const_cast< T* >( Operator_Arrow< It >( ( It& )a_Iterator0 ) );
			}
			else
			{
				It Result = Operator_Subtraction< It >( ( It& )a_Iterator0, 1, ( const EnumerableType* )a_Arg0 );
				*( T** )a_Arg2 = const_cast< T* >( Operator_Arrow< It >( Result ) );
			}

			break;
		}
		case EnumeratorOperation::Greater:
		{
			if constexpr ( !_Reversed )
			{
				return Operator_Greater< It >( ( It& )a_Iterator0, ( It& )*a_Iterator1, ( const EnumerableType* )a_Arg0 );
			}
			else
			{
				return Operator_Lesser< It >( ( It& )a_Iterator0, ( It& )*a_Iterator1, ( const EnumerableType* )a_Arg0 );
			}
		}
		case EnumeratorOperation::GreaterEqual:
		{
			if constexpr ( !_Reversed )
			{
				return Operator_GreaterEqual< It >( ( It& )a_Iterator0, ( It& )*a_Iterator1, ( const EnumerableType* )a_Arg0 );
			}
			else
			{
				return Operator_LesserEqual< It >( ( It& )a_Iterator0, ( It& )*a_Iterator1, ( const EnumerableType* )a_Arg0 );
			}
		}
		case EnumeratorOperation::Lesser:
		{
			if constexpr ( !_Reversed )
			{
				return Operator_Lesser< It >( ( It& )a_Iterator0, ( It& )*a_Iterator1, ( const EnumerableType* )a_Arg0 );
			}
			else
			{
				return Operator_Greater< It >( ( It& )a_Iterator0, ( It& )*a_Iterator1, ( const EnumerableType* )a_Arg0 );
			}
		}
		case EnumeratorOperation::LesserEqual:
		{
			if constexpr ( !_Reversed )
			{
				return Operator_LesserEqual< It >( ( It& )a_Iterator0, ( It& )*a_Iterator1, ( const EnumerableType* )a_Arg0 );
			}
			else
			{
				return Operator_GreaterEqual< It >( ( It& )a_Iterator0, ( It& )*a_Iterator1, ( const EnumerableType* )a_Arg0 );
			}
		}
		case EnumeratorOperation::Equal:
		{
			return Operator_Equal< It >( ( It& )a_Iterator0, ( It& )*a_Iterator1 );
		}
		case EnumeratorOperation::Unequal:
		{
			return Operator_Inequal< It >( ( It& )a_Iterator0, ( It& )*a_Iterator1 );
		}
		case EnumeratorOperation::Index:
		{
			if constexpr ( !_Reversed )
			{
				*( T** )a_Arg2 = const_cast< T* >( &Operator_Index< It >( ( It& )a_Iterator0, ( int64_t )a_Arg0, ( const EnumerableType* )a_Arg1 ) );
			}
			else
			{
				*( T** )a_Arg2 = const_cast< T* >( &Operator_Index< It >( ( It& ) a_Iterator0, -( int64_t )a_Arg0 - 1, ( const EnumerableType* )a_Arg1 ) );
			}

			break;
		}
		}

		return true;
	}

	template < typename It > static decltype( auto ) Operator_PreIncrement( It& a_Iterator );
	template < typename It > static decltype( auto ) Operator_PostIncrement( It& a_Iterator );
	template < typename It > static decltype( auto ) Operator_PreDecrement( It& a_Iterator, const EnumerableType* a_Bounds );
	template < typename It > static decltype( auto ) Operator_PostDecrement( It& a_Iterator, const EnumerableType* a_Bounds );
	template < typename It > static decltype( auto ) Operator_Addition( It& a_Iterator, size_t a_Offset );
	template < typename It > static decltype( auto ) Operator_AdditionEqual( It& a_Iterator, size_t a_Offset );
	template < typename It > static decltype( auto ) Operator_Subtraction( It& a_Iterator, size_t a_Offset, const EnumerableType* a_Bounds );
	template < typename It > static decltype( auto ) Operator_SubtractionEqual( It& a_Iterator, size_t a_Offset, const EnumerableType* a_Bounds );
	template < typename It > static decltype( auto ) Operator_Difference( It& a_Left, It& a_Right, const EnumerableType* a_Bounds );
	template < typename It > static decltype( auto ) Operator_Dereference( It& a_Iterator );
	template < typename It > static decltype( auto ) Operator_Arrow( It& a_Iterator );
	template < typename It > static decltype( auto ) Operator_Greater( It& a_Left, It& a_Right, const EnumerableType* a_Bounds );
	template < typename It > static decltype( auto ) Operator_GreaterEqual( It& a_Left, It& a_Right, const EnumerableType* a_Bounds );
	template < typename It > static decltype( auto ) Operator_Lesser( It& a_Left, It& a_Right, const EnumerableType* a_Bounds );
	template < typename It > static decltype( auto ) Operator_LesserEqual( It& a_Left, It& a_Right, const EnumerableType* a_Bounds );
	template < typename It > static decltype( auto ) Operator_Index( It& a_Any, int64_t  a_Offset, const EnumerableType* a_Bounds );
	template < typename It > static decltype( auto ) Operator_Equal( It& a_Left, It& a_Right );
	template < typename It > static decltype( auto ) Operator_Inequal( It& a_Left, It& a_Right );

	VTableType            m_VTable;
	mutable std::any      m_Iterator;
	const EnumerableType* m_Enumerable;
};

template < typename T >
class ICollection
{
public:

	using ValueType = std::decay_t< T >;
	using IteratorType = Enumerator< T >;
	using CIteratorType = CEnumerator< T >;
	using EnumeratorType = Enumerator< T >;
	using CEnumeratorType = CEnumerator< T >;
	using EnumerableType = Enumerable< T >;
	using CEnumerableType = CEnumerable< T >;

	IteratorType Begin() { return IBegin(); }
	CIteratorType Begin() const { return const_cast< ICollection* >( this )->IBegin(); }
	CIteratorType CBegin() const { return const_cast< ICollection* >( this )->IBegin(); }
	IteratorType End() { return IEnd(); }
	CIteratorType End() const { return const_cast< ICollection* >( this )->IEnd(); }
	CIteratorType CEnd() const { return const_cast< ICollection* >( this )->IEnd(); }
	EnumerableType ToEnumerable() { return IToEnumerable(); }
	CEnumerableType ToEnumerable() const { return const_cast< ICollection* >( this )->IToEnumerable(); }
	CEnumerableType ToCEnumerable() const { return const_cast< ICollection* >( this )->IToEnumerable(); }
	size_t Size() const { return ISize(); }
	size_t MaxSize() const { return IMaxSize(); }

protected:

	virtual EnumerableType IToEnumerable() = 0;
	virtual size_t ISize() const = 0;
	virtual size_t IMaxSize() const = 0;
	virtual EnumeratorType IBegin() = 0;
	virtual EnumeratorType IEnd() = 0;
};

template < typename T >
class IContiguousCollection : public ICollection< T >
{
public:

	using BaseType = ICollection< T >;
	using IteratorType = typename BaseType::IteratorType;
	using CIteratorType = typename BaseType::CIteratorType;
	using RIteratorType = Enumerator< T >;
	using CRIteratorType = CEnumerator< T >;
	using EnumeratorType = typename BaseType::EnumeratorType;
	using CEnumeratorType = typename BaseType::CEnumeratorType;
	using EnumerableType = typename BaseType::EnumerableType;
	using CEnumerableType = typename BaseType::CEnumerableType;

	RIteratorType RBegin() { return IRBegin(); }
	CRIteratorType RBegin() const { return const_cast< ICollection* >( this )->IRBegin(); }
	CRIteratorType CRBegin() const { return const_cast< ICollection* >( this )->IRBegin(); }
	RIteratorType REnd() { return IREnd(); }
	CRIteratorType REnd() const { return const_cast< ICollection* >( this )->IREnd(); }
	CRIteratorType CREnd() const { return const_cast< ICollection* >( this )->IREnd(); }

	T* Data() { return IData(); }
	const T* Data() const { return const_cast< IContiguousCollection* >( this )->IData(); }
	T& At( size_t a_Index ) { return IAt( a_Index ); }
	const T& At( size_t a_Index ) const { return const_cast< IContiguousCollection* >( this )->IAt( a_Index ); }

protected:

	virtual EnumeratorType IRBegin() = 0;
	virtual EnumeratorType IREnd() = 0;
	virtual T* IData() = 0;
	virtual T& IAt( size_t a_Index ) = 0;
};

template < typename T >
class Enumerable : public ICollection< T >
{
public:

	using BaseType = ICollection< T >;
	using IteratorType = Enumerator< T >;
	using CIteratorType = CEnumerator< T >;
	using RIteratorType = Enumerator< T >;
	using CRIteratorType = CEnumerator< T >;
	using EnumeratorType = typename BaseType::EnumeratorType;
	using CEnumeratorType = typename BaseType::CEnumeratorType;
	using EnumerableType = typename BaseType::EnumerableType;
	using CEnumerableType = typename BaseType::CEnumerableType;

	Enumerable() = default;
	Enumerable( const Enumerable& ) = default;
	Enumerable( Enumerable&& ) = default;

	template < typename = std::enable_if_t< std::is_const_v< T > > >
	Enumerable( const Enumerable< std::remove_const_t< T > >& a_Enumerable )
		: m_Size( a_Enumerable.m_Size )
		, m_Begin( a_Enumerable.m_Begin )
		, m_End( a_Enumerable.m_End )
	{}

	Enumerable( size_t a_Size, const EnumeratorType& a_Begin, const EnumeratorType& a_End )
		: m_Size( a_Size )
		, m_Begin( a_Begin )
		, m_End( a_End )
	{}

	template < typename It, typename = std::enable_if_t< !std::is_enumerator_v< It > > >
	Enumerable( It& a_Begin, It& a_End )
		: m_Size( std::distance( a_Begin, a_End ) )
		, m_Begin( a_Begin )
		, m_End( a_End )
	{}

	template < typename It, typename = std::enable_if_t< !std::is_enumerator_v< It > > >
	Enumerable( size_t a_Size, It& a_Begin, It& a_End )
		: m_Size( a_Size )
		, m_Begin( a_Begin )
		, m_End( a_End )
	{}

	const std::type_info& GetType() const { return m_Begin.GetType(); }
	EnumerableType ToEnumerable() { return *this; }
	CEnumerableType ToEnumerable() const { return *this; }
	CEnumerableType ToCEnumerable() const { return *this; }
	size_t Size() const { return m_Size; }
	size_t MaxSize() const { return static_cast< size_t >( -1 ); }

	IteratorType Begin() { return m_Begin; }
	CIteratorType Begin() const { return m_Begin; }
	CIteratorType CBegin() const { return m_Begin; }
	IteratorType End() { return m_End; }
	CIteratorType End() const { return m_End; }
	CIteratorType CEnd() const { return m_End; }

protected:

	size_t ISize() const { return Size(); }
	size_t IMaxSize() const { return MaxSize(); }
	EnumerableType IToEnumerable() { return *this; }
	IteratorType IBegin() { return m_Begin; }
	IteratorType IEnd() { return m_End; }

private:

	template < class > friend class Enumerable;
	
	size_t m_Size;
	IteratorType m_Begin;
	IteratorType m_End;
};

namespace std
{
	template < typename T >
	struct iterator_traits< IForwardIterator< T > >
	{
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = std::decay_t< T >;
		using reference = T&;
		using pointer = T*;
	};

	template < typename T >
	struct iterator_traits< IBidirectionalIterator< T > >
	{
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = std::decay_t< T >;
		using reference = T&;
		using pointer = T*;
	};

	template < typename T >
	struct iterator_traits< IRandomAccessIterator< T > >
	{
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = std::decay_t< T >;
		using reference = T&;
		using pointer = T*;
	};

	template < typename T >
	struct iterator_traits< ForwardIterator< T > >
	{
		using iterator_category = std::forward_iterator_tag;
		using difference_type = typename std::iterator_traits< T >::difference_type;
		using value_type = typename std::iterator_traits< T >::value_type;
		using reference = typename std::iterator_traits< T >::reference;
		using pointer = typename std::iterator_traits< T >::pointer;
	};

	template < typename T >
	struct iterator_traits< BidirectionalIterator< T > >
	{
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = typename std::iterator_traits< T >::difference_type;
		using value_type = typename std::iterator_traits< T >::value_type;
		using reference = typename std::iterator_traits< T >::reference;
		using pointer = typename std::iterator_traits< T >::pointer;
	};

	template < typename T >
	struct iterator_traits< RandomAccessIterator< T > >
	{
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = typename std::iterator_traits< T >::difference_type;
		using value_type = typename std::iterator_traits< T >::value_type;
		using reference = typename std::iterator_traits< T >::reference;
		using pointer = typename std::iterator_traits< T >::pointer;
	};

	template < typename T >
	struct iterator_traits< Enumerator< T > >
	{
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = std::decay_t< T >;
		using reference = T&;
		using pointer = T*;
	};
}

template < typename T >
template < typename It, typename >
Enumerator< T >& Enumerator< T >::operator=( It& a_Iterator )
{
	m_VTable = Operator< It, false >;
	m_Iterator = a_Iterator;
	m_Enumerable = 0;
	return *this;
}

template < typename T >
template < typename It, typename >
Enumerator< T >& Enumerator< T >::operator=( It&& a_Iterator )
{
	m_VTable = Operator< It, false >;
	m_Iterator = std::forward< It >( a_Iterator );
	m_Enumerable = 0;
	return *this;
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_PreIncrement( It& a_Iterator )
{
	++a_Iterator;
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_PostIncrement( It& a_Iterator )
{
	return a_Iterator++;
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_PreDecrement( It& a_Iterator, const EnumerableType* a_Bounds )
{
	if constexpr ( IsBidirectionalIterator< It >() )
	{
		--a_Iterator;
	}
	else
	{
		_ASSERT_EXPR( a_Bounds, ENUMERATOR_ERROR_MESSAGE_BOUNDING_ENUMERABLE_BIDIRECTIONAL );

		It Sentinel = ( It& )a_Bounds->Begin().AsAny();

		_ASSERT_EXPR( Sentinel != a_Iterator, ENUMERATOR_ERROR_MESSAGE_OUT_OF_RANGE );

		It Result = Sentinel++;

		while ( Sentinel != a_Iterator )
		{
			++Sentinel;
			++Result;
		}

		a_Iterator = std::move( Result );
	}
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_PostDecrement( It& a_Iterator, const EnumerableType* a_Bounds )
{
	if constexpr ( IsBidirectionalIterator< It >() )
	{
		return a_Iterator--;
	}
	else
	{
		_ASSERT_EXPR( a_Bounds, ENUMERATOR_ERROR_MESSAGE_BOUNDING_ENUMERABLE_BIDIRECTIONAL );

		It Sentinel = ( It& )a_Bounds->Begin().AsAny();

		_ASSERT_EXPR( Sentinel != a_Iterator, ENUMERATOR_ERROR_MESSAGE_OUT_OF_RANGE );

		It Result = Sentinel++;

		while ( Sentinel != a_Iterator )
		{
			++Sentinel;
			++Result;
		}

		It Before = std::move( a_Iterator );
		a_Iterator = std::move( Result );
		return Before;
	}
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_Addition( It& a_Iterator, size_t a_Offset )
{
	It Result = a_Iterator;
	std::advance( Result, a_Offset );
	return Result;
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_AdditionEqual( It& a_Iterator, size_t a_Offset )
{
	std::advance( a_Iterator, a_Offset );
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_Subtraction( It& a_Iterator, size_t a_Offset, const EnumerableType* a_Bounds )
{
	if constexpr ( IsBidirectionalIterator< It >() )
	{
		It Result = a_Iterator;
		std::advance( Result, -( int64_t )a_Offset );
		return Result;
	}
	else
	{
		_ASSERT_EXPR( a_Bounds, ENUMERATOR_ERROR_MESSAGE_BOUNDING_ENUMERABLE_BIDIRECTIONAL );
		_ASSERT_EXPR( a_Bounds->Size() > a_Offset, ENUMERATOR_ERROR_MESSAGE_OUT_OF_RANGE );

		It Sentinel = ( const It& )a_Bounds->Begin().AsAny();

		for ( int i = 0; i < a_Offset; ++i )
		{
			_ASSERT_EXPR( Sentinel != a_Iterator, ENUMERATOR_ERROR_MESSAGE_OUT_OF_RANGE );

			++Sentinel;
		}

		It Result = ( const It& )a_Bounds->Begin().AsAny();

		while ( Sentinel != a_Iterator )
		{
			++Sentinel;
			++Result;
		}

		return Result;
	}
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_SubtractionEqual( It& a_Iterator, size_t a_Offset, const EnumerableType* a_Bounds )
{
	if constexpr ( IsBidirectionalIterator< It >() )
	{
		It Result = a_Iterator;
		std::advance( Result, -( int64_t )a_Offset );
		return Result;
	}
	else
	{
		_ASSERT_EXPR( a_Bounds, ENUMERATOR_ERROR_MESSAGE_BOUNDING_ENUMERABLE_BIDIRECTIONAL );
		_ASSERT_EXPR( a_Bounds->Size() > a_Offset, ENUMERATOR_ERROR_MESSAGE_OUT_OF_RANGE );

		It Sentinel = ( const It& )a_Bounds->Begin().AsAny();

		for ( int i = 0; i < a_Offset; ++i )
		{
			_ASSERT_EXPR( Sentinel != a_Iterator, ENUMERATOR_ERROR_MESSAGE_OUT_OF_RANGE );

			++Sentinel;
		}

		It Result = ( const It& )a_Bounds->Begin().AsAny();

		while ( Sentinel != a_Iterator )
		{
			++Sentinel;
			++Result;
		}

		a_Iterator = std::move( Result );
	}
}

template < typename T >
template < typename It > 
decltype( auto ) Enumerator< T >::Operator_Difference( It& a_Left, It& a_Right, const EnumerableType* a_Bounds )
{
	if constexpr ( IsRandomAccessIterator< It >() )
	{
		return a_Left - a_Right;
	}
	else
	{
		_ASSERT_EXPR( a_Bounds, ENUMERATOR_ERROR_MESSAGE_BOUNDING_ENUMERABLE_RANDOM_ACCESS );

		if ( a_Left == a_Right )
		{
			return ( int64_t )0;
		}

		It& End = ( It& )a_Bounds->End().AsAny();
		int64_t Count = 0;

		for ( It Right = a_Right; Right != End; ++Count )
		{
			if ( Right == a_Left )
			{
				return Count;
			}
		}

		Count = 0;

		for ( It Left = a_Left; Left != End; ++Count )
		{
			if ( Left == a_Right )
			{
				return Count;
			}
		}

		_ASSERT_EXPR( false, ENUMERATOR_ERROR_MESSAGE_OUT_OF_RANGE );
	}
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_Dereference( It& a_Iterator )
{
	return *a_Iterator;
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_Arrow( It& a_Iterator )
{
	if constexpr ( std::is_pointer_v< It > )
	{
		return It( a_Iterator );
	}
	else
	{
		return a_Iterator.operator->();
	}
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_Greater( It& a_Left, It& a_Right, const EnumerableType* a_Bounds )
{
	if constexpr ( IsRandomAccessIterator< It >() )
	{
		return a_Left > a_Right;
	}
	else
	{
		_ASSERT_EXPR( a_Bounds, ENUMERATOR_ERROR_MESSAGE_BOUNDING_ENUMERABLE_BIDIRECTIONAL );

		if ( a_Left == a_Right )
		{
			return false;
		}

		It& End = ( It& )a_Bounds->End().AsAny();

		if ( a_Right == End )
		{
			return false;
		}

		for ( It Right = a_Right; Right != End; ++Right )
		{
			if ( Right == a_Left )
			{
				return true;
			}
		}

		return false;
	}
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_GreaterEqual( It& a_Left, It& a_Right, const EnumerableType* a_Bounds )
{
	if constexpr ( IsRandomAccessIterator< It >() )
	{
		return a_Left >= a_Right;
	}
	else
	{
		_ASSERT_EXPR( a_Bounds, ENUMERATOR_ERROR_MESSAGE_BOUNDING_ENUMERABLE_RANDOM_ACCESS );

		if ( a_Left == a_Right )
		{
			return true;
		}

		It& End = ( It& )a_Bounds->End().AsAny();

		if ( a_Right == End )
		{
			return false;
		}

		for ( It Right = a_Right; Right != End; ++Right )
		{
			if ( Right == a_Left )
			{
				return true;
			}
		}

		return false;
	}
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_Lesser( It& a_Left, It& a_Right, const EnumerableType* a_Bounds )
{
	if constexpr ( IsRandomAccessIterator< It >() )
	{
		return a_Left < a_Right;
	}
	else
	{
		_ASSERT_EXPR( a_Bounds, ENUMERATOR_ERROR_MESSAGE_BOUNDING_ENUMERABLE_RANDOM_ACCESS );

		if ( a_Left == a_Right )
		{
			return false;
		}

		It& End = ( It& )a_Bounds->End().AsAny();

		if ( a_Right == End )
		{
			return true;
		}

		for ( It Right = a_Right; Right != End; ++Right )
		{
			if ( Right == a_Left )
			{
				return false;
			}
		}

		return true;
	}
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_LesserEqual( It& a_Left, It& a_Right, const EnumerableType* a_Bounds )
{
	if constexpr ( IsRandomAccessIterator< It >() )
	{
		return a_Left <= a_Right;
	}
	else
	{
		_ASSERT_EXPR( a_Bounds, ENUMERATOR_ERROR_MESSAGE_BOUNDING_ENUMERABLE_RANDOM_ACCESS );

		if ( a_Left == a_Right )
		{
			return true;
		}

		It& End = ( It& )a_Bounds->End().AsAny();

		if ( a_Right == End )
		{
			return true;
		}

		for ( It Right = a_Right; Right != End; ++Right )
		{
			if ( Right == a_Left )
			{
				return false;
			}
		}

		return true;
	}
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_Index( It& a_Iterator, int64_t a_Offset, const EnumerableType* a_Bounds )
{
	if constexpr ( IsRandomAccessIterator< It >() )
	{
		return a_Iterator[ a_Offset ];
	}
	else
	{
		if ( a_Offset < 0 )
		{
			return *Operator_Subtraction< It >( a_Iterator, ( size_t )-a_Offset, a_Bounds );
		}
		else
		{
			return *Operator_Addition< It >( a_Iterator, ( size_t )a_Offset );
		}
	}
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_Equal( It& a_Left, It& a_Right )
{
	return a_Left == a_Right;
}

template < typename T >
template < typename It >
decltype( auto ) Enumerator< T >::Operator_Inequal( It& a_Left, It& a_Right )
{
	return a_Left != a_Right;
}

namespace std
{
	template < typename T > auto begin( Enumerable< T >& a_Enumerable ) { return a_Enumerable.Begin(); }
	template < typename T > auto begin( const Enumerable< T >& a_Enumerable ) { return a_Enumerable.Begin(); }
	template < typename T > auto cbegin( const Enumerable< T >& a_Enumerable ) { return a_Enumerable.CBegin(); }
	template < typename T > auto rbegin( Enumerable< T >& a_Enumerable ) { return a_Enumerable.RBegin(); }
	template < typename T > auto rbegin( const Enumerable< T >& a_Enumerable ) { return a_Enumerable.RBegin(); }
	template < typename T > auto crbegin( const Enumerable< T >& a_Enumerable ) { return a_Enumerable.CRBegin(); }
	template < typename T > auto end( Enumerable< T >& a_Enumerable ) { return a_Enumerable.End(); }
	template < typename T > auto end( const Enumerable< T >& a_Enumerable ) { return a_Enumerable.End(); }
	template < typename T > auto cend( const Enumerable< T >& a_Enumerable ) { return a_Enumerable.CEnd(); }
	template < typename T > auto rend( Enumerable< T >& a_Enumerable ) { return a_Enumerable.REnd(); }
	template < typename T > auto rend( const Enumerable< T >& a_Enumerable ) { return a_Enumerable.REnd(); }
	template < typename T > auto crend( const Enumerable< T >& a_Enumerable ) { return a_Enumerable.CREnd(); }
}