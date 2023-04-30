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
class IIterator
{
public:

	using ValueType = std::decay_t< T >;
	using ReferenceType = T&;
	using PointerType = T*;
	using EnumeratorType = Enumerator< T >;

	virtual EnumeratorType ToEnumerator() const = 0;
	virtual ReferenceType operator*() const = 0;
	virtual PointerType operator->() const = 0;
};

template < typename T >
class IForwardIterator : public IIterator< T >
{
public:

	using BaseType = IIterator< T >;
	using ValueType = typename BaseType::ValueType;
	using ReferenceType = typename BaseType::ReferenceType;
	using PointerType = typename BaseType::PointerType;
	using EnumeratorType = typename BaseType::EnumeratorType;

	IForwardIterator& operator++() { this->IIncrement(); return *this; }

protected:

	virtual void IIncrement() = 0;
};

template < typename T >
class IBidirectionalIterator : public IForwardIterator< T >
{
public:

	using BaseType = IForwardIterator< T >;
	using ValueType = typename BaseType::ValueType;
	using ReferenceType = typename BaseType::ReferenceType;
	using PointerType = typename BaseType::PointerType;
	using EnumeratorType = typename BaseType::EnumeratorType;

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
	using ValueType = typename BaseType::ValueType;
	using ReferenceType = typename BaseType::ReferenceType;
	using PointerType = typename BaseType::PointerType;
	using EnumeratorType = typename BaseType::EnumeratorType;

	IRandomAccessIterator& operator++() { this->IIncrement(); return *this; }
	IRandomAccessIterator& operator+=( int64_t a_Offset ) { this->ISeek( a_Offset ); return *this; }
	IRandomAccessIterator& operator--() { this->IDecrement(); return *this; }
	IRandomAccessIterator& operator-=( int64_t a_Offset ) { this->ISeek( -a_Offset ); return *this; }
	virtual ReferenceType operator[]( int64_t a_Offset ) const = 0;

protected:

	virtual void ISeek( int64_t a_Offset ) = 0;
};

template < typename _Underlying >
class ForwardIterator : public IForwardIterator< std::remove_reference_t< typename std::iterator_traits< _Underlying >::value_type > >
{
public:

	using BaseType = IForwardIterator< std::remove_reference_t< typename std::iterator_traits< _Underlying >::value_type > >;
	using TraitsType = std::iterator_traits< ForwardIterator >;
	using ValueType = typename TraitsType::value_type;
	using ReferenceType = typename TraitsType::reference;
	using PointerType = typename TraitsType::pointer;
	using DifferenceType = typename TraitsType::difference_type;
	using CategoryType = typename TraitsType::iterator_category;
	using UnderlyingType = _Underlying;
	using EnumeratorType = typename BaseType::EnumeratorType;

	ForwardIterator() = default;
	ForwardIterator( const ForwardIterator& ) = default;
	ForwardIterator( ForwardIterator&& ) = default;
	ForwardIterator( const UnderlyingType& a_Underlying ) : m_Underlying( a_Underlying ) {}
	ForwardIterator& operator=( const ForwardIterator& ) = default;
	ForwardIterator& operator=( ForwardIterator&& ) = default;
	EnumeratorType ToEnumerator() const { return EnumeratorType::FromIterator( m_Underlying ); }
	ReferenceType operator*() const { return *m_Underlying; }
	PointerType operator->() const { if constexpr ( !std::is_pointer_v< UnderlyingType > ) { return m_Underlying.operator->(); } else { return m_Underlying; } }
	ForwardIterator& operator++() { ( void )++m_Underlying; return *this; }
	ForwardIterator operator++( int ) { return ForwardIterator( m_Underlying++ ); }
	bool operator==( const ForwardIterator& a_Iterator ) const { return m_Underlying == a_Iterator.m_Underlying; }
	bool operator!=( const ForwardIterator& a_Iterator ) const { return m_Underlying != a_Iterator.m_Underlying; }
	operator UnderlyingType& () { return m_Underlying; }
	operator const UnderlyingType& () const { return m_Underlying; }

protected:

	void IIncrement() { ( void )++m_Underlying; }

	UnderlyingType m_Underlying;
};

template < typename _Underlying >
class BidirectionalIterator : public IBidirectionalIterator< std::remove_reference_t< typename std::iterator_traits< _Underlying >::value_type > >
{
public:

	using BaseType = IBidirectionalIterator< std::remove_reference_t< typename std::iterator_traits< _Underlying >::value_type > >;
	using TraitsType = std::iterator_traits< BidirectionalIterator >;
	using ValueType = typename TraitsType::value_type;
	using ReferenceType = typename TraitsType::reference;
	using PointerType = typename TraitsType::pointer;
	using DifferenceType = typename TraitsType::difference_type;
	using CategoryType = typename TraitsType::iterator_category;
	using UnderlyingType = _Underlying;
	using EnumeratorType = Enumerator< std::remove_reference_t< ReferenceType > >;

	BidirectionalIterator() = default;
	BidirectionalIterator( const BidirectionalIterator& ) = default;
	BidirectionalIterator( BidirectionalIterator&& ) = default;
	BidirectionalIterator( const UnderlyingType& a_Underlying ) : m_Underlying( a_Underlying ) {}
	BidirectionalIterator& operator=( const BidirectionalIterator& ) = default;
	BidirectionalIterator& operator=( BidirectionalIterator&& ) = default;
	EnumeratorType ToEnumerator() const { return EnumeratorType( m_Underlying ); }
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

	void IIncrement() { ( void )++m_Underlying; }
	void IDecrement() { ( void )--m_Underlying; }

	UnderlyingType m_Underlying;
};

template < typename _Underlying >
class RandomAccessIterator : public IRandomAccessIterator< std::remove_reference_t< typename std::iterator_traits< _Underlying >::value_type > >
{
public:

	using BaseType = IRandomAccessIterator< std::remove_reference_t< typename std::iterator_traits< _Underlying >::value_type > >;
	using TraitsType = std::iterator_traits< RandomAccessIterator >;
	using ValueType = typename TraitsType::value_type;
	using ReferenceType = typename TraitsType::reference;
	using PointerType = typename TraitsType::pointer;
	using DifferenceType = typename TraitsType::difference_type;
	using CategoryType = typename TraitsType::iterator_category;
	using UnderlyingType = _Underlying;
	using EnumeratorType = Enumerator< std::remove_reference_t< ReferenceType > >;

	RandomAccessIterator() = default;
	RandomAccessIterator( const RandomAccessIterator& ) = default;
	RandomAccessIterator( RandomAccessIterator&& ) = default;
	RandomAccessIterator( const UnderlyingType& a_Underlying ) : m_Underlying( a_Underlying ) {}
	RandomAccessIterator& operator=( const RandomAccessIterator& ) = default;
	RandomAccessIterator& operator=( RandomAccessIterator&& ) = default;
	EnumeratorType ToEnumerator() const { return EnumeratorType( m_Underlying ); }
	ReferenceType operator*() const { return *m_Underlying; }
	PointerType operator->() const { if constexpr ( !std::is_pointer_v< UnderlyingType > ) { return m_Underlying.operator->(); } else { return m_Underlying; } }
	RandomAccessIterator& operator++() { ++this->m_Underlying; return *this; }
	RandomAccessIterator operator++( int ) { return RandomAccessIterator( this->m_Underlying++ ); }
	RandomAccessIterator& operator+=( DifferenceType a_Offset ) const { this->m_Underlying += a_Offset; return *this; }
	RandomAccessIterator operator+( DifferenceType a_Offset ) const { return RandomAccessIterator( this->m_Underlying + a_Offset ); }
	RandomAccessIterator& operator--() { --this->m_Underlying; return *this; }
	RandomAccessIterator operator--( int ) { return RandomAccessIterator( this->m_Underlying-- ); }
	RandomAccessIterator& operator-=( DifferenceType a_Offset ) const { this->m_Underlying -= a_Offset; return *this; }
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

	void IIncrement() { ( void )++m_Underlying; }
	void IDecrement() { ( void )--m_Underlying; }
	void ISeek( int64_t a_Offset ) { m_Underlying += a_Offset; }

	UnderlyingType m_Underlying;
};

template < typename T >
class Enumerable;

template < typename T >
using CEnumerator = Enumerator< const T >;

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
	Equal,
	Unequal,
	Index
};

using EnumeratorVTable = bool( * )( EnumeratorOperation, std::any&, std::any*, uintptr_t, uintptr_t, uintptr_t );

template < typename T >
class Enumerator : public IRandomAccessIterator< T >
{
public:

	using BaseType = IRandomAccessIterator< T >;
	using TraitsType = std::iterator_traits< Enumerator< T > >;
	using ValueType = typename TraitsType::value_type;
	using ReferenceType = typename TraitsType::reference;
	using PointerType = typename TraitsType::pointer;
	using DifferenceType = typename TraitsType::difference_type;
	using CategoryType = typename TraitsType::iterator_category;

	using VTableType = EnumeratorVTable;
	using EnumerableType = Enumerable< ValueType >;

