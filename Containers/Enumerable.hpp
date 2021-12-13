#pragma once
#include <type_traits>
#include <iterator>
#include <vector>
#include <array>
#include <set>
#include <bitset>

#include "ContainerTraits.hpp"
#include "Reference.hpp"
#include "Invoker.hpp"

using namespace std;

template < typename T, size_t SIZE >
class ReadOnlyArray;

template < typename T, size_t SIZE >
class Array;

template < typename ValueType >
class Enumerable
{
private:

	template < typename T >
	struct LessPtr
	{
		bool operator()( const T* Left, const T* Right ) const
		{
			return *Left < *Right;
		}
	};

	enum class OperatorType : char
	{
		FBase,
		RBase,
		FClone,
		RClone,
		FConvert,
		RConvert,
		FDelete,
		RDelete,
		FBegin,
		RBegin,
		FEnd,
		REnd,
		FShift,
		RShift,
		FDereference,
		RDereference,
		FDistance,
		RDistance,
		FEquality,
		REquality,
		FGreaterEqual,
		RGreaterEqual,
		FLesserEqual,
		RLesserEqual,
		FAssign,
		RAssign
	};

public:

	class RIterator;
	class CIterator;
	class CRIterator;

	class Iterator
	{
	public:

		using iterator_category = random_access_iterator_tag;
		using difference_type   = ptrdiff_t;
		using value_type        = ValueType;
		using pointer           = value_type*;
		using reference         = value_type&;

	private:

		Iterator( const Enumerable< ValueType >* a_Enumerable, void* a_Iterator )
			: m_Enumerable( a_Enumerable )
			, m_Iterator( a_Iterator )
		{ }

	public:

		Iterator( const Iterator& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_Iterator.m_Iterator, 0, 0, OperatorType::FClone ) )
		{ }

		Iterator( const Iterator&& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
			, m_Iterator( a_Iterator.m_Iterator )
		{ }

		Iterator( const RIterator& a_RIterator )
			: m_Enumerable( a_RIterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_RIterator.m_Iterator, 0, m_Enumerable, OperatorType::FConvert ) )
		{ }

