#pragma once
#include <any>

#include "Invoker.hpp"

#define ENUMERATOR_ERROR_MESSAGE_FORWARD_ITERATOR "Iterator must be at least a forward iterator."
#define ENUMERATOR_ERROR_MESSAGE_BIDIRECTIONAL_ITERATOR "Iterator must be at least a bidirectional iterator."
#define ENUMERATOR_ERROR_MESSAGE_BOUNDING_ENUMERABLE_BIDIRECTIONAL "Non-bidirectional iterators require a bounding Enumerable."
#define ENUMERATOR_ERROR_MESSAGE_BOUNDING_ENUMERABLE_RANDOM_ACCESS "Non-random access iterators require a bounding Enumerable."
#define ENUMERATOR_ERROR_MESSAGE_OUT_OF_RANGE "Iterator outside of bounding range."
#define ENUMERATOR_ERROR_MESSAGE_NO_VTABLE "No virtual table set."
#define ENUMERATOR_ERROR_INCOMPATIBLE_UNDERLYING_ITERATORS "Underlying iterators are incompatible and can not be compared."

template < typename T >
class Enumerator;

template < typename T >
using CEnumerator = Enumerator< const T >;

template < typename T >
class Enumerable;

template < typename T >
using CEnumerable = Enumerable< const T >;

template < typename T >
using InitializerList = std::initializer_list< T >;

enum class EIteratorType
{
	Forward,
	Bidirectional,
	RandomAccess
};

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
	EIteratorType Type() const { return IType(); }

protected:

	virtual EIteratorType IType() const = 0;
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

	EIteratorType IType() const { return EIteratorType::Forward; }
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

	EIteratorType IType() const { return EIteratorType::Bidirectional; }
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

	EIteratorType IType() const { return EIteratorType::RandomAccess; }
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

	constexpr ForwardIterator() = default;
	constexpr ForwardIterator( const ForwardIterator& ) = default;
	constexpr ForwardIterator( ForwardIterator&& ) = default;
	constexpr ForwardIterator( const UnderlyingType& a_Underlying ) : m_Underlying( a_Underlying ) {}

	template < typename It, typename = std::enable_if_t< std::is_constructible_v< _Underlying, It > > >
	constexpr ForwardIterator( const ForwardIterator< It >& a_Iterator )
		: m_Underlying( a_Iterator.m_Underlying )
	{}

	template < typename It, typename = std::enable_if_t< std::is_constructible_v< _Underlying, It&& > > >
	constexpr ForwardIterator( ForwardIterator< It >&& a_Iterator )
		: m_Underlying( std::move( a_Iterator.m_Underlying ) )
	{}

	constexpr ForwardIterator& operator=( const ForwardIterator& ) = default;
	constexpr ForwardIterator& operator=( ForwardIterator&& ) = default;
	constexpr ReferenceType operator*() const { return *m_Underlying; }
	constexpr PointerType operator->() const { if constexpr ( !std::is_pointer_v< UnderlyingType > ) { return m_Underlying.operator->(); } else { return m_Underlying; } }
	constexpr ForwardIterator& operator++() { ( void )++m_Underlying; return *this; }
	constexpr ForwardIterator operator++( int ) { return ForwardIterator( m_Underlying++ ); }
	constexpr bool operator==( const ForwardIterator& a_Iterator ) const { return m_Underlying == a_Iterator.m_Underlying; }
	constexpr bool operator!=( const ForwardIterator& a_Iterator ) const { return m_Underlying != a_Iterator.m_Underlying; }
	constexpr operator UnderlyingType& () { return m_Underlying; }
	constexpr operator const UnderlyingType& () const { return m_Underlying; }

protected:

	EnumeratorType IToEnumerator() { return *this; }
	ReferenceType IDereference() const { return this->operator*(); }
	PointerType IArrow() const { return this->operator->(); }
	void IIncrement() { ( void )++m_Underlying; }

private:

	template < class > friend class ForwardIterator;

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

	constexpr BidirectionalIterator() = default;
	constexpr BidirectionalIterator( const BidirectionalIterator& ) = default;
	constexpr BidirectionalIterator( BidirectionalIterator&& ) = default;
	constexpr BidirectionalIterator( const UnderlyingType& a_Underlying ) : m_Underlying( a_Underlying ) {}

	template < typename It, typename = std::enable_if_t< std::is_constructible_v< _Underlying, It > > >
	constexpr BidirectionalIterator( const BidirectionalIterator< It >& a_Iterator )
		: m_Underlying( a_Iterator.m_Underlying )
	{}

	template < typename It, typename = std::enable_if_t< std::is_constructible_v< _Underlying, It&& > > >
	constexpr BidirectionalIterator( BidirectionalIterator< It >&& a_Iterator )
		: m_Underlying( std::move( a_Iterator.m_Underlying ) )
	{}

	constexpr BidirectionalIterator& operator=( const BidirectionalIterator& ) = default;
	constexpr BidirectionalIterator& operator=( BidirectionalIterator&& ) = default;
	constexpr ReferenceType operator*() const { return *m_Underlying; }
	constexpr PointerType operator->() const { if constexpr ( !std::is_pointer_v< UnderlyingType > ) { return m_Underlying.operator->(); } else { return m_Underlying; } }
	constexpr BidirectionalIterator& operator++() { ( void )++this->m_Underlying; return *this; }
	constexpr BidirectionalIterator operator++( int ) { return BidirectionalIterator( this->m_Underlying++ ); }
	constexpr BidirectionalIterator& operator--() { ( void )--this->m_Underlying; return *this; }
	constexpr BidirectionalIterator operator--( int ) { return BidirectionalIterator( this->m_Underlying-- ); }
	constexpr bool operator==( const BidirectionalIterator& a_Iterator ) const { return this->m_Underlying == a_Iterator.m_Underlying; }
	constexpr bool operator!=( const BidirectionalIterator& a_Iterator ) const { return this->m_Underlying != a_Iterator.m_Underlying; }
	constexpr operator UnderlyingType& () { return this->m_Underlying; }
	constexpr operator const UnderlyingType& () const { return this->m_Underlying; }

