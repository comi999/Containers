#include <iostream>
#include <vector>
#include <memory>

// All containers should inherit from IContainer<T>
// IContainer<T> should implement a way to get elements through virtualization
// All iterators should inherit from IIterator<T>
// Enumerator should be a special type of Iterator that uses type erasure so that all Iterators can be converted into an Enumerator

// IContainer should only be an interface, but it needs to somehow also be able to return primitive iterators.
// IContainer::Begin should return an Enumerator which is type erased. The implementation of constructing the Enumerator should be in the implementing container class.

// I need to enforce template types for containers to not be references.
// Collection<T> needs to be properly fleshed out.
// Need to make two variants of each type of iterator adapter for const and non-const so that we respect constness properly.
// Need to rename the adapters to be something better.

#include "Array.hpp"

//template < typename T >
//class DeferredCollection;
//
//template < typename T >
//class DeferredCollectionIterator : public IForwardIterator< T >
//{
//public:
//
//	using BaseType = IForwardIterator< T >;
//	using TraitsType = std::iterator_traits< DeferredCollectionIterator >;
//	using CategoryType = typename TraitsType::iterator_category;
//	using DifferenceType = typename TraitsType::difference_type;
//	using ValueType = typename TraitsType::value_type;
//	using ReferenceType = typename TraitsType::reference;
//	using PointerType = typename TraitsType::pointer;
//	using EnumeratorType = typename BaseType::EnumeratorType;
//	using CEnumeratorType = typename BaseType::CEnumeratorType;
//
//	DeferredCollectionIterator() = default;
//	DeferredCollectionIterator( const DeferredCollectionIterator& ) = default;
//	DeferredCollectionIterator( DeferredCollectionIterator&& ) = default;
//	DeferredCollectionIterator& operator=( const DeferredCollectionIterator& ) = default;
//	DeferredCollectionIterator& operator=( DeferredCollectionIterator&& ) = default;
//
//	DeferredCollectionIterator( DeferredCollection< T >* a_DeferredCollection, size_t a_Index )
//		: m_Index( -1 )
//		, m_Value( nullptr )
//		, m_DeferredCollection( a_DeferredCollection )
//	
//	{
//		for ( int i = -1; i < ( int )a_Index; ++i )
//			++*this;
//	}
//
//	ReferenceType operator*() const { return *m_Value; }
//	PointerType operator->() const { return m_Value; }
//	DeferredCollectionIterator& operator++() { IIncrement(); return *this; }
//	DeferredCollectionIterator operator++( int ) { DeferredCollectionIterator Temp = *this; ++*this; return Temp; }
//	bool operator==( const DeferredCollectionIterator& a_Iterator ) const { return false; }
//	bool operator!=( const DeferredCollectionIterator& a_Iterator ) const { return false; }
//
//protected:
//
//	EnumeratorType IToEnumerator() { return *this; }
//	ReferenceType IDereference() const { return *m_Value; }
//	PointerType IArrow() const { return m_Value; }
//	void IIncrement();
//
//private:
//
//	int64_t m_Index;
//	T* m_Value;
//	DeferredCollection< T >* m_DeferredCollection;
//};
//
//namespace std
//{
//	template < typename T >
//	struct iterator_traits< DeferredCollectionIterator< T > >
//	{
//		using iterator_category = std::forward_iterator_tag;
//		using difference_type = std::ptrdiff_t;
//		using value_type = std::decay_t< T >;
//		using reference = T&;
//		using pointer = T*;
//	};
//}
//
//template < typename T >
//class DeferredCollection// : public ICollection< T >
//{
//private:
//public:
//
//	friend class DeferredCollectionIterator< T >;
//
//	DeferredCollection( Enumerator< T >&& a_Begin, size_t a_Size, std::any&& a_State, const Invoker< T*, IForwardIterator< T >&, std::any& >& a_Processor )
//		: m_Begin( std::move( a_Begin ) )
//		, m_Size( a_Size )
//		, m_State( std::move( a_State ) )
//		, m_Processor( a_Processor )
//		, m_Index( -1 )
//	{}
//
//	void Progress( int a_Count )
//	{
//		// Do something that finds the next element.
//		if ( a_Count == 0 )
//		{
//			return;
//		}
//
//		m_Cache.reserve( m_Cache.size() + a_Count );
//
//		for ( ; a_Count > 0; --a_Count )
//		{
//			T* v = m_Processor.Invoke( m_Begin.AsIterator(), m_State );
//
//			if ( v == nullptr )
//			{
//				return;
//			}
//
//			m_Cache.push_back( v );
//			++m_Index;
//		}
//	}
//
//	T* Get( size_t a_Index )
//	{
//		Progress( a_Index - m_Index );
//
//		if ( !( a_Index < m_Cache.size() ) )
//		{
//			return nullptr;
//		}
//
//		return m_Cache[ a_Index ];
//	}
//
//	std::vector< T* > ToVector()
//	{
//		DeferredCollectionIterator< T > iter = Begin();
//		std::vector< T* > Result;
//
//		for ( ;; ++iter )
//		{
//			T* val = iter.operator->();
//
//			if ( val == nullptr )
//			{
//				return Result;
//			}
//
//			Result.push_back( val );
//		}
//	}
//
//	DeferredCollectionIterator< T > Begin()
//	{
//		return DeferredCollectionIterator< T >( this, 0 );
//	}
//
//	Enumerator< T > m_Begin;
//	size_t m_Size;
//	std::any m_State;
//	std::vector< T* > m_Cache;
//	Invoker< T*, IForwardIterator< T >&, std::any& > m_Processor;
//	int64_t m_Index;
//};
//
//template < typename T >
//void DeferredCollectionIterator< T >::IIncrement()
//{
//	m_Value = m_DeferredCollection->Get( ++m_Index );
//}

int main()
{
	Array< int, 5 > a = { 1, 3, 3, 3, 5 };

	auto df = a.FindAll( 3 );
	
	for ( auto& val : df )
	{
		std::cout << val << std::endl;
	}
	
}