		Iterator( const RIterator&& a_RIterator )
			: m_Enumerable( a_RIterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_RIterator.m_Iterator, 0, m_Enumerable, OperatorType::FConvert ) )
		{ }
		
		~Iterator()
		{
			m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FDelete );
		}

		inline RIterator RBase()
		{
			return RIterator( m_Enumerable, m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RBase ) );
		}

		inline RIterator RBase() const
		{
			return RIterator( m_Enumerable, m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RBase ) );
		}

		inline Iterator& operator++()
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( 1 ), m_Enumerable, OperatorType::FShift );
			return *this;
		}

		inline Iterator operator++( int )
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FClone );
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( 1 ), m_Enumerable, OperatorType::FShift );
			return Iterator( m_Enumerable, New );
		}

		inline Iterator& operator--()
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( -1 ), m_Enumerable, OperatorType::FShift );
			return *this;
		}

		inline Iterator operator--( int )
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FClone );
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( -1 ), m_Enumerable, OperatorType::FShift );
			return Iterator( m_Enumerable, New );
		}

		inline Iterator& operator+=( int a_Places )
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( a_Places ), m_Enumerable, OperatorType::FShift );
			return *this;
		}

		inline Iterator& operator-=( int a_Places )
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( -a_Places ), m_Enumerable, OperatorType::FShift );
			return *this;
		}

		inline Iterator operator+( int a_Places ) const
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FClone );
			return Iterator( m_Enumerable, m_Enumerable->m_Operator( New, reinterpret_cast< void* >( a_Places ), m_Enumerable, OperatorType::FShift ) );
		}

		inline Iterator operator-( int a_Places ) const
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FClone );
			return Iterator( m_Enumerable, m_Enumerable->m_Operator( New, reinterpret_cast< void* >( -a_Places ), m_Enumerable, OperatorType::FShift ) );
		}

		inline int operator-( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return reinterpret_cast< int >( m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::FDistance ) );
		}
		
		inline Iterator& operator=( Iterator& a_Iterator )
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, 0, OperatorType::FAssign );
			return *this;
		}

		inline ValueType& operator*()
		{
			return *reinterpret_cast< ValueType* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FDereference ) );
		}

		inline const ValueType& operator*() const
		{
			return *reinterpret_cast< ValueType* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FDereference ) );
		}

		inline ValueType* operator->()
		{
			return *reinterpret_cast< ValueType* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FDereference ) );
		}

		inline const ValueType* operator->() const
		{
			return *reinterpret_cast< ValueType* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FDereference ) );
		}

		inline bool operator==( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, 0, OperatorType::FEquality );
		}

		inline bool operator==( const CIterator& a_CIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CIterator.m_Iterator, 0, OperatorType::FEquality );
		}

		inline bool operator!=( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return !m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, 0, OperatorType::FEquality );
		}

		inline bool operator!=( const CIterator& a_CIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return !m_Enumerable->m_Operator( m_Iterator, a_CIterator.m_Iterator, 0, OperatorType::FEquality );
		}

		inline bool operator>( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::FGreaterEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator>( const CIterator& a_CIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CIterator.m_Iterator, m_Enumerable, OperatorType::FGreaterEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator>=( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::FGreaterEqual );
		}

		inline bool operator>=( const CIterator& a_CIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CIterator.m_Iterator, m_Enumerable, OperatorType::FGreaterEqual );
		}

		inline bool operator<( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::FLesserEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator<( const CIterator& a_CIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CIterator.m_Iterator, m_Enumerable, OperatorType::FLesserEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator<=( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::FLesserEqual );
		}

		inline bool operator<=( const CIterator& a_CIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CIterator.m_Iterator, m_Enumerable, OperatorType::FLesserEqual );
		}

	private:

		friend class Enumerable< ValueType >;
		friend class Enumerable< ValueType >::RIterator;

		const Enumerable< ValueType >* m_Enumerable;
		void*						   m_Iterator;

	};

	class RIterator
	{
	public:

		using iterator_category = random_access_iterator_tag;
		using difference_type = ptrdiff_t;
		using value_type = ValueType;
		using pointer = value_type*;
		using reference = value_type&;

	private:

		RIterator( const Enumerable< ValueType >* a_Enumerable, void* a_Iterator )
			: m_Enumerable( a_Enumerable )
			, m_Iterator( a_Iterator )
		{ }

	public:

		RIterator( const Iterator& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_Iterator.m_Iterator, 0, m_Enumerable, OperatorType::RConvert ) )
		{ }

		RIterator( const Iterator&& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_Iterator.m_Iterator, 0, m_Enumerable, OperatorType::RConvert ) )
		{ }

		RIterator( const RIterator& a_RIterator )
			: m_Enumerable( a_RIterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_RIterator.m_Iterator, 0, 0, OperatorType::RClone ) )
		{ }

		RIterator( const RIterator&& a_RIterator )
			: m_Enumerable( a_RIterator.m_Enumerable )
			, m_Iterator( a_RIterator.m_Iterator )
		{ }

		~RIterator()
		{
			m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RDelete );
		}

		inline Iterator Base()
		{
			return Iterator( m_Enumerable, m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FBase ) );
		}

		inline Iterator Base() const
		{
			return Iterator( m_Enumerable, m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FBase ) );
		}

		inline RIterator& operator++()
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( 1 ), m_Enumerable, OperatorType::RShift );
			return *this;
		}

		inline RIterator operator++( int )
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RClone );
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( 1 ), m_Enumerable, OperatorType::RShift );
			return RIterator( m_Enumerable, New );
		}

		inline RIterator& operator--()
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( -1 ), m_Enumerable, OperatorType::RShift );
			return *this;
		}

		inline RIterator operator--( int )
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RClone );
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( -1 ), m_Enumerable, OperatorType::RShift );
			return RIterator( m_Enumerable, New );
		}

		inline RIterator& operator+=( int a_Places )
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( a_Places ), m_Enumerable, OperatorType::RShift );
			return *this;
		}

		inline RIterator& operator-=( int a_Places )
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( -a_Places ), m_Enumerable, OperatorType::RShift );
			return *this;
		}

		inline RIterator operator+( int a_Places ) const
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RClone );
			return RIterator( m_Enumerable, m_Enumerable->m_Operator( New, reinterpret_cast< void* >( a_Places ), m_Enumerable, OperatorType::RShift ) );
		}

		inline RIterator operator-( int a_Places ) const
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RClone );
			return RIterator( m_Enumerable, m_Enumerable->m_Operator( New, reinterpret_cast< void* >( -a_Places ), m_Enumerable, OperatorType::RShift ) );
		}

		inline int operator-( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return reinterpret_cast< int >( m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, m_Enumerable, OperatorType::RDistance ) );
		}

		inline RIterator& operator=( RIterator& a_RIterator )
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, 0, OperatorType::RAssign );
			return *this;
		}

		inline ValueType& operator*()
		{
			return *reinterpret_cast< ValueType* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RDereference ) );
		}

		inline const ValueType& operator*() const
		{
			return *reinterpret_cast< ValueType* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RDereference ) );
		}

		inline ValueType* operator->()
		{
			return *reinterpret_cast< ValueType* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RDereference ) );
		}

		inline const ValueType* operator->() const
		{
			return *reinterpret_cast< ValueType* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RDereference ) );
		}

		inline bool operator==( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, 0, OperatorType::REquality );
		}

		inline bool operator==( const CRIterator& a_CRIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CRIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CRIterator.m_Iterator, 0, OperatorType::REquality );
		}

		inline bool operator!=( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return !m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, 0, OperatorType::REquality );
		}

		inline bool operator!=( const CRIterator& a_CRIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CRIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return !m_Enumerable->m_Operator( m_Iterator, a_CRIterator.m_Iterator, 0, OperatorType::REquality );
		}

		inline bool operator>( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, m_Enumerable, OperatorType::RGreaterEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator>( const CRIterator& a_CRIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CRIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CRIterator.m_Iterator, m_Enumerable, OperatorType::RGreaterEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator>=( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, m_Enumerable, OperatorType::RGreaterEqual );
		}

		inline bool operator>=( const CRIterator& a_CRIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CRIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CRIterator.m_Iterator, m_Enumerable, OperatorType::RGreaterEqual );
		}

		inline bool operator<( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, m_Enumerable, OperatorType::RLesserEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator<( const CRIterator& a_CRIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CRIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CRIterator.m_Iterator, m_Enumerable, OperatorType::RLesserEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator<=( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, m_Enumerable, OperatorType::RLesserEqual );
		}

		inline bool operator<=( const CRIterator& a_CRIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CRIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CRIterator.m_Iterator, m_Enumerable, OperatorType::RLesserEqual );
		}

	private:

		friend class Enumerable< ValueType >;
		friend class Enumerable< ValueType >::Iterator;

		const Enumerable< ValueType >* m_Enumerable;
		void*						   m_Iterator;
	};

	class CIterator
	{
	public:

		using iterator_category = random_access_iterator_tag;
		using difference_type = ptrdiff_t;
		using value_type = const ValueType;
		using pointer = value_type*;
		using reference = value_type&;

	private:

		CIterator( const Enumerable< ValueType >* a_Enumerable, void* a_Iterator )
			: m_Enumerable( a_Enumerable )
			, m_Iterator( a_Iterator )
		{ }

	public:

		CIterator( const Iterator& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_Iterator.m_Iterator, 0, 0, OperatorType::FClone ) )
		{ }

		CIterator( const Iterator&& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
			, m_Iterator( a_Iterator.m_Iterator )
		{ }

		CIterator( const CIterator& a_CIterator )
			: m_Enumerable( a_CIterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_CIterator.m_Iterator, 0, 0, OperatorType::FClone ) )
		{ }

		CIterator( const CIterator&& a_CIterator )
			: m_Enumerable( a_CIterator.m_Enumerable )
			, m_Iterator( a_CIterator.m_Iterator )
		{ }

		CIterator( const RIterator& a_RIterator )
			: m_Enumerable( a_RIterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_RIterator.m_Iterator, 0, m_Enumerable, OperatorType::FConvert ) )
		{ }

		CIterator( const RIterator&& a_RIterator )
			: m_Enumerable( a_RIterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_RIterator.m_Iterator, 0, m_Enumerable, OperatorType::FConvert ) )
		{ }

		CIterator( const CRIterator& a_CRIterator )
			: m_Enumerable( a_CRIterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_CRIterator.m_Iterator, 0, m_Enumerable, OperatorType::FConvert ) )
		{ }

		CIterator( const CRIterator&& a_CRIterator )
			: m_Enumerable( a_CRIterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_CRIterator.m_Iterator, 0, m_Enumerable, OperatorType::FConvert ) )
		{ }

		~CIterator()
		{
			m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FDelete );
		}

		inline CRIterator RBase()
		{
			return CRIterator( m_Enumerable, m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RBase ) );
		}

		inline CRIterator RBase() const
		{
			return CRIterator( m_Enumerable, m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RBase ) );
		}

		inline CIterator& operator++()
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( 1 ), m_Enumerable, OperatorType::FShift );
			return *this;
		}

		inline CIterator operator++( int )
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FClone );
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( 1 ), m_Enumerable, OperatorType::FShift );
			return Iterator( m_Enumerable, New );
		}

		inline CIterator& operator--()
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( -1 ), m_Enumerable, OperatorType::FShift );
			return *this;
		}

		inline CIterator operator--( int )
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FClone );
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( -1 ), m_Enumerable, OperatorType::FShift );
			return Iterator( m_Enumerable, New );
		}

		inline CIterator& operator+=( int a_Places )
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( a_Places ), m_Enumerable, OperatorType::FShift );
			return *this;
		}

		inline CIterator& operator-=( int a_Places )
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( -a_Places ), m_Enumerable, OperatorType::FShift );
			return *this;
		}

		inline CIterator operator+( int a_Places ) const
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FClone );
			return Iterator( m_Enumerable, m_Enumerable->m_Operator( New, reinterpret_cast< void* >( a_Places ), m_Enumerable, OperatorType::FShift ) );
		}

		inline CIterator operator-( int a_Places ) const
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FClone );
			return Iterator( m_Enumerable, m_Enumerable->m_Operator( New, reinterpret_cast< void* >( -a_Places ), m_Enumerable, OperatorType::FShift ) );
		}

		inline int operator-( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return reinterpret_cast< int >( m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::FDistance ) );
		}

		inline int operator-( const CIterator& a_CIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return reinterpret_cast< int >( m_Enumerable->m_Operator( m_Iterator, a_CIterator.m_Iterator, m_Enumerable, OperatorType::FDistance ) );
		}

		inline CIterator& operator=( Iterator& a_Iterator )
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, 0, OperatorType::FAssign );
			return *this;
		}

		inline CIterator& operator=( CIterator& a_CIterator )
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			m_Enumerable->m_Operator( m_Iterator, a_CIterator.m_Iterator, 0, OperatorType::FAssign );
			return *this;
		}

		inline const ValueType& operator*() const
		{
			return *reinterpret_cast< ValueType* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FDereference ) );
		}

		inline const ValueType* operator->() const
		{
			return *reinterpret_cast< ValueType* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FDereference ) );
		}

		inline bool operator==( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, 0, OperatorType::FEquality );
		}

		inline bool operator==( const CIterator& a_CIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CIterator.m_Iterator, 0, OperatorType::FEquality );
		}

		inline bool operator!=( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return !m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, 0, OperatorType::FEquality );
		}

		inline bool operator!=( const CIterator& a_CIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return !m_Enumerable->m_Operator( m_Iterator, a_CIterator.m_Iterator, 0, OperatorType::FEquality );
		}

		inline bool operator>( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::FGreaterEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator>( const CIterator& a_CIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CIterator.m_Iterator, m_Enumerable, OperatorType::FGreaterEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator>=( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::FGreaterEqual );
		}

		inline bool operator>=( const CIterator& a_CIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CIterator.m_Iterator, m_Enumerable, OperatorType::FGreaterEqual );
		}

		inline bool operator<( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::FLesserEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator<( const CIterator& a_CIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CIterator.m_Iterator, m_Enumerable, OperatorType::FLesserEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator<=( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::FLesserEqual );
		}

		inline bool operator<=( const CIterator& a_CIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CIterator.m_Iterator, m_Enumerable, OperatorType::FLesserEqual );
		}

	private:

		friend class Enumerable< ValueType >;
		friend class Enumerable< ValueType >::Iterator;
		friend class Enumerable< ValueType >::RIterator;
		friend class Enumerable< ValueType >::CRIterator;

		const Enumerable< ValueType >* m_Enumerable;
		void*						   m_Iterator;

	};

	class CRIterator
	{
	public:

		using iterator_category = random_access_iterator_tag;
		using difference_type = ptrdiff_t;
		using value_type = const ValueType;
		using pointer = value_type*;
		using reference = value_type&;

	private:

		CRIterator( const Enumerable< ValueType >* a_Enumerable, void* a_Iterator )
			: m_Enumerable( a_Enumerable )
			, m_Iterator( a_Iterator )
		{ }

	public:

		CRIterator( const Iterator& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_Iterator.m_Iterator, 0, m_Enumerable, OperatorType::RConvert ) )
		{ }

		CRIterator( const Iterator&& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_Iterator.m_Iterator, 0, m_Enumerable, OperatorType::RConvert ) )
		{ }

		CRIterator( const CIterator& a_CIterator )
			: m_Enumerable( a_CIterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_CIterator.m_Iterator, 0, m_Enumerable, OperatorType::RConvert ) )
		{ }

		CRIterator( const CIterator&& a_CIterator )
			: m_Enumerable( a_CIterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_CIterator.m_Iterator, 0, m_Enumerable, OperatorType::RConvert ) )
		{ }

		CRIterator( const RIterator& a_RIterator )
			: m_Enumerable( a_RIterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_RIterator.m_Iterator, 0, 0, OperatorType::RClone ) )
		{ }

		CRIterator( const RIterator&& a_RIterator )
			: m_Enumerable( a_RIterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_RIterator.m_Iterator, 0, 0, OperatorType::RClone ) )
		{ }

		CRIterator( const CRIterator& a_CRIterator )
			: m_Enumerable( a_CRIterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_CRIterator.m_Iterator, 0, 0, OperatorType::RClone ) )
		{ }

		CRIterator( const CRIterator&& a_CRIterator )
			: m_Enumerable( a_CRIterator.m_Enumerable )
			, m_Iterator( a_CRIterator.m_Iterator )
		{ }

		~CRIterator()
		{
			m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RDelete );
		}
		
		inline CIterator Base()
		{
			return CIterator( m_Enumerable, m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FBase ) );
		}

		inline CIterator Base() const
		{
			return CIterator( m_Enumerable, m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FBase ) );
		}

		inline CRIterator& operator++()
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( 1 ), m_Enumerable, OperatorType::RShift );
			return *this;
		}

		inline CRIterator operator++( int )
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RClone );
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( 1 ), m_Enumerable, OperatorType::RShift );
			return RIterator( m_Enumerable, New );
		}

		inline CRIterator& operator--()
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( -1 ), m_Enumerable, OperatorType::RShift );
			return *this;
		}

		inline CRIterator operator--( int )
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RClone );
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( -1 ), m_Enumerable, OperatorType::RShift );
			return RIterator( m_Enumerable, New );
		}

		inline CRIterator& operator+=( int a_Places )
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( a_Places ), m_Enumerable, OperatorType::RShift );
			return *this;
		}

		inline CRIterator& operator-=( int a_Places )
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( -a_Places ), m_Enumerable, OperatorType::RShift );
			return *this;
		}

		inline CRIterator operator+( int a_Places ) const
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RClone );
			return RIterator( m_Enumerable, m_Enumerable->m_Operator( New, reinterpret_cast< void* >( a_Places ), m_Enumerable, OperatorType::RShift ) );
		}

		inline CRIterator operator-( int a_Places ) const
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RClone );
			return RIterator( m_Enumerable, m_Enumerable->m_Operator( New, reinterpret_cast< void* >( -a_Places ), m_Enumerable, OperatorType::RShift ) );
		}

		inline int operator-( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return reinterpret_cast< int >( m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, m_Enumerable, OperatorType::RDistance ) );
		}

		inline int operator-( const CRIterator& a_CRIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CRIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return reinterpret_cast< int >( m_Enumerable->m_Operator( m_Iterator, a_CRIterator.m_Iterator, m_Enumerable, OperatorType::RDistance ) );
		}

		inline CRIterator& operator=( RIterator& a_RIterator )
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, 0, OperatorType::RAssign );
			return *this;
		}

		inline CRIterator& operator=( CRIterator& a_CRIterator )
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CRIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			m_Enumerable->m_Operator( m_Iterator, a_CRIterator.m_Iterator, 0, OperatorType::RAssign );
			return *this;
		}

		inline const ValueType& operator*() const
		{
			return *reinterpret_cast< ValueType* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RDereference ) );
		}

		inline const ValueType* operator->() const
		{
			return *reinterpret_cast< ValueType* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RDereference ) );
		}

		inline bool operator==( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, 0, OperatorType::REquality );
		}

		inline bool operator==( const CRIterator& a_CRIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CRIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CRIterator.m_Iterator, 0, OperatorType::REquality );
		}

		inline bool operator!=( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return !m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, 0, OperatorType::REquality );
		}

		inline bool operator!=( const CRIterator& a_CRIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CRIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return !m_Enumerable->m_Operator( m_Iterator, a_CRIterator.m_Iterator, 0, OperatorType::REquality );
		}

		inline bool operator>( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, m_Enumerable, OperatorType::RGreaterEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator>( const CRIterator& a_CRIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CRIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CRIterator.m_Iterator, m_Enumerable, OperatorType::RGreaterEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator>=( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, m_Enumerable, OperatorType::RGreaterEqual );
		}

		inline bool operator>=( const CRIterator& a_CRIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CRIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CRIterator.m_Iterator, m_Enumerable, OperatorType::RGreaterEqual );
		}

		inline bool operator<( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, m_Enumerable, OperatorType::RLesserEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator<( const CRIterator& a_CRIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CRIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CRIterator.m_Iterator, m_Enumerable, OperatorType::RLesserEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator<=( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, m_Enumerable, OperatorType::RLesserEqual );
		}

		inline bool operator<=( const CRIterator& a_CRIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_CRIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_CRIterator.m_Iterator, m_Enumerable, OperatorType::RLesserEqual );
		}

	private:

		friend class Enumerable< ValueType >;
		friend class Enumerable< ValueType >::Iterator;
		friend class Enumerable< ValueType >::RIterator;
		friend class Enumerable< ValueType >::CIterator;

		const Enumerable< ValueType >* m_Enumerable;
		void*                          m_Iterator;
	};

	Enumerable( Enumerable& a_Enumerable )
		: m_Begin( a_Enumerable.m_Operator( a_Enumerable.m_Begin, 0, 0, OperatorType::FClone ) )
		, m_End( a_Enumerable.m_Operator( a_Enumerable.m_End, 0, 0, OperatorType::FClone ) )
		, m_Size( a_Enumerable.m_Size )
		, m_Operator( a_Enumerable.m_Operator )
	{ }

	Enumerable( Enumerable&& a_Enumerable )
		: m_Begin( a_Enumerable.m_Begin )
		, m_End( a_Enumerable.m_End )
		, m_Size( a_Enumerable.m_Size )
		, m_Operator( a_Enumerable.m_Operator )
	{ }

	template < typename Iter >
	Enumerable( Iter a_Begin, Iter a_End )
		: m_Begin( new Iter( a_Begin ) )
		, m_End( new Iter( a_End ) )
		, m_Size( distance( a_Begin, a_End ) )
		, m_Operator( Operator< Iter > )
	{ }

	template < typename Iter >
	Enumerable( Iter a_Begin, Iter a_End, size_t a_Size )
		: m_Begin( new Iter( a_Begin ) )
		, m_End( new Iter( a_End ) )
		, m_Size( a_Size )
		, m_Operator( Operator< Iter > )
	{ }

	template < typename Container, typename Iter = ContainerTraits::GetIter< Container >, typename = ContainerTraits::EnableIfContainer< Container, ValueType > >
	Enumerable( Container& a_Container )
		: m_Begin( new Iter( ContainerTraits::Begin( a_Container ) ) )
		, m_End( new Iter( ContainerTraits::End( a_Container ) ) )
		, m_Size( ContainerTraits::Size( a_Container ) )
		, m_Operator( Operator< Iter > )
	{ }

	~Enumerable()
	{
		m_Operator( m_Begin, 0, 0, OperatorType::FDelete );
		m_Operator( m_End,   0, 0, OperatorType::FDelete );
	}

	template < size_t LENGTH >
	inline auto AsReadOnly() const
	{
		array< ValueType, LENGTH > Result;
		size_t Length = LENGTH < m_Size ? LENGTH : m_Size;
		size_t Index = 0;
		CIterator Beg( Begin() );
		
		for ( ; Index < Length; ++Index, ++Beg )
		{
			Result[ Index ] = *Beg;
		}

		return reinterpret_cast< ReadOnlyArray< ValueType, LENGTH >& >( Result );
	}

	inline void Assign( const ValueType& a_Value )
	{
		Iterator Beg( Begin() );
		Iterator End( End() );

		for ( ; Beg != End; ++Beg )
		{
			*Beg = a_Value;
		}
	}

	inline void Assign( size_t a_Position, size_t a_Length, const ValueType& a_Value )
	{
		Iterator Beg( Begin() += a_Position );

		for ( ; a_Length > 0; --a_Length, ++Beg )
		{
			*Beg = a_Value;
		}
	}

	inline void Assign( Iterator a_Begin, const Iterator& a_End, const ValueType& a_Value )
	{
		for ( ; a_Begin != a_End; ++a_Begin )
		{
			*a_Begin = a_Value;
		}
	}

	inline auto& At( size_t a_Position )
	{
		return *( Begin() + a_Position );
	}

	inline const auto& At( size_t a_Position ) const
	{
		return *( Begin() + a_Position );
	}

	inline auto& Back()
	{
		return *( End() - 1 );
	}

	inline const auto& Back() const
	{
		return *( End() - 1 );
	}

	inline auto Combine( const Enumerable< ValueType >& a_Enumerable ) const
	{
		vector< ValueType > Result;
		size_t LengthA = m_Size;
		size_t LengthB = a_Enumerable.m_Size;
		Result.reserve( LengthA + LengthB );

		for ( CIterator Beg( Begin() ); LengthA > 0; --LengthA, ++Beg )
		{
			Result.push_back( *Beg );
		}

		for ( CIterator Beg( a_Enumerable.Begin() ); LengthB > 0; --LengthB, ++Beg )
		{
			Result.push_back( *Beg );
		}

		return reinterpret_cast< Array< ValueType >& >( Result );
	}

	inline void CopyTo( Enumerable< ValueType >& a_Enumerable ) const
	{
		size_t Length = m_Size < a_Enumerable.m_Size ? m_Size : a_Enumerable.m_Size;
		CIterator BegA( Begin() );
		Iterator BegB( a_Enumerable.Begin() );

		for ( ; Length > 0; --Length, ++BegA, ++BegB )
		{
			*BegB = *BegA;
		}
	}

	inline auto Difference( const Enumerable< ValueType >& a_Enumerable ) const
	{
		set< const ValueType*, LessPtr< ValueType > > SetA;
		set< const ValueType*, LessPtr< ValueType > > SetB;
		vector< ValueType > Result;
		size_t LengthA = m_Size;
		size_t LengthB = a_Enumerable.m_Size;

		for ( CIterator Beg( Begin() ); LengthA > 0; --LengthA, ++Beg )
		{
			SetA.insert( &*Beg );
		}

		for ( CIterator Beg( a_Enumerable.Begin() ); LengthB > 0; --LengthB, ++Beg )
		{
			SetB.insert( &*Beg );
		}

		for ( auto Iterator = SetA.begin(); Iterator != SetA.end(); ++Iterator )
		{
			if ( SetB.insert( *Iterator ).second )
			{
				Result.push_back( **Iterator );
			}
		}

		return reinterpret_cast< Array< ValueType >& >( Result );
	}

	template < typename... T >
	inline auto& EmplaceAt( Iterator& a_Where, T&&... a_Args )
	{
		return *( new ( &*a_Where ) ValueType( forward< T >( a_Args )... ) );
	}

	template < typename... T >
	inline auto& EmplaceAt( Iterator&& a_Where, T&&... a_Args )
	{
		return *( new ( &*a_Where ) ValueType( forward< T >( a_Args )... ) );
	}

	inline auto Empty() const
	{
		return !m_Size;
	}

	inline bool Equals( const Enumerable< ValueType >& a_Enumerable )
	{
		if ( m_Size != a_Enumerable.m_Size )
		{
			return false;
		}

		size_t Length = m_Size;
		CIterator BegA( Begin() );
		CIterator BegB( a_Enumerable.Begin() );
		
		for ( ; Length > 0; --Length, ++BegA, ++BegB )
		{
			if ( *BegA != *BegB )
			{
				return false;
			}
		}

		return true;
	}

	bool Exists( const Predicate< const ValueType& >& a_Predicate ) const
	{
		size_t Length = m_Size;
		CIterator Beg( Begin() );

		for ( ; Length > 0; --Length, ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return true;
			}
		}

		return false;
	}

	auto Find( const Predicate< const ValueType& >& a_Predicate )
	{
		size_t Length = m_Size;
		Iterator Beg( Begin() );

		for ( ; Length > 0; --Length, ++Beg )
		{
			ValueType& Value = *Beg;

			if ( a_Predicate.Invoke( Value ) )
			{
				return Reference< ValueType >( Value );
			}
		}

		return Reference< ValueType >();
	}

	auto Find( const Predicate< const ValueType& >& a_Predicate ) const
	{
		size_t Length = m_Size;
		CIterator Beg( Begin() );

		for ( ; Length > 0; --Length, ++Beg )
		{
			const ValueType& Value = *Beg;

			if ( a_Predicate.Invoke( Value ) )
			{
				return CReference< ValueType >( Value );
			}
		}

		return CReference< ValueType >();
	}

	auto Find( Iterator a_Begin, const Iterator& a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( ; a_Begin != a_End; ++a_Begin )
		{
			ValueType& Value = *a_Begin;

			if ( a_Predicate.Invoke( Value ) )
			{
				return Reference< ValueType >( Value );
			}
		}

		return Reference< ValueType >();
	}

	auto Find( CIterator a_Begin, const CIterator& a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( ; a_Begin != a_End; ++a_Begin )
		{
			const ValueType& Value = *a_Begin;

			if ( a_Predicate.Invoke( Value ) )
			{
				return CReference< ValueType >( Value );
			}
		}

		return CReference< ValueType >();
	}

	auto FindIterator( const Predicate< const ValueType& >& a_Predicate )
	{
		size_t Length = m_Size;
		Iterator Beg( Begin() );

		for ( ; Length > 0; --Length, ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return Beg;
			}
		}

		return End();
	}

	auto FindIterator( const Predicate< const ValueType& >& a_Predicate ) const
	{
		size_t Length = m_Size;
		CIterator Beg( Begin() );

		for ( ; Length > 0; --Length, ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return Beg;
			}
		}

		return End();
	}

	auto FindIterator( Iterator a_Begin, const Iterator& a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		for ( ; a_Begin != a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				return a_Begin;
			}
		}

		return End();
	}

	auto FindIterator( CIterator a_Begin, const CIterator& a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		for ( ; a_Begin != a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				return a_Begin;
			}
		}

		return End();
	}

	auto FindAll( const Predicate< const ValueType& >& a_Predicate )
	{
		vector< Reference< ValueType > > Result;
		size_t Length = m_Size;
		Iterator Beg( Begin() );

		for ( ; Length > 0; --Length, ++Beg )
		{
			ValueType& Value = *Beg;

			if ( a_Predicate.Invoke( Value ) )
			{
				Result.emplace_back( Value );
			}
		}

		return reinterpret_cast< Array< Reference< ValueType > >& >( Result );
	}

	auto FindAll( const Predicate< const ValueType& >& a_Predicate ) const
	{
		vector< CReference< ValueType > > Result;
		size_t Length = m_Size;
		CIterator Beg( Begin() );

		for ( ; Length > 0; --Length, ++Beg )
		{
			const ValueType& Value = *Beg;

			if ( a_Predicate.Invoke( Value ) )
			{
				Result.emplace_back( Value );
			}
		}

		return reinterpret_cast< Array< CReference< ValueType > >& >( Result );
	}

	auto FindAll( Iterator a_Begin, const Iterator& a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		vector< Reference< ValueType > > Result;
		
		for ( ; a_Begin != a_End; ++a_Begin )
		{
			ValueType& Value = *a_Begin;

			if ( a_Predicate.Invoke( Value ) )
			{
				Result.emplace_back( Value );
			}
		}

		return reinterpret_cast< Array< Reference< ValueType > >& >( Result );
	}

	auto FindAll( CIterator a_Begin, const CIterator& a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		vector< CReference< ValueType > > Result;

		for ( ; a_Begin != a_End; ++a_Begin )
		{
			const ValueType& Value = *a_Begin;

			if ( a_Predicate.Invoke( Value ) )
			{
				Result.emplace_back( Value );
			}
		}

		return reinterpret_cast< Array< CReference< ValueType > >& >( Result );
	}

	auto FindIteratorAll( const Predicate< const ValueType& >& a_Predicate )
	{
		vector< Iterator > Result;
		size_t Length = m_Size;
		Iterator Beg( Begin() );

		for ( ; Length > 0; --Length, ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				Result.emplace_back( Beg );
			}
		}

		return reinterpret_cast< Array< Iterator >& >( Result );
	}

	auto FindIteratorAll( const Predicate< const ValueType& >& a_Predicate ) const
	{
		vector< CIterator > Result;
		size_t Length = m_Size;
		CIterator Beg( Begin() );

		for ( ; Length > 0; --Length, ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				Result.emplace_back( Beg );
			}
		}

		return reinterpret_cast< Array< CIterator >& >( Result );
	}

	auto FindIteratorAll( Iterator a_Begin, const Iterator& a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		vector< Iterator > Result;

		for ( ; a_Begin != a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.emplace_back( a_Begin );
			}
		}

		return reinterpret_cast< Array< Iterator >& >( Result );
	}

	auto FindIteratorAll( CIterator a_Begin, const CIterator& a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		vector< CIterator > Result;

		for ( ; a_Begin != a_End; ++a_Begin )
		{
			if ( a_Predicate.Invoke( *a_Begin ) )
			{
				Result.emplace_back( a_Begin );
			}
		}

		return reinterpret_cast< Array< CIterator >& >( Result );
	}
	
	auto FindLast( const Predicate< const ValueType& >& a_Predicate )
	{
		RIterator Beg( RBegin() );
		RIterator End( REnd() );

		for ( ; Beg != End; ++Beg )
		{
			ValueType& Value = *Beg;

			if ( a_Predicate.Invoke( Value ) )
			{
				return Reference< ValueType >( Value );
			}
		}

		return Reference< ValueType >();
	}

	auto FindLast( const Predicate< const ValueType& >& a_Predicate ) const
	{
		CRIterator Beg( RBegin() );
		CRIterator End( REnd() );

		for ( ; Beg != End; ++Beg )
		{
			const ValueType& Value = *Beg;

			if ( a_Predicate.Invoke( Value ) )
			{
				return CReference< ValueType >( Value );
			}
		}

		return CReference< ValueType >();
	}

	auto FindLast( const Iterator& a_Begin, const Iterator& a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		RIterator Beg( a_End );
		RIterator End( a_Begin );

		for ( ; Beg != End; ++Beg )
		{
			ValueType& Value = *Beg;

			if ( a_Predicate.Invoke( Value ) )
			{
				return Reference< ValueType >( Value );
			}
		}

		return Reference< ValueType >();
	}

	auto FindLast( const Iterator& a_Begin, const Iterator& a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		CRIterator Beg( a_End );
		CRIterator End( a_Begin );

		for ( ; Beg != End; ++Beg )
		{
			const ValueType& Value = *Beg;

			if ( a_Predicate.Invoke( Value ) )
			{
				return CReference< ValueType >( Value );
			}
		}

		return CReference< ValueType >();
	}

	auto FindIteratorLast( const Predicate< const ValueType& >& a_Predicate )
	{
		RIterator Beg( RBegin() );
		RIterator End( REnd() );

		for ( ; Beg != End; ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return Beg.Base();
			}
		}

		return Enumerable< ValueType >::End();
	}

	auto FindIteratorLast( const Predicate< const ValueType& >& a_Predicate ) const
	{
		CRIterator Beg( RBegin() );
		CRIterator End( REnd() );

		for ( ; Beg != End; ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return Beg.Base();
			}
		}

		return Enumerable< ValueType >::End();
	}

	auto FindIteratorLast( const Iterator& a_Begin, const Iterator& a_End, const Predicate< const ValueType& >& a_Predicate )
	{
		RIterator Beg( a_End );
		RIterator End( a_Begin );

		for ( ; Beg != End; ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return Beg.Base();
			}
		}

		return Enumerable< ValueType >::End();
	}

	auto FindIteratorLast( const Iterator& a_Begin, const Iterator& a_End, const Predicate< const ValueType& >& a_Predicate ) const
	{
		CRIterator Beg( a_End );
		CRIterator End( a_Begin );

		for ( ; Beg != End; ++Beg )
		{
			if ( a_Predicate.Invoke( *Beg ) )
			{
				return Beg.Base();
			}
		}

		return Enumerable< ValueType >::End();
	}

	inline void ForEach( const Action< ValueType& >& a_Action )
	{
		size_t Length = m_Size;
		Iterator Beg( Begin() );

		for ( ; Length > 0; --Length, ++Beg )
		{
			a_Action.Invoke( *Beg );
		}
	}

	inline void ForEach( const Action< const ValueType& >& a_Action ) const
	{
		size_t Length = m_Size;
		CIterator Beg( Begin() );

		for ( ; Length > 0; --Length, ++Beg )
		{
			a_Action.Invoke( *Beg );
		}
	}

	inline void ForEach( Iterator a_Begin, const Iterator& a_End, const Action< ValueType& >& a_Action )
	{
		for ( ; a_Begin != a_End; ++a_Begin )
		{
			a_Action.Invoke( *a_Begin );
		}
	}

	inline void ForEach( CIterator a_Begin, const CIterator& a_End, const Action< const ValueType& >& a_Action ) const
	{
		for ( ; a_Begin != a_End; ++a_Begin )
		{
			a_Action.Invoke( *a_Begin );
		}
	}

	inline auto& Front()
	{
		return *Begin();
	}

	inline const auto& Front() const
	{
		return *Begin();
	}

	inline auto Intersection( const Enumerable< ValueType >& a_Enumerable ) const
	{
		set< const ValueType*, LessPtr< ValueType > > SetA;
		set< const ValueType*, LessPtr< ValueType > > SetB;
		vector< ValueType > Result;
		size_t LengthA = m_Size;
		size_t LengthB = a_Enumerable.m_Size;

		for ( CIterator Beg( Begin() ); LengthA > 0; --LengthA, ++Beg )
		{
			SetA.insert( &*Beg );
		}

		for ( CIterator Beg( a_Enumerable.Begin() ); LengthB > 0; --LengthB, ++Beg )
		{
			SetB.insert( &*Beg );
		}

		bool SetAIsLargest = SetA.size() > SetB.size();
		decltype( SetA )& Smallest = !SetAIsLargest ? SetA : SetB;
		decltype( SetA )& Largest = SetAIsLargest ? SetA : SetB;
		auto Beg = !SetAIsLargest ? SetA.begin() : SetB.begin();
		auto End = !SetAIsLargest ? SetA.end() : SetB.end();

		for ( ; Beg != End; ++Beg )
		{
			if ( !Largest.insert( *Beg ).second )
			{
				Result.push_back( **Beg );
			}
		}

		return Result;
	}

	inline auto MaxSize() const
	{
		return m_Size;
	}

	void MemCopy( const void* a_Source, size_t a_Offset, size_t a_Length )
	{
		size_t Index  = a_Offset / sizeof( ValueType );
		size_t Offset = a_Offset - Index * sizeof( ValueType );
		const unsigned char* Source = ( const unsigned char* )a_Source;

		for ( Iterator Beg( Begin() + Index ); a_Length > 0; ++Beg )
		{
			auto& val = *Beg;
			void* Dest = ( unsigned char* )&*Beg + Offset;
			memcpy( Dest, a_Source, a_Length > sizeof( ValueType ) - Offset ? sizeof( ValueType ) - Offset : a_Length );
			Source += sizeof( ValueType ) - Offset;
			size_t Adjustment = a_Length > sizeof( ValueType ) - Offset ? sizeof( ValueType ) - Offset : a_Length;
			a_Length -= Adjustment;
			Offset = 0;
		}
	}

	inline void MemSet( char a_Value, size_t a_Offset, size_t a_Length )
	{

	}

	inline size_t Size() const
	{
		return m_Size;
	}

	#pragma region Iterator
	inline Iterator Begin()
	{
		return Iterator( this, m_Operator( 0, 0, this, OperatorType::FBegin ) );
	}

	inline CIterator Begin() const
	{
		return CIterator( this, m_Operator( 0, 0, this, OperatorType::FBegin ) );
	}

	inline CIterator CBegin()
	{
		return CIterator( this, m_Operator( 0, 0, this, OperatorType::FBegin ) );
	}

	inline CIterator CBegin() const
	{
		return CIterator( this, m_Operator( 0, 0, this, OperatorType::FBegin ) );
	}

	inline RIterator RBegin()
	{
		return RIterator( this, m_Operator( 0, 0, this, OperatorType::RBegin ) );
	}

	inline CRIterator RBegin() const
	{
		return CRIterator( this, m_Operator( 0, 0, this, OperatorType::RBegin ) );
	}

	inline CRIterator CRBegin()
	{
		return CRIterator( this, m_Operator( 0, 0, this, OperatorType::RBegin ) );
	}

	inline CRIterator CRBegin() const
	{
		return CRIterator( this, m_Operator( 0, 0, this, OperatorType::RBegin ) );
	}

	inline Iterator End()
	{
		return Iterator( this, m_Operator( 0, 0, this, OperatorType::FEnd ) );
	}

	inline CIterator End() const
	{
		return CIterator( this, m_Operator( 0, 0, this, OperatorType::FEnd ) );
	}

	inline CIterator CEnd()
	{
		return CIterator( this, m_Operator( 0, 0, this, OperatorType::FEnd ) );
	}

	inline CIterator CEnd() const
	{
		return CIterator( this, m_Operator( 0, 0, this, OperatorType::FEnd ) );
	}

	inline RIterator REnd()
	{
		return RIterator( this, m_Operator( 0, 0, this, OperatorType::REnd ) );
	}

	inline CRIterator REnd() const
	{
		return CRIterator( this, m_Operator( 0, 0, this, OperatorType::REnd ) );
	}

	inline CRIterator CREnd()
	{
		return CRIterator( this, m_Operator( 0, 0, this, OperatorType::REnd ) );
	}

	inline CRIterator CREnd() const
	{
		return CRIterator( this, m_Operator( 0, 0, this, OperatorType::REnd ) );
	}
	#pragma endregion