protected:

	EnumeratorType IToEnumerator() { return *this; }
	ReferenceType IDereference() const { return this->operator*(); }
	PointerType IArrow() const { return this->operator->(); }
	void IIncrement() { ( void )++m_Underlying; }
	void IDecrement() { ( void )--m_Underlying; }

private:

	template < class > friend class BidirectionalIterator;

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

	constexpr RandomAccessIterator() = default;
	constexpr RandomAccessIterator( const RandomAccessIterator& ) = default;
	constexpr RandomAccessIterator( RandomAccessIterator&& ) = default;
	constexpr RandomAccessIterator( const UnderlyingType& a_Underlying ) : m_Underlying( a_Underlying ) {}

	template < typename It, typename = std::enable_if_t< std::is_constructible_v< _Underlying, It > > >
	constexpr RandomAccessIterator( const RandomAccessIterator< It >& a_Iterator )
		: m_Underlying( a_Iterator.m_Underlying )
	{}

	template < typename It, typename = std::enable_if_t< std::is_constructible_v< _Underlying, It&& > > >
	constexpr RandomAccessIterator( RandomAccessIterator< It >&& a_Iterator )
		: m_Underlying( std::move( a_Iterator.m_Underlying ) )
	{}

	constexpr RandomAccessIterator& operator=( const RandomAccessIterator& ) = default;
	constexpr RandomAccessIterator& operator=( RandomAccessIterator&& ) = default;
	constexpr ReferenceType operator*() const { return *m_Underlying; }
	constexpr PointerType operator->() const { if constexpr ( !std::is_pointer_v< UnderlyingType > ) { return m_Underlying.operator->(); } else { return m_Underlying; } }
	constexpr RandomAccessIterator& operator++() { ++this->m_Underlying; return *this; }
	constexpr RandomAccessIterator operator++( int ) { return RandomAccessIterator( this->m_Underlying++ ); }
	constexpr RandomAccessIterator& operator+=( DifferenceType a_Offset ) { this->m_Underlying += a_Offset; return *this; }
	constexpr RandomAccessIterator operator+( DifferenceType a_Offset ) const { return RandomAccessIterator( this->m_Underlying + a_Offset ); }
	constexpr RandomAccessIterator& operator--() { --this->m_Underlying; return *this; }
	constexpr RandomAccessIterator operator--( int ) { return RandomAccessIterator( this->m_Underlying-- ); }
	constexpr RandomAccessIterator& operator-=( DifferenceType a_Offset ) { this->m_Underlying -= a_Offset; return *this; }
	constexpr RandomAccessIterator operator-( DifferenceType a_Offset ) const { return RandomAccessIterator( this->m_Underlying - a_Offset ); }
	constexpr DifferenceType operator-( const RandomAccessIterator& a_Iterator ) const { return this->m_Underlying - a_Iterator.m_Underlying; }
	constexpr bool operator>( const RandomAccessIterator& a_Iterator ) const { return this->m_Underlying > a_Iterator.m_Underlying; }
	constexpr bool operator>=( const RandomAccessIterator& a_Iterator ) const { return this->m_Underlying >= a_Iterator.m_Underlying; }
	constexpr bool operator<( const RandomAccessIterator& a_Iterator ) const { return this->m_Underlying < a_Iterator.m_Underlying; }
	constexpr bool operator<=( const RandomAccessIterator& a_Iterator ) const { return this->m_Underlying <= a_Iterator.m_Underlying; }
	constexpr bool operator==( const RandomAccessIterator& a_Iterator ) const { return this->m_Underlying == a_Iterator.m_Underlying; }
	constexpr bool operator!=( const RandomAccessIterator& a_Iterator ) const { return this->m_Underlying != a_Iterator.m_Underlying; }
	constexpr ReferenceType operator[]( DifferenceType a_Offset ) const { return this->m_Underlying[ a_Offset ]; }
	constexpr operator UnderlyingType& () { return this->m_Underlying; }
	constexpr operator const UnderlyingType& () const { return this->m_Underlying; }

protected:

	EnumeratorType IToEnumerator() { return *this; }
	ReferenceType IDereference() const { return this->operator*(); }
	PointerType IArrow() const { return this->operator->(); }
	void IIncrement() { ( void )++m_Underlying; }
	void IDecrement() { ( void )--m_Underlying; }
	void ISeek( int64_t a_Offset ) { ( void )( m_Underlying += a_Offset ); }
	ReferenceType IAt( DifferenceType a_Offset ) const { return m_Underlying[ a_Offset ]; }

private:

	template < class > friend class RandomAccessIterator;

	UnderlyingType m_Underlying;
};

namespace std
{
	template < typename It >
	struct is_iterator
	{
		static constexpr bool value = _Is_iterator_v< It >;
	};

	template < typename It >
	struct is_const_iterator
	{
	private:

		template < typename U > 
		static constexpr bool test()
		{
			if constexpr ( is_iterator< U >::value )
			{
				return is_const_v< remove_reference_t< decltype( *declval< U >() ) > >;
			}

			return false;
		}

	public:

		static constexpr bool value = test< It >();
	};

	template < typename It >
	struct is_reverse_iterator
	{
		static constexpr bool value = false;
	};

	template < typename It >
	struct is_reverse_iterator< std::reverse_iterator< It > >
	{
		static constexpr bool value = true;
	};

	template < typename It >
	struct is_const_reverse_iterator
	{
	private:

		template < typename U >
		static constexpr bool test()
		{
			if constexpr ( is_reverse_iterator< U >::value )
			{
				return is_const_v< remove_reference_t< decltype( *declval< U >() ) > >;
			}

			return false;
		}

	public:

		static constexpr bool value = test< It >();
	};

	template < typename It >
	struct is_forward_iterator
	{
	private:

		template < typename U >
		static constexpr bool test()
		{
			if constexpr ( is_iterator< It >::value ) 
			{ 
				return is_base_of_v< forward_iterator_tag, typename iterator_traits< It >::iterator_category >; 
			}

			return false;
		}

	public:

		static constexpr bool value = test< It >();
	};

	template < typename It >
	struct is_bidirectional_iterator
	{
	private:

		template < typename U >
		static constexpr bool test()
		{
			if constexpr ( is_iterator< It >::value )
			{
				return is_base_of_v< bidirectional_iterator_tag, typename iterator_traits< It >::iterator_category >;
			}

			return false;
		}

	public:

		static constexpr bool value = test< It >();
	};

	template < typename It >
	struct is_random_access_iterator
	{
	private:

		template < typename U >
		static constexpr bool test()
		{
			if constexpr ( is_iterator< It >::value )
			{
				return is_base_of_v< random_access_iterator_tag, typename iterator_traits< It >::iterator_category >;
			}

			return false;
		}

	public:

		static constexpr bool value = test< It >();
	};
	
	template < typename It >
	static constexpr bool is_iterator_v = is_iterator< It >::value;

	template < typename It >
	static constexpr bool is_const_iterator_v = is_const_iterator< It >::value;

	template < typename It >
	static constexpr bool is_reverse_iterator_v = is_reverse_iterator< It >::value;
	
	template < typename It >
	static constexpr bool is_const_reverse_iterator_v = is_const_reverse_iterator< It >::value;

	template < typename It >
	static constexpr bool is_forward_iterator_v = is_forward_iterator< It >::value;
	
	template < typename It >
	static constexpr bool is_bidirectional_iterator_v = is_bidirectional_iterator< It >::value;

	template < typename It >
	static constexpr bool is_random_access_iterator_v = is_random_access_iterator< It >::value;

	template < typename It >
	struct is_enumerator { static constexpr bool value = false; };

	template < typename It >
	struct is_enumerator< Enumerator< It > > { static constexpr bool value = true; };

	template < typename It >
	struct is_const_enumerator { static constexpr bool value = false; };

	template < typename It >
	struct is_const_enumerator< CEnumerator< It > > { static constexpr bool value = true; };

	template < typename It >
	static constexpr bool is_enumerator_v = is_enumerator< It >::value;

	template < typename It >
	static constexpr bool is_const_enumerator_v = is_const_enumerator< It >::value;
}

enum class EEnumeratorOperation
{
	Reverse,
	IsReversed,
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

using EnumeratorVTable = bool( * )( EEnumeratorOperation, std::any&, std::any*, uintptr_t, uintptr_t, uintptr_t );

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
	using EnumerableType = Enumerable< ValueType >;
	using CEnumerableType = Enumerable< const ValueType >;

	using VTableType = EnumeratorVTable;

	Enumerator() = default;
	Enumerator( const Enumerator& a_Enumerator ) = default;

	Enumerator( Enumerator&& a_Enumerator ) noexcept
		: m_VTable( a_Enumerator.m_VTable )
		, m_Iterator( std::move( a_Enumerator.m_Iterator ) )
		, m_Enumerable( a_Enumerator.m_Enumerable )
	{
		a_Enumerator.m_VTable = nullptr;
		a_Enumerator.m_Iterator.reset();
		a_Enumerator.m_Enumerable = nullptr;
	}

	template < typename = std::enable_if_t< std::is_const_v< T > > >
	Enumerator( const Enumerator< std::decay_t< T > >& a_Enumerator )
		: m_VTable( a_Enumerator.m_VTable )
		, m_Iterator( a_Enumerator.m_Iterator )
		, m_Enumerable( ( const CEnumerableType* )a_Enumerator.m_Enumerable )
	{}

	template < typename = std::enable_if_t< std::is_const_v< T > > >
	Enumerator( Enumerator< std::decay_t< T > >&& a_Enumerator )
		: m_VTable( a_Enumerator.m_VTable )
		, m_Iterator( std::move( a_Enumerator.m_Iterator ) )
		, m_Enumerable( ( CEnumerableType* )a_Enumerator.m_Enumerable )
	{}

	Enumerator& operator=( const Enumerator& ) = default;
	Enumerator& operator=( Enumerator&& ) = default;