	Enumerator() = default;
	Enumerator( const Enumerator& ) = default;
	Enumerator( Enumerator&& ) = default;

	template < typename = std::enable_if_t< std::is_const_v< T > > >
	Enumerator( const Enumerator< std::remove_const_t< T > >& a_Enumerator ) 
		: m_VTable( a_Enumerator.m_VTable )
		, m_Iterator( a_Enumerator.m_Iterator )
		, m_Enumerable( a_Enumerator.m_Enumerable )
	{}

	template < typename It >
	static Enumerator FromIterator( const It& a_Iterator )
	{
		return Enumerator( Operator< std::decay_t< It >, false >, std::any( a_Iterator ), nullptr );
	}

	//template < typename = std::enable_if_t< std::is_const_v< T > > >
	//Enumerator( Enumerator< std::remove_const_t< T > >&& a_Enumerator )
	//	: m_VTable( a_Enumerator.m_VTable )
	//	, m_Iterator( a_Enumerator.m_Iterator )
	//	, m_Enumerable( a_Enumerator.m_Enumerable )
	//{}

	//template < typename It, typename = std::enable_if_t< std::is_const_v< T > || !std::is_const_v< std::remove_reference_t< decltype( *std::declval< It >() ) > > > >
	//Enumerator( It& a_Iterator )
	//	: m_VTable( Operator< std::decay_t< It >, false > )
	//	, m_Iterator( a_Iterator )
	//	, m_Enumerable( nullptr )
	//{}

	//template < typename It >
	//Enumerator( It&& a_Iterator ) 
	//	: m_VTable( Operator< std::decay_t< It >, false > )
	//	, m_Iterator( a_Iterator )
	//	, m_Enumerable( nullptr )
	//{}

private:

	Enumerator( VTableType a_VTable, std::any&& a_Iterator, const EnumerableType* a_Enumerable )
		: m_VTable( a_VTable )
		, m_Iterator( std::forward< std::any >( a_Iterator ) )
		, m_Enumerable( a_Enumerable )
	{}

public:

	Enumerator ToEnumerator() const { return Enumerator( *this ); }

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

	Enumerator& operator=( const Enumerator& a_Enumerator )
	{
		m_VTable = a_Enumerator.m_VTable;
		m_Iterator = a_Enumerator.m_Iterator;
		m_Enumerable = a_Enumerator.m_Enumerable;
		return *this;
	}

	Enumerator& operator=( Enumerator&& a_Enumerator ) noexcept
	{
		m_VTable = a_Enumerator.m_VTable;
		m_Iterator = std::move( a_Enumerator.m_Iterator );
		m_Enumerable = a_Enumerator.m_Enumerable;
		return *this;
	}

	template < typename It >
	Enumerator& operator=( It& a_Iterator );

	template < typename It >
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
				const_cast< std::any& >( m_Iterator ),
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
				const_cast< std::any& >( m_Iterator ),
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
				const_cast< std::any& >( m_Iterator ),
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
				const_cast< std::any& >( m_Iterator ),
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
				const_cast< std::any& >( m_Iterator ),
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
				const_cast< std::any& >( m_Iterator ),
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
			const_cast< std::any& >( m_Iterator ),
			const_cast< std::any* >( &a_Enumerator.m_Iterator ),
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
			const_cast< std::any& >( m_Iterator ),
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
			const_cast< std::any& >( m_Iterator ),
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
			const_cast< std::any& >( m_Iterator ),
			const_cast< std::any* >( &a_Enumerator.m_Iterator ),
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
			const_cast< std::any& >( m_Iterator ),
			const_cast< std::any* >( &a_Enumerator.m_Iterator ),
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
			const_cast< std::any& >( m_Iterator ),
			const_cast< std::any* >( &a_Enumerator.m_Iterator ),
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
			const_cast< std::any& >( m_Iterator ),
			const_cast< std::any* >( &a_Enumerator.m_Iterator ),
			0,
			0,
			0
		);
	}

	bool operator==( const Enumerator< T >& a_Enumerator ) const
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		return m_VTable(
			EnumeratorOperation::Equal,
			const_cast< std::any& >( m_Iterator ),
			const_cast< std::any* >( &a_Enumerator.m_Iterator ),
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
			const_cast< std::any& >( m_Iterator ),
			const_cast< std::any* >( &a_Enumerator.m_Iterator ),
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
			const_cast< std::any& >( m_Iterator ),
			0,
			( uintptr_t )( uint64_t )a_Offset,
			( uintptr_t )m_Enumerable,
			( uintptr_t )&Result
		);

		return *Result;
	}

	const std::any& AsAny() const { return m_Iterator; }
	const std::type_info& GetType() const { return m_Iterator.type(); }