private:

	template < typename Iter >
	static void* Operator( void* a_ValueA, void* a_ValueB, const Enumerable< ValueType >* a_Enumerable, OperatorType a_OperatorType )
	{
		switch ( a_OperatorType )
		{
		case OperatorType::FBase:
		{
			return new Iter( reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA )->base() );
		}
		case OperatorType::RBase:
		{
			return new reverse_iterator< Iter >( *reinterpret_cast< Iter* >( a_ValueA ) );
		}
		case OperatorType::FClone:
		{
			return new Iter( *reinterpret_cast< Iter* >( a_ValueA ) );
		}
		case OperatorType::RClone:
		{
			if constexpr ( _Is_bidi_iter_v< Iter > || _Is_random_iter_v< Iter > )
			{
				return new reverse_iterator< Iter >( *reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA ) );
			}

			return new Iter( *reinterpret_cast< Iter* >( a_ValueA ) );
		}
		case OperatorType::FConvert:
		{
			if constexpr ( _Is_bidi_iter_v< Iter > || _Is_random_iter_v< Iter > )
			{
				/*auto* Old = reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA );
				Iter* New = new Iter( Old->base() );
				++*New;*/
				return new Iter( reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA )->base() );
			}

			Iter& Old = *reinterpret_cast< Iter* >( a_ValueA );
			Iter& New = *( new Iter( Old ) );

			if ( Old == *reinterpret_cast< Iter* >( a_Enumerable->m_End ) )
			{
				New = *reinterpret_cast< Iter* >( a_Enumerable->m_Begin );;
				return &New;
			}

			return &++New;
		}
		case OperatorType::RConvert:
		{
			if constexpr ( _Is_bidi_iter_v< Iter > || _Is_random_iter_v< Iter > )
			{
				return new reverse_iterator< Iter >( make_reverse_iterator( *reinterpret_cast< Iter* >( a_ValueA ) ) );
			}

			Iter& Old = *reinterpret_cast< Iter* >( a_ValueA );
			Iter& New = *( new Iter( Old ) );
			Iter& Beg = *reinterpret_cast< Iter* >( a_Enumerable->m_Begin );

			if ( Old == Beg )
			{
				New = *reinterpret_cast< Iter* >( a_Enumerable->m_End );
				return &New;
			}

			Iter Guard = Beg;
			Iter Marker = Guard++;
			for ( ; Guard != New; ++Guard, ++Marker );
			New = Marker;
			return &New;
		}
		case OperatorType::FDelete:
		{
			delete reinterpret_cast< Iter* >( a_ValueA );
			return nullptr;
		}
		case OperatorType::RDelete:
		{
			if constexpr ( _Is_bidi_iter_v< Iter > || _Is_random_iter_v< Iter > )
			{
				delete reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA );
				return nullptr;
			}

			delete reinterpret_cast< Iter* >( a_ValueA );
			return nullptr;
		}
		case OperatorType::FBegin:
		{
			return new Iter( *reinterpret_cast< Iter* >( a_Enumerable->m_Begin ) );
		}
		case OperatorType::RBegin:
		{
			Iter& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );

			if constexpr ( _Is_bidi_iter_v< Iter > || _Is_random_iter_v< Iter > )
			{
				return new reverse_iterator< Iter >( End );
			}

			Iter& Beg = *reinterpret_cast< Iter* >( a_Enumerable->m_Begin );

			if ( Beg == End )
			{
				return new Iter( Beg );
			}

			Iter Guard = Beg;
			Iter* Marker = new Iter( Guard++ );
			for ( ; Guard != End; ++*Marker, ++Guard );
			return Marker;
		}
		case OperatorType::FEnd:
		{
			return new Iter( *reinterpret_cast< Iter* >( a_Enumerable->m_End ) );
		}
		case OperatorType::REnd:
		{
			if constexpr ( _Is_bidi_iter_v< Iter > || _Is_random_iter_v< Iter > )
			{
				return new reverse_iterator< Iter >( *reinterpret_cast< Iter* >( a_Enumerable->m_Begin ) );
			}

			return new Iter( *reinterpret_cast< Iter* >( a_Enumerable->m_End ) );
		}
		case OperatorType::FShift:
		{
			int Places = reinterpret_cast< int >( a_ValueB );

			if constexpr ( _Is_bidi_iter_v< Iter > || _Is_random_iter_v< Iter > )
			{
				advance( *reinterpret_cast< Iter* >( a_ValueA ), Places );
				return a_ValueA;
			}

			if ( !Places )
			{
				return a_ValueA;
			}
			else if ( Places > 0 )
			{
				Iter& Subject = *reinterpret_cast< Iter* >( a_ValueA );
				for ( ; Places > 0; --Places ) ++Subject;
				return a_ValueA;
			}
			else if ( Places < 0 )
			{
				Iter Marker = *reinterpret_cast< Iter* >( a_Enumerable->m_Begin );
				Iter& Sub = *reinterpret_cast< Iter* >( a_ValueA );
				for ( ; Places < 0 && Marker != Sub; ++Places, ++Marker );
				_STL_ASSERT( Places == 0, "Cannot iterate before begin." );
				Iter Beg = *reinterpret_cast< Iter* >( a_Enumerable->m_Begin );
				for ( ; Marker != Sub; ++Beg, ++Marker );
				Sub = Beg;
				return a_ValueA;
			}
		}
		case OperatorType::RShift:
		{
			int Places = reinterpret_cast< int >( a_ValueB );

			if constexpr ( _Is_bidi_iter_v< Iter > || _Is_random_iter_v< Iter > )
			{
				advance( *reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA ), Places );
				return a_ValueA;
			}

			if ( !Places )
			{
				return a_ValueA;
			}
			else if ( Places > 0 )
			{
				Iter& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );
				Iter& Sub = *reinterpret_cast< Iter* >( a_ValueA );

				_STL_ASSERT( Sub != End, "Cannot iterate past end." );

				Iter& Begin = *reinterpret_cast< Iter* >( a_Enumerable->m_Begin );
				Iter Marker = Begin;
				for ( ; Places > 0 && Marker != Sub; --Places, ++Marker );

				if ( Places == 1 )
				{
					Sub = End;
					return a_ValueA;
				}

				_STL_ASSERT( Places == 0, "Cannot iterate past end." );
				Iter Tail = Begin;
				for ( ; Marker != Sub; ++Tail, ++Marker );
				Sub = Tail;
				return a_ValueA;
			}
			else if ( Places < 0 )
			{
				Iter& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );
				Iter& Sub = *reinterpret_cast< Iter* >( a_ValueA );

				if ( Sub == End )
				{
					Sub = *reinterpret_cast< Iter* >( a_Enumerable->m_Begin );

					if ( ++Places == 0 )
					{
						return a_ValueA;
					}
				}

				Iter Guard = Sub;
				++Guard;
				for ( ; Places < 0 && Guard != End; ++Places, ++Guard, ++Sub );
				_STL_ASSERT( Places == 0, "Cannot iterate before begin.");
				return a_ValueA;
			}
		}
		case OperatorType::FDereference:
		{
			return const_cast< ValueType* >( &**reinterpret_cast< Iter* >( a_ValueA ) );
		}
		case OperatorType::RDereference:
		{
			if constexpr ( _Is_bidi_iter_v< Iter > || _Is_random_iter_v< Iter > )
			{
				return const_cast< ValueType* >( &**reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA ) );
			}

			return const_cast< ValueType* >( &**reinterpret_cast< Iter* >( a_ValueA ) );
		}
		case OperatorType::FDistance:
		{
			if constexpr ( _Is_random_iter_v< Iter > )
			{
				return reinterpret_cast< void* >( 
					*reinterpret_cast< Iter* >( a_ValueA ) - 
					*reinterpret_cast< Iter* >( a_ValueB ) );
			}

			Iter Left   = *reinterpret_cast< Iter* >( a_ValueA );
			Iter& Right = *reinterpret_cast< Iter* >( a_ValueB );

			if ( Left == Right )
			{
				return 0;
			}
			else
			{
				int Length = 0;
				const Iter& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );
				for ( ; Left != Right && Left != End; ++Left, ++Length );

				if ( Left == Right )
				{
					return reinterpret_cast< void* >( -Length );
				}

				for ( Left = *reinterpret_cast< Iter* >( a_Enumerable->m_Begin ); Left != Right; ++Left, ++Length );

				return reinterpret_cast< void* >( a_Enumerable->m_Size - Length );
			}
		}
		case OperatorType::RDistance:
		{
			if constexpr ( _Is_random_iter_v< Iter > )
			{
				return reinterpret_cast< void* >( 
					*reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA ) - 
					*reinterpret_cast< reverse_iterator< Iter >* >( a_ValueB ) );
			}

			if constexpr ( _Is_bidi_iter_v< Iter > )
			{
				auto Left  = reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA )->base();
				auto Right = reinterpret_cast< reverse_iterator< Iter >* >( a_ValueB )->base();

				if ( Left == Right )
				{
					return 0;
				}

				int Length = 0;
				auto& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );
				for ( ; Left != Right && Left != End; ++Left, ++Length );

				if ( Left == Right )
				{
					return reinterpret_cast< void* >( Length );
				}

				for ( Left = *reinterpret_cast< Iter* >( a_Enumerable->m_Begin ); Left != Right; ++Left, ++Length );
				return reinterpret_cast< void* >( Length - a_Enumerable->m_Size );
			}

			Iter Left   = *reinterpret_cast< Iter* >( a_ValueA );
			Iter& Right = *reinterpret_cast< Iter* >( a_ValueB );

			if ( Left == Right )
			{
				return 0;
			}

			int Length = 0;
			Iter& Beg = *reinterpret_cast< Iter* >( a_Enumerable->m_Begin );
			Iter& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );
			
			if ( Right == End )
			{
				for ( ; Left != End; ++Left, ++Length );
				return reinterpret_cast< void* >( Length - a_Enumerable->m_Size - 1 );
			}

			if ( Left == End )
			{
				Left = Beg;
				++Length;
				for ( ; Left != Right; ++Left, ++Length );
				return reinterpret_cast< void* >( Length );
			}

			for ( ; Left != Right && Left != End; ++Left, ++Length );

			if ( Left == Right )
			{
				return reinterpret_cast< void* >( Length );
			}

			Left = Beg;
			++Length;
			for ( ; Left != Right; ++Left, ++Length );
			return reinterpret_cast< void* >( Length - a_Enumerable->m_Size - 1 );
		}
		case OperatorType::FEquality:
		{
			return reinterpret_cast< void* >( 
				*reinterpret_cast< Iter* >( a_ValueA ) == 
				*reinterpret_cast< Iter* >( a_ValueB ) );
		}
		case OperatorType::REquality:
		{
			if constexpr ( _Is_bidi_iter_v< Iter > || _Is_random_iter_v< Iter > )
			{
				return reinterpret_cast< void* >( 
					*reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA ) == 
					*reinterpret_cast< reverse_iterator< Iter >* >( a_ValueB ) );
			}

			return reinterpret_cast< void* >(
				*reinterpret_cast< Iter* >( a_ValueA ) ==
				*reinterpret_cast< Iter* >( a_ValueB ) );
		}
		case OperatorType::FGreaterEqual:
		{
			if constexpr ( _Is_random_iter_v< Iter > )
			{
				Iter& Left = *reinterpret_cast< Iter* >( a_ValueA );
				Iter& Right = *reinterpret_cast< Iter* >( a_ValueB );

				if ( Left == Right )
				{
					return reinterpret_cast< void* >( 1 );
				}

				if ( Left > Right )
				{
					return reinterpret_cast< void* >( 2 );
				}
			}

			Iter Left   = *reinterpret_cast< Iter* >( a_ValueA );
			Iter& Right = *reinterpret_cast< Iter* >( a_ValueB );
			Iter& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );

			if ( Left == Right )
			{
				return reinterpret_cast< void* >( 1 );
			}

			if ( Right == End )
			{
				return nullptr;
			}

			for ( ; Left != Right && Left != End; ++Left );

			if ( Left == End )
			{
				return reinterpret_cast< void* >( 2 );
			}

			return nullptr;
		}
		case OperatorType::RGreaterEqual:
		{
			if constexpr ( _Is_random_iter_v< Iter > )
			{
				auto& Left  = *reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA );
				auto& Right = *reinterpret_cast< reverse_iterator< Iter >* >( a_ValueB );

				if ( Left == Right )
				{
					return reinterpret_cast< void* >( 1 );
				}

				if ( Left > Right )
				{
					return reinterpret_cast< void* >( 2 );
				}
			}

			if constexpr ( _Is_bidi_iter_v< Iter > )
			{
				auto Left  = reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA )->base();
				auto Right = reinterpret_cast< reverse_iterator< Iter >* >( a_ValueB )->base();

				if ( Left == Right )
				{
					return reinterpret_cast< void* >( 1 );
				}

				Iter& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );

				if ( Right == End )
				{
					return reinterpret_cast< void* >( 2 );
				}

				for ( ; Left != Right && Left != End; ++Left );

				if ( Left == End )
				{
					return nullptr;
				}

				return reinterpret_cast< void* >( 2 );
			}
			
			Iter Left = *reinterpret_cast< Iter* >( a_ValueA );
			Iter& Right = *reinterpret_cast< Iter* >( a_ValueB );

			if ( Left == Right )
			{
				return reinterpret_cast< void* >( 1 );
			}

			Iter& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );

			if ( Left == End )
			{
				return reinterpret_cast< void* >( 2 );
			}

			if ( Right == End )
			{
				return nullptr;
			}

			for ( ; Left != Right & Left != End; ++Left );

			if ( Left == Right )
			{
				return reinterpret_cast< void* >( 2 );
			}

			return nullptr;
		}
		case OperatorType::FLesserEqual:
		{
			if constexpr ( _Is_random_iter_v< Iter > )
			{
				Iter& Left = *reinterpret_cast< Iter* >( a_ValueA );
				Iter& Right = *reinterpret_cast< Iter* >( a_ValueB );

				if ( Left == Right )
				{
					return reinterpret_cast< void* >( 1 );
				}

				if ( Left < Right )
				{
					return reinterpret_cast< void* >( 2 );
				}
			}

			Iter Left = *reinterpret_cast< Iter* >( a_ValueA );
			Iter& Right = *reinterpret_cast< Iter* >( a_ValueB );
			Iter& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );

			if ( Left == Right )
			{
				return reinterpret_cast< void* >( 1 );
			}

			if ( Right == End )
			{
				return reinterpret_cast< void* >( 2 );
			}

			for ( ; Left != Right && Left != End; ++Left );

			if ( Left == End )
			{
				return nullptr;
			}

			return reinterpret_cast< void* >( 2 );
		}
		case OperatorType::RLesserEqual:
		{
			if constexpr ( _Is_random_iter_v< Iter > )
			{
				auto& Left = *reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA );
				auto& Right = *reinterpret_cast< reverse_iterator< Iter >* >( a_ValueB );

				if ( Left == Right )
				{
					return reinterpret_cast< void* >( 1 );
				}

				if ( Left < Right )
				{
					return reinterpret_cast< void* >( 2 );
				}
			}

			if constexpr ( _Is_bidi_iter_v< Iter > )
			{
				Iter Left = reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA )->base();
				Iter Right = reinterpret_cast< reverse_iterator< Iter >* >( a_ValueB )->base();

				if ( Left == Right )
				{
					return reinterpret_cast< void* >( 1 );
				}

				Iter& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );

				if ( Right == End )
				{
					return nullptr;
				}

				for ( ; Left != Right && Left != End; ++Left );

				if ( Left == End )
				{
					return reinterpret_cast< void* >( 2 );
				}

				return nullptr;
			}

			Iter Left = *reinterpret_cast< Iter* >( a_ValueA );
			Iter& Right = *reinterpret_cast< Iter* >( a_ValueB );

			if ( Left == Right )
			{
				return reinterpret_cast< void* >( 1 );
			}

			Iter& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );

			if ( Left == End )
			{
				return nullptr;
			}

			if ( Right == End )
			{
				return reinterpret_cast< void* >( 2 );
			}

			for ( ; Left != Right & Left != End; ++Left );

			if ( Left == Right )
			{
				return nullptr;
			}

			return reinterpret_cast< void* >( 2 );
		}
		case OperatorType::FAssign:
		{
			*reinterpret_cast< Iter* >( a_ValueA ) = *reinterpret_cast< Iter* >( a_ValueB );
			return a_ValueA;
		}
		case OperatorType::RAssign:
		{
			if constexpr ( _Is_bidi_iter_v< Iter > || _Is_random_iter_v< Iter > )
			{
				*reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA ) = *reinterpret_cast< reverse_iterator< Iter >* >( a_ValueB );
				return a_ValueA;
			}

			*reinterpret_cast< Iter* >( a_ValueA ) = *reinterpret_cast< Iter* >( a_ValueB );
			return a_ValueA;
		}
		}
	}

	void*    m_Begin;
	void*    m_End;
	size_t   m_Size;
	void* ( *m_Operator )( void*, void*, const Enumerable< ValueType >*, OperatorType );
};