	// Need to add proper conversions in the assignment operator for non const enumerator

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
		, m_Iterator( std::move( a_Iterator ) )
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
			EEnumeratorOperation::Reverse,
			m_Iterator,
			0,
			( uintptr_t )&Table,
			0,
			0
		);

		std::any Copy = m_Iterator;
		return Enumerator( Table, std::move( Copy ), m_Enumerable );
	}

	bool IsReversed() const
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		return m_VTable(
			EEnumeratorOperation::IsReversed,
			m_Iterator,
			0,
			0,
			0,
			0
		);
	}

	template < typename It, typename = std::enable_if_t< is_compatible_iterator_v< It > > >
	Enumerator& operator=( It& a_Iterator );

	template < typename It, typename = std::enable_if_t< is_compatible_iterator_v< It > > >
	Enumerator& operator=( It&& a_Iterator );

	Enumerator& operator++()
	{
		_ASSERT_EXPR( m_VTable, ENUMERATOR_ERROR_MESSAGE_NO_VTABLE );

		m_VTable( 
			EEnumeratorOperation::PreIncrement,
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
			EEnumeratorOperation::PostIncrement,
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
			EEnumeratorOperation::PreDecrement,
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
			EEnumeratorOperation::PostDecrement,
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
				EEnumeratorOperation::Subtraction,
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
				EEnumeratorOperation::Addition,
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
				EEnumeratorOperation::SubtractionEqual,
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
				EEnumeratorOperation::AdditionEqual,
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
				EEnumeratorOperation::Addition,
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
				EEnumeratorOperation::Subtraction,
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
				EEnumeratorOperation::AdditionEqual,
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
				EEnumeratorOperation::SubtractionEqual,
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
		_ASSERT_EXPR( m_Iterator.type() == a_Enumerator.m_Iterator.type(), ENUMERATOR_ERROR_INCOMPATIBLE_UNDERLYING_ITERATORS );

		DifferenceType Difference;

		m_VTable(
			EEnumeratorOperation::Difference,
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
			EEnumeratorOperation::Dereference,
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
			EEnumeratorOperation::Arrow,
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
		_ASSERT_EXPR( m_Iterator.type() == a_Enumerator.m_Iterator.type(), ENUMERATOR_ERROR_INCOMPATIBLE_UNDERLYING_ITERATORS );

		return m_VTable( 
			EEnumeratorOperation::Greater, 
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
		_ASSERT_EXPR( m_Iterator.type() == a_Enumerator.m_Iterator.type(), ENUMERATOR_ERROR_INCOMPATIBLE_UNDERLYING_ITERATORS );

		return m_VTable(
			EEnumeratorOperation::GreaterEqual,
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
		_ASSERT_EXPR( m_Iterator.type() == a_Enumerator.m_Iterator.type(), ENUMERATOR_ERROR_INCOMPATIBLE_UNDERLYING_ITERATORS );

		return m_VTable(
			EEnumeratorOperation::Lesser,
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
		_ASSERT_EXPR( m_Iterator.type() == a_Enumerator.m_Iterator.type(), ENUMERATOR_ERROR_INCOMPATIBLE_UNDERLYING_ITERATORS );

		return m_VTable(
			EEnumeratorOperation::LesserEqual,
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
			EEnumeratorOperation::Index,
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
		_ASSERT_EXPR( m_Iterator.type() == a_Enumerator.m_Iterator.type(), ENUMERATOR_ERROR_INCOMPATIBLE_UNDERLYING_ITERATORS );

		return m_VTable(
			EEnumeratorOperation::Equal,
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
		_ASSERT_EXPR( m_Iterator.type() == a_Enumerator.m_Iterator.type(), ENUMERATOR_ERROR_INCOMPATIBLE_UNDERLYING_ITERATORS );

		return m_VTable(
			EEnumeratorOperation::Unequal,
			m_Iterator,
			&a_Enumerator.m_Iterator,
			0,
			0,
			0
		);
	}

	const std::any& AsAny() const { return m_Iterator; }
	IForwardIterator< T >& AsIterator() { return ( IForwardIterator< T >& )m_Iterator; }
	const IForwardIterator< const T >& AsIterator() const { return const_cast< Enumerator< T >* >( this )->AsIterator(); }
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
	static bool Operator( EEnumeratorOperation a_Operation, std::any& a_Iterator0, std::any* a_Iterator1, uintptr_t a_Arg0, uintptr_t a_Arg1, uintptr_t a_Arg2 )
	{
		_ASSERT_EXPR( a_Iterator0.has_value(), "Cannot perform an operation on an empty enumerator." );
		_ASSERT_EXPR( std::is_forward_iterator_v< It >, "Iterator type must be at least forward iterator." );

		switch ( a_Operation )
		{
		case EEnumeratorOperation::Reverse:
		{
			*( VTableType* )a_Arg0 = Operator< It, !_Reversed >;
			break;
		}
		case EEnumeratorOperation::IsReversed:
		{
			return _Reversed;
			break;
		}
		case EEnumeratorOperation::PreIncrement:
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
		case EEnumeratorOperation::PostIncrement:
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
		case EEnumeratorOperation::PreDecrement:
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
		case EEnumeratorOperation::PostDecrement:
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
		case EEnumeratorOperation::Addition:
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
		case EEnumeratorOperation::AdditionEqual:
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
		case EEnumeratorOperation::Subtraction:
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
		case EEnumeratorOperation::SubtractionEqual:
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
		case EEnumeratorOperation::Difference:
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
		case EEnumeratorOperation::Dereference:
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
		case EEnumeratorOperation::Arrow:
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
		case EEnumeratorOperation::Greater:
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
		case EEnumeratorOperation::GreaterEqual:
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
		case EEnumeratorOperation::Lesser:
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
		case EEnumeratorOperation::LesserEqual:
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
		case EEnumeratorOperation::Equal:
		{
			return Operator_Equal< It >( ( It& )a_Iterator0, ( It& )*a_Iterator1 );
		}
		case EEnumeratorOperation::Unequal:
		{
			return Operator_Inequal< It >( ( It& )a_Iterator0, ( It& )*a_Iterator1 );
		}
		case EEnumeratorOperation::Index:
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
class Deferred;

template < typename T >
class ICollection
{
public:

	using ValueType = std::decay_t< T >;
	using SizeType = size_t;
	using DifferenceType = std::ptrdiff_t;

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

	bool Contains( const T& a_Value ) const;
	bool Contains( const Predicate< const T& >& a_Predicate ) const;
	const T* Find( const T& a_Value ) const { return const_cast< ICollection* >( this )->Find( a_Value ); }
	const T* Find( const Predicate< const T& >& a_Predicate ) const { return const_cast< ICollection* >( this )->Find( a_Predicate ); }
	T* Find( const T& a_Value );
	T* Find( const Predicate< const T& >& a_Predicate );
	const T* FindLast( const T& a_Value ) const { return const_cast< ICollection* >( this )->FindLast( a_Value ); }
	const T* FindLast( const Predicate< const T& >& a_Predicate ) const { return const_cast< ICollection* >( this )->FindLast( a_Predicate ); }
	T* FindLast( const T& a_Value );
	T* FindLast( const Predicate< const T& >& a_Predicate );
	
	//Collection< const T* > FindAll( const T& a_Value ) const;
	//Collection< const T* > FindAll( const Predicate< const T& >& a_Predicate ) const;
	Deferred< T > FindAll( const T& a_Value );

	template < typename _Predicate
	Deferred< T > FindAll( _Predicate&& a_Predicate );

	template < typename U = T >
	U Aggregate( const Invoker< U, const T&, const U& >& a_Aggregator ) const;

	template < typename U = T >
	U Aggregate( const U& a_Seed, const Invoker< U, const T&, const U& >& a_Aggregator ) const;

	bool TrueForAll( const Predicate< const T& >& a_Predicate ) const;
	bool TrueForAny( const Predicate< const T& >& a_Predicate ) const;

	template < typename U >
	std::enable_if_t< std::is_arithmetic_v< U >, U > Average() const;

	template < typename U >
	std::enable_if_t< std::is_arithmetic_v< U >, U > Average( const Invoker< U, const T& >& a_Converter ) const;

	size_t Count( const T& a_Value ) const;
	size_t Count( const Predicate< const T& >& a_Predicate ) const;

	//Collection< const T* > Distinct() const;
	//Collection< const T* > Distinct( const EqualityComparer< const T& >& a_EqualityComparer ) const;

	//Collection< T* > Distinct();
	//Collection< T* > Distinct( const EqualityComparer< const T& >& a_EqualityComparer );

protected:

	virtual EnumeratorType IBegin() = 0;
	virtual EnumeratorType IEnd() = 0;
	virtual EnumerableType IToEnumerable() = 0;
	virtual size_t ISize() const = 0;
};

template < typename T >
class IContiguousCollection : public ICollection< T >
{
public:

	using ValueType = T;
	using SizeType = size_t;
	using DifferenceType = std::ptrdiff_t;
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
	CRIteratorType RBegin() const { return const_cast< IContiguousCollection* >( this )->IRBegin(); }
	CRIteratorType CRBegin() const { return const_cast< IContiguousCollection* >( this )->IRBegin(); }
	RIteratorType REnd() { return IREnd(); }
	CRIteratorType REnd() const { return const_cast< IContiguousCollection* >( this )->IREnd(); }
	CRIteratorType CREnd() const { return const_cast< IContiguousCollection* >( this )->IREnd(); }

	T* Data() { return IData(); }
	const T* Data() const { return const_cast< IContiguousCollection* >( this )->IData(); }
	T& At( size_t a_Index ) { return IAt( a_Index ); }
	const T& At( size_t a_Index ) const { return const_cast< IContiguousCollection* >( this )->IAt( a_Index ); }

	//bool IsValidIndex(size_t);
	//size_t IndexOf(const T&) const;
	//size_t IndexOf(Func&&) const;
	//const T& At(size_t) const;
	//T& At(size_t);
	//const T& operator[] const;
	//T& operator[];


protected:

	virtual EnumeratorType IRBegin() = 0;
	virtual EnumeratorType IREnd() = 0;
	virtual T* IData() = 0;
	virtual T& IAt( size_t a_Index ) = 0;
};

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
	EnumeratorType IBegin() { return Begin(); }
	EnumeratorType IEnd() { return End(); }
	EnumeratorType IRBegin() { return RBegin(); }
	EnumeratorType IREnd() { return REnd(); }
	T* IData() { return m_Data; }
	T& IAt( size_t a_Index ) { return m_Data[ a_Index ]; }

	ValueType* m_Data;
	SizeType   m_Size;
};

#pragma region SpanDefinitions
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
#pragma endregion

template < typename T >
class Collection : public Span< T >
{
public:

	using ValueType = T;
	using SizeType = size_t;
	using DifferenceType = std::ptrdiff_t;
	using BaseType = Span< T >;

	Collection() = default;
	Collection( const Collection& );
	Collection( Collection&& );
	Collection( SizeType a_Capacity );
	Collection( const ICollection< T >& a_Collection );
	Collection( const ICollection< T >& a_Collection, SizeType a_Capacity );

	constexpr SizeType MaxSize() const { return -1; }
	SizeType Capacity() const { return m_Capacity; }
	bool Empty() const { return !this->m_Size; }
	bool Full() const { return this->m_Size == this->m_Capacity; }
	void Add( const T& a_Value );

	const T& operator[]( SizeType a_Index ) const { return this->m_Data[ a_Index ]; }
	T& operator[]( SizeType a_Index ) { return this->m_Data[ a_Index ]; }

	// Need more functions.

protected:

	SizeType m_Capacity;
};

template < typename T >
Collection< T >::Collection( const Collection& a_Collection )
	: BaseType( ( T* )std::malloc( a_Collection.m_Capacity * sizeof( T ) ), a_Collection.m_Size )
	, m_Capacity( a_Collection.m_Capacity )
{
	if constexpr ( std::is_pod_v< T > )
	{
		std::memcpy( this->m_Data, a_Collection.m_Data, a_Collection.m_Size * sizeof( T ) );
	}
	else
	{
		std::copy( a_Collection.Begin(), a_Collection.End(), this->Begin() );
	}
}

template < typename T >
Collection< T >::Collection( Collection&& a_Collection )
	: BaseType( a_Collection.m_Data, a_Collection.m_Size )
	, m_Capacity( a_Collection.m_Capacity )
{
	a_Collection.m_Data = nullptr;
	a_Collection.m_Size = 0u;
	a_Collection.m_Capacity = 0u;
}

template < typename T >
Collection< T >::Collection( SizeType a_Capacity )
	: BaseType( ( T* )std::malloc( a_Capacity * sizeof( T ) ), 0u )
	, m_Capacity( a_Capacity )
{}

template < typename T >
Collection< T >::Collection( const ICollection< T >& a_Collection )
	: BaseType( ( T* )std::malloc( a_Collection.Size() * sizeof( T ) ), a_Collection.Size() )
	, m_Capacity( a_Collection.Size() )
{
	std::copy( a_Collection.Begin(), a_Collection.End(), this->Begin() );
}

template < typename T >
Collection< T >::Collection( const ICollection< T >& a_Collection, SizeType a_Capacity )
	: BaseType( ( T* )std::malloc( a_Capacity * sizeof( T ) ), a_Collection.Size() )
	, m_Capacity( a_Capacity )
{
	std::copy_n( a_Collection.Begin(), a_Collection.Size(), this->Begin() );
}


template < typename T >
void Collection< T >::Add( const T& a_Value )
{
	_ASSERT_EXPR( !Full(), "Can not add more than capacity." );

	new ( this->m_Data + this->m_Size++ ) T( a_Value );
}

template < typename T >
class DeferredIterator : public IForwardIterator< T >
{
public:

	using BaseType = IForwardIterator< T >;
	using TraitsType = std::iterator_traits< DeferredIterator >;
	using CategoryType = typename TraitsType::iterator_category;
	using DifferenceType = typename TraitsType::difference_type;
	using ValueType = typename TraitsType::value_type;
	using ReferenceType = typename TraitsType::reference;
	using PointerType = typename TraitsType::pointer;
	using EnumeratorType = typename BaseType::EnumeratorType;
	using CEnumeratorType = typename BaseType::CEnumeratorType;

	DeferredIterator() = default;
	DeferredIterator( const DeferredIterator& ) = default;
	DeferredIterator( DeferredIterator&& ) = default;
	DeferredIterator& operator=( const DeferredIterator& ) = default;
	DeferredIterator& operator=( DeferredIterator&& ) = default;

	DeferredIterator( Deferred< T >* a_Deferred, size_t a_Index )
		: m_Index( -1 )
		, m_Value( nullptr )
		, m_Deferred( a_Deferred )

	{
		for ( int i = -1; i < ( int )a_Index; ++i ) ++*this;
	}

	ReferenceType operator*() const { return *m_Value; }
	PointerType operator->() const { return m_Value; }
	DeferredIterator& operator++() { IIncrement(); return *this; }
	DeferredIterator operator++( int ) { DeferredIterator Temp = *this; ++* this; return Temp; }
	bool operator==( const DeferredIterator& a_Iterator ) const { return m_Value == a_Iterator.m_Value; }
	bool operator!=( const DeferredIterator& a_Iterator ) const { return m_Value != a_Iterator.m_Value; }

protected:

	EnumeratorType IToEnumerator() { return *this; }
	ReferenceType IDereference() const { return *m_Value; }
	PointerType IArrow() const { return m_Value; }
	void IIncrement();

private:

	int64_t m_Index;
	T* m_Value;
	Deferred< T >* m_Deferred;
};

template < typename T >
class Deferred : public ICollection< T >
{
public:

	using ValueType = T;
	using SizeType = size_t;
	using DifferenceType = std::ptrdiff_t;
	using BaseType = ICollection< T >;
	using SelectorType = Invoker< T* >;

	using IteratorType = DeferredIterator< T >;
	using CIteratorType = DeferredIterator< const T >;
	using EnumeratorType = Enumerator< T >;
	using CEnumeratorType = CEnumerator< T >;
	using EnumerableType = Enumerable< T >;
	using CEnumerableType = CEnumerable< T >;

	Deferred() = default;
	Deferred( const Deferred& ) = default;
	Deferred( Deferred&& ) = default;

	Deferred( SizeType a_Capacity, SelectorType&& a_Selector )
		: m_Cache( a_Capacity )
		, m_Selector( std::move( a_Selector ) )
	{}

	IteratorType Begin() { return IteratorType( this, 0 ); }
	CIteratorType Begin() const { return CIteratorType( this, 0 ); }
	CIteratorType CBegin() const { return CIteratorType( this, 0 ); }
	IteratorType End() { return IteratorType(); }
	CIteratorType End() const { return CIteratorType(); }
	CIteratorType CEnd() const { return CIteratorType(); }

	SizeType Size() const 
	{
		Progress();
		return m_Cache.Size();
	}

protected:

	SizeType Progress( SizeType a_Count = -1 ) const
	{
		// Do something that finds the next element.
		if ( a_Count == 0u || m_Cache.Full() )
		{
			return 0u;
		}

		SizeType Count = 0u;

		if ( a_Count == -1 )
		{
			while ( ValueType* Value = m_Selector.Invoke() )
			{
				m_Cache.Add( Value );
				++Count;
			}

			return Count;
		}

		for ( SizeType i = 0; i < a_Count; ++i )
		{
			if ( ValueType* Value = m_Selector.Invoke() )
			{
				m_Cache.Add( Value );
				++Count;
				continue;
			}

			break;
		}

		return Count;
	}

	T* Get( SizeType a_Index ) const
	{
		Progress( a_Index - m_Cache.Size() + 1u );

		if ( !( a_Index < m_Cache.Size() ) )
		{
			return nullptr;
		}

		return m_Cache[ a_Index ];
	}

	std::vector< T* > ToVector()
	{
		std::vector< T* > Result;

		for ( auto& val : *this )
		{
			Result.push_back( &val );
		}

		return Result;
	}

	SizeType ISize() const { return Size(); }
	EnumeratorType IBegin() { return Begin(); }
	EnumeratorType IEnd() { return End(); }
	EnumerableType IToEnumerable() { return EnumerableType( Size(), Begin(), End() ); }

private:

	friend class DeferredIterator< T >;

	mutable Collection< T* > m_Cache;
	SelectorType             m_Selector;
};

template < typename T >
void DeferredIterator< T >::IIncrement()
{
	m_Value = m_Deferred->Get( ++m_Index );
}

template < typename T >
Deferred< T > ICollection< T >::FindAll( const T& a_Value )
{
	return Deferred< T >( Size(), [ Begin = this->Begin(), End = this->End(), ValueToFind = &a_Value ]() mutable -> T*
	{
		while ( Begin != End )
		{
			T& Value = *Begin;
			++Begin;

			if ( Value == *ValueToFind )
			{
				return &Value;
			}
		}

		return nullptr;
	} );
}

template < typename T >
template < typename _Predicate >
Deferred< T > ICollection< T >::FindAll( _Predicate&& a_Value )
{
	/*return Deferred< T >( Size(), [ Begin = this->Begin(), End = this->End(), Predicate = a_Value]() mutable -> T*
		{
			while ( Begin != End )
			{
				T& Value = *Begin;
				++Begin;

				if ( Value == *ValueToFind )
				{
					return &Value;
				}
			}

	return nullptr;
		} );*/
}

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

	template < typename = std::enable_if_t< std::is_const_v< T > > >
	Enumerable( Enumerable< std::remove_const_t< T > >&& a_Enumerable )
		: m_Size( a_Enumerable.m_Size )
		, m_Begin( std::move( a_Enumerable.m_Begin ) )
		, m_End( std::move( a_Enumerable.m_End ) )
	{}

	template < typename It >
	Enumerable( size_t a_Size, It&& a_Begin, It&& a_End )
		: m_Size( a_Size )
		, m_Begin( std::forward< It >( a_Begin ) )
		, m_End( std::forward< It >( a_End ) )
	{
		m_Begin.m_Enumerable = this;
		m_End.m_Enumerable = this;
	}

	template < typename It, typename = std::enable_if_t< !std::is_enumerator_v< It > > >
	Enumerable( It& a_Begin, It& a_End )
		: m_Size( std::distance( a_Begin, a_End ) )
		, m_Begin( a_Begin )
		, m_End( a_End )
	{}

	const std::type_info& GetUnderlyingType() const { return m_Begin.GetUnderlyingType(); }
	EnumerableType ToEnumerable() { return *this; }
	CEnumerableType ToEnumerable() const { return *this; }
	CEnumerableType ToCEnumerable() const { return *this; }
	size_t Size() const { return m_Size; }
	size_t MaxSize() const { return static_cast< size_t >( -1 ); }

	IteratorType Begin() { return m_Begin; }
	const CIteratorType& Begin() const { return m_Begin; }
	const CIteratorType& CBegin() const { return m_Begin; }	
	RIteratorType RBegin() { return m_Begin.Reverse(); }
	CRIteratorType RBegin() const { return m_Begin.Reverse(); }
	CRIteratorType CRBegin() const { return m_Begin.Reverse(); }
	IteratorType End() { return m_End; }
	const CIteratorType& End() const { return m_End; }
	const CIteratorType& CEnd() const { return m_End; }
	RIteratorType REnd() { return m_End.Reverse(); }
	CRIteratorType REnd() const { return m_End.Reverse(); }
	CRIteratorType CREnd() const { return m_End.Reverse(); }

protected:

	template < class > friend class Enumerable;

	size_t ISize() const { return Size(); }
	size_t IMaxSize() const { return MaxSize(); }
	EnumerableType IToEnumerable() { return *this; }
	IteratorType IBegin() { return m_Begin; }
	IteratorType IEnd() { return m_End; }

	size_t m_Size;
	IteratorType m_Begin;
	IteratorType m_End;
};

#pragma region EnumeratorDefinitions
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
	if constexpr ( std::is_bidirectional_iterator_v< It > )
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
	if constexpr ( std::is_bidirectional_iterator_v< It > )
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
	if constexpr ( std::is_bidirectional_iterator_v< It > )
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
	if constexpr ( std::is_bidirectional_iterator_v< It > )
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
	if constexpr ( std::is_random_access_iterator_v< It > )
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
	if constexpr ( std::is_random_access_iterator_v< It > )
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
	if constexpr ( std::is_random_access_iterator_v< It > )
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
	if constexpr ( std::is_random_access_iterator_v< It > )
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
	if constexpr ( std::is_random_access_iterator_v< It > )
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
	if constexpr ( std::is_random_access_iterator_v< It > )
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
#pragma endregion

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

	template < typename T > auto begin( ICollection< T >& a_Collection ) { return a_Collection.Begin(); }
	template < typename T > auto begin( const ICollection< T >& a_Collection ) { return a_Collection.Begin(); }
	template < typename T > auto cbegin( const ICollection< T >& a_Collection ) { return a_Collection.CBegin(); }
	template < typename T > auto rbegin( ICollection< T >& a_Collection ) { return a_Collection.RBegin(); }
	template < typename T > auto rbegin( const ICollection< T >& a_Collection ) { return a_Collection.RBegin(); }
	template < typename T > auto crbegin( const ICollection< T >& a_Collection ) { return a_Collection.CRBegin(); }
	template < typename T > auto end( ICollection< T >& a_Collection ) { return a_Collection.End(); }
	template < typename T > auto end( const ICollection< T >& a_Collection ) { return a_Collection.End(); }
	template < typename T > auto cend( const ICollection< T >& a_Collection ) { return a_Collection.CEnd(); }
	template < typename T > auto rend( ICollection< T >& a_Collection ) { return a_Collection.REnd(); }
	template < typename T > auto rend( const ICollection< T >& a_Collection ) { return a_Collection.REnd(); }
	template < typename T > auto crend( const ICollection< T >& a_Collection ) { return a_Collection.CREnd(); }

	template < typename T > auto begin( IContiguousCollection< T >& a_Collection ) { return a_Collection.Begin(); }
	template < typename T > auto begin( const IContiguousCollection< T >& a_Collection ) { return a_Collection.Begin(); }
	template < typename T > auto cbegin( const IContiguousCollection< T >& a_Collection ) { return a_Collection.CBegin(); }
	template < typename T > auto rbegin( IContiguousCollection< T >& a_Collection ) { return a_Collection.RBegin(); }
	template < typename T > auto rbegin( const IContiguousCollection< T >& a_Collection ) { return a_Collection.RBegin(); }
	template < typename T > auto crbegin( const IContiguousCollection< T >& a_Collection ) { return a_Collection.CRBegin(); }
	template < typename T > auto end( IContiguousCollection< T >& a_Collection ) { return a_Collection.End(); }
	template < typename T > auto end( const IContiguousCollection< T >& a_Collection ) { return a_Collection.End(); }
	template < typename T > auto cend( const IContiguousCollection< T >& a_Collection ) { return a_Collection.CEnd(); }
	template < typename T > auto rend( IContiguousCollection< T >& a_Collection ) { return a_Collection.REnd(); }
	template < typename T > auto rend( const IContiguousCollection< T >& a_Collection ) { return a_Collection.REnd(); }
	template < typename T > auto crend( const IContiguousCollection< T >& a_Collection ) { return a_Collection.CREnd(); }

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

	template < typename T > auto begin( Collection< T >& a_Collection ) { return a_Collection.Begin(); }
	template < typename T > auto begin( const Collection< T >& a_Collection ) { return a_Collection.Begin(); }
	template < typename T > auto cbegin( const Collection< T >& a_Collection ) { return a_Collection.CBegin(); }
	template < typename T > auto rbegin( Collection< T >& a_Collection ) { return a_Collection.RBegin(); }
	template < typename T > auto rbegin( const Collection< T >& a_Collection ) { return a_Collection.RBegin(); }
	template < typename T > auto crbegin( const Collection< T >& a_Collection ) { return a_Collection.CRBegin(); }
	template < typename T > auto end( Collection< T >& a_Collection ) { return a_Collection.End(); }
	template < typename T > auto end( const Collection< T >& a_Collection ) { return a_Collection.End(); }
	template < typename T > auto cend( const Collection< T >& a_Collection ) { return a_Collection.CEnd(); }
	template < typename T > auto rend( Collection< T >& a_Collection ) { return a_Collection.REnd(); }
	template < typename T > auto rend( const Collection< T >& a_Collection ) { return a_Collection.REnd(); }
	template < typename T > auto crend( const Collection< T >& a_Collection ) { return a_Collection.CREnd(); }

	template < typename T >
	struct iterator_traits< IForwardIterator< T > >
	{
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using reference = T&;
		using pointer = T*;
	};

	template < typename T >
	struct iterator_traits< IBidirectionalIterator< T > >
	{
		using iterator_category = std::bidirectional_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using reference = T&;
		using pointer = T*;
	};

	template < typename T >
	struct iterator_traits< IRandomAccessIterator< T > >
	{
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
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
	struct iterator_traits< DeferredIterator< T > >
	{
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using reference = T&;
		using pointer = T*;
	};

	template < typename T >
	struct iterator_traits< Enumerator< T > >
	{
		using iterator_category = std::random_access_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using reference = T&;
		using pointer = T*;
	};
}