protected:

	void IIncrement() { ++*this; }
	void IDecrement() { --*this; }
	void ISeek( int64_t a_Offset ) { *this += a_Offset; }

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
				( It& )*a_Iterator1 = std::move( Result );
				*( VTableType* )a_Arg1 = Operator< It, _Reversed >;
			}
			else
			{
				It Result = Operator_Subtraction< It >( ( It& )a_Iterator0, ( size_t )a_Arg0, ( const EnumerableType* )a_Arg2 );
				( It& )*a_Iterator1 = std::move( Result );
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
				( It& )*a_Iterator1 = std::move( Result );
				*( VTableType* )a_Arg1 = Operator< It, _Reversed >;
			}
			else
			{
				It Result = Operator_Addition< It >( ( It& )a_Iterator0, ( size_t )a_Arg0 );
				( It& )*a_Iterator1 = std::move( Result );
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
	std::any              m_Iterator;
	const EnumerableType* m_Enumerable;
};

template < typename T >
class Enumerable;

template < typename T >
class ICollection
{
public:

	using ValueType = T;
	using EnumerableType = Enumerable< T >;
	using IteratorType = Enumerator< T >;
	using CIteratorType = CEnumerator< T >;

	IteratorType Begin() { return IBegin(); }
	CIteratorType Begin() const { return const_cast< ICollection* >( this )->IBegin(); }
	IteratorType End() { return IEnd(); }
	CIteratorType End() const { return const_cast< ICollection* >( this )->IEnd(); }

	EnumerableType ToEnumerable() = 0;
	virtual size_t Size() const = 0;
	virtual size_t MaxSize() const = 0;

protected:

	virtual IteratorType IBegin() = 0;
	virtual IteratorType IEnd() = 0;
};

template < typename T >
class IContiguousCollection : public ICollection< T >
{
public:

	using BaseType = ICollection< T >;

	T* Data() { return IData(); }
	const T* Data() const { return const_cast< IContainerIterator* >( this )->IData(); }
	T& At( size_t a_Index ) { return IAt( a_Index ); }
	const T& At( size_t a_Index ) const { return const_cast< IContiguousCollection* >( this )->IAt( a_Index ); }

protected:

	virtual T* IData() = 0;
	virtual T& IAt( size_t a_Index ) = 0;
};

template < typename T >
class Enumerable : public ICollection< T >
{
	static_assert( std::is_same_v< std::decay_t< T >, T >, "Use of non-decayed type not allowed." );

public:

	using IteratorType = Enumerator< T >;
	using CIteratorType = CEnumerator< T >;
	using RIteratorType = Enumerator< T >;
	using CRIteratorType = CEnumerator< T >;

	Enumerable() = default;
	Enumerable( const Enumerable& ) = default;
	Enumerable( Enumerable&& ) = default;

	Enumerable( size_t a_Size, const IteratorType& a_Begin, const IteratorType& a_End )
		: m_Size( a_Size )
		, m_Begin( a_Begin )
		, m_End( a_End )
	{}

	const std::type_info& GetType() const { return m_Begin.GetType(); }
	size_t Size() const { return m_Size; }
	size_t MaxSize() const { return static_cast< size_t >( -1 ); }

	IteratorType Begin() { return IteratorType( m_Begin.m_VTable, std::any( m_Begin.m_Iterator ), this ); }
	CIteratorType Begin() const { return m_Begin; }
	CIteratorType CBegin() const { return m_Begin; }
	IteratorType End() { return m_End; }
	CIteratorType End() const { return m_End; }
	CIteratorType CEnd() const { return m_End; }

protected:

	IteratorType IBegin() { return m_Begin; }
	IteratorType IEnd() { return m_End; }

private:
	
	size_t m_Size;
	IteratorType m_Begin;
	IteratorType m_End;
};

namespace std
{
	template < typename T >
	struct iterator_traits< Enumerator< T > >
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
}

template < typename T >
template < typename It >
Enumerator< T >& Enumerator< T >::operator=( It& a_Iterator )
{
	m_VTable = Operator< It, false >;
	m_Iterator = a_Iterator;
	m_Enumerable = 0;
}

template < typename T >
template < typename It >
Enumerator< T >& Enumerator< T >::operator=( It&& a_Iterator )
{
	m_VTable = Operator< It, false >;
	m_Iterator = std::forward< It >( a_Iterator );
	m_Enumerable = 0;
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
