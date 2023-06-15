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
#include "Span.hpp"

//template < typename T >
//using CollectionIterator = T*;
//
//// This needs to be completed properly.
//template < typename T, typename _Allocator = std::allocator< T > >
//class Collection : public IContiguousCollection< T >
//{
//public:
//
//	using BaseType = IContiguousCollection< T >;
//	using AllocatorType = _Allocator;
//	using IteratorType = CollectionIterator< T >;
//	using CIteratorType = CollectionIterator< const T >;
//	using RIteratorType = RandomAccessIterator< std::reverse_iterator< CollectionIterator< T > > >;
//	using CRIteratorType = RandomAccessIterator< std::reverse_iterator< CollectionIterator< const T > > >;
//	using EnumeratorType = typename BaseType::EnumeratorType;
//	using CEnumeratorType = typename BaseType::CEnumeratorType;
//	using EnumerableType = typename BaseType::EnumerableType;
//	using CEnumerableType = typename BaseType::CEnumerableType;
//
//	Collection();
//	Collection( size_t a_Capacity );
//	Collection( const Collection& a_Collection );
//	Collection( Collection&& a_Collection );
//
//private:
//
//	template < typename U >
//	static constexpr bool is_compatible_type_v = std::is_const_v< T > && std::is_same_v< std::decay_t< T >, U >;
//
//public:
//
//	template < typename U, typename = std::enable_if_t< is_compatible_type_v< U > > >
//	Collection( const Collection< U, _Allocator >& a_Collection );
//
//	template < typename U, typename = std::enable_if_t< is_compatible_type_v< U > > >
//	Collection( Collection< U, _Allocator >&& a_Collection );
//
//	~Collection();
//	void Add( const T& a_Value );
//	void Remove( size_t a_Index );
//	size_t Capacity() const { return m_Capacity; }
//	size_t Size() const { return m_Size; }
//	constexpr size_t MaxSize() const { return ( size_t )-1; }
//	const T* Data() const { return m_Data; }
//	T* Data() { return m_Data; }
//	const T& At( size_t a_Index ) const { return m_Data[ a_Index ]; }
//	T& At( size_t a_Index ) { return m_Data[ a_Index ]; }
//
//	IteratorType Begin() { return IteratorType(); }
//	CIteratorType Begin() const { return CIteratorType(); }
//	CIteratorType CBegin() const { return CIteratorType(); }
//	RIteratorType RBegin() { return RIteratorType(); }
//	CRIteratorType RBegin() const { return CRIteratorType(); }
//	CRIteratorType CRBegin() const { return CRIteratorType(); }
//
//	IteratorType End() { return IteratorType(); }
//	CIteratorType End() const { return CIteratorType(); }
//	CIteratorType CEnd() const { return CIteratorType(); }
//	RIteratorType REnd() { return RIteratorType(); }
//	CRIteratorType REnd() const { return CRIteratorType(); }
//	CRIteratorType CREnd() const { return CRIteratorType(); }
//
//protected:
//
//	EnumerableType IToEnumerable() { return EnumerableType( Size(), Begin(), End() ); }
//	size_t ISize() const { return Size(); }
//	size_t IMaxSize() const { return MaxSize(); }
//	EnumeratorType IBegin() { return Begin(); }
//	EnumeratorType IRBegin() { return RBegin(); }
//	EnumeratorType IEnd() { return End(); }
//	EnumeratorType IREnd() { return REnd(); }
//	T* IData() { return m_Data; }
//	T& IAt( size_t a_Index ) { return m_Data[ a_Index ]; }
//
//private:
//
//	T* m_Data;
//	size_t m_Capacity;
//	size_t m_Size;
//	AllocatorType m_Allocator;
//};
//
//template < typename T, typename _Allocator >
//Collection< T, _Allocator >::Collection()
//	: m_Data( nullptr )
//	, m_Size( 0 )
//	, m_Capacity( 0 )
//{}
//
//template < typename T, typename _Allocator >
//Collection< T, _Allocator >::Collection( size_t a_Capacity )
//	: m_Data( new T[ a_Capacity ] )
//	, m_Size( 0 )
//	, m_Capacity( a_Capacity )
//{}
//
//template < typename T, typename _Allocator  >
//Collection< T, _Allocator >::Collection( const Collection& a_Collection )
//{
//
//}
//
//template < typename T, typename _Allocator  >
//Collection< T, _Allocator >::Collection( Collection&& a_Collection )
//{
//
//}
//
//template < typename T, typename _Allocator  >
//template < typename U, typename >
//Collection< T, _Allocator >::Collection( const Collection< U, _Allocator >& a_Collection )
//{
//
//}
//
//template < typename T, typename _Allocator >
//template < typename U, typename >
//Collection< T, _Allocator >::Collection( Collection< U, _Allocator >&& a_Collection )
//{
//
//}
//
//template < typename T, typename _Allocator  >
//Collection< T, _Allocator >::~Collection()
//{
//
//}
//
//template < typename T, typename _Allocator  >
//void Collection< T, _Allocator >::Add( const T& a_Value )
//{
//
//}
//
//template < typename T, typename _Allocator  >
//void Collection< T, _Allocator >::Remove( size_t a_Index )
//{
//
//}

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
	//Collection< int > coll;

	//Array< int, 5 > a = { 1, 2, 3, 4, 5 };

	//ArrayCIterator< int, 5 > ait = a.CBegin();

	//ICollection< int >& ac = a;

	//struct State
	//{
	//	int Index = 0;
	//	int Max = 0;
	//};

	//State StateObject = { -1, 4 };
	//std::any StateAny = std::make_any< State >(StateObject);
	//
	//DeferredCollection< int > dc( ac.Begin(), ac.Size(), std::move(StateObject),
	//	[]( IForwardIterator< int >& a_Iter, std::any& a_State ) -> int*
	//	{
	//		State& StateObj = ( State& )a_State;

	//		if ( StateObj.Index == StateObj.Max )
	//		{
	//			return nullptr;
	//		}

	//		while ( StateObj.Index++ < StateObj.Max )
	//		{
	//			++a_Iter;
	//			int* Val = &*a_Iter;

	//			if ( *Val > 3 )
	//			{
	//				return Val;
	//			}
	//		}

	//		return nullptr;
	//	} );

	//auto beg = dc.Begin();

	struct SomeStruct
	{
		SomeStruct()
		{
			std::cout << "creating";
		}
	};


	
	std::vector< int > ints = { 1, 2, 3, 4 };
	Span< int > span( ints.data(), ints.size() );
	auto val = span.At( 2 );
	
	for ( auto& vals : span )
	{
		std::cout << vals << std::endl;
	}
}