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

template < typename T >
void Destructor( void* a_Object );

using DestructorFunction = void( * )( void* );

constexpr uintptr_t VirtualSignature = 0xFEEDDEAFDEADBEEF;

template < typename T >
struct VirtualObject
{
	template < typename... Args >
	VirtualObject( Args&&... a_Args )
		: Value( std::forward< Args >( a_Args )... )
	{
		Function = Destructor< T >;
	}

	const uintptr_t Signature = VirtualSignature;
	DestructorFunction Function;
	T Value;
};

template < typename T >
void Destructor( void* a_Object )
{
	delete ( VirtualObject< T >* )a_Object;
}

template < typename Return, typename... Args >
class invoker
{
public:

	using InvocationFunc = Return( * )( void*, Args&&... );

	template < auto _Function >
	static Return Invocation( void* a_Object, Args&&... a_Args )
	{
		return ( reinterpret_cast< FunctionTraits::GetObject< decltype( _Function ) >* >( a_Object )->*_Function )( std::forward< Args >( a_Args )... );
	}

	template < typename T >
	invoker( T&& a_Lambda )
	{
		m_Object = new VirtualObject< T >( std::forward< T >( a_Lambda ) );
		m_Function = reinterpret_cast< void* >( Invocation< &std::remove_reference_t<T>::operator() > );
	}

	~invoker()
	{
		Unbind();
	}

	void Unbind()
	{
		if ( m_Object )
		{
			( *reinterpret_cast< DestructorFunction* >( ( uint8_t* )m_Object + sizeof( uintptr_t ) ) )( m_Object );
		}

		m_Object = nullptr;
		m_Function = nullptr;
	}

	Return Invoke( Args... a_Args )
	{
		if ( *( uintptr_t* )m_Object == VirtualSignature )
		{
			return ( ( InvocationFunc )m_Function )( ( uint8_t* )m_Object + sizeof( uintptr_t ) + sizeof( DestructorFunction ), std::forward< Args >( a_Args )... );
		}
		else
		{

		}
	}

	void* m_Function;
	void* m_Object;
};

struct Movable
{
	Movable() = default;
	Movable( const Movable& ) = default;
	Movable( Movable&& ) = default;

	std::any Value;
};

struct CallableObject
{
	CallableObject( std::initializer_list< int > a_List )
		: data( a_List )
	{}

	CallableObject() = default;
	CallableObject( const CallableObject& ) = default;
	CallableObject( CallableObject&& ) = default;
	~CallableObject() { std::cout << "destroying"; }
	int operator()()
	{
		return data[ 1 ];
	}

	std::vector< int > data;
};


int main()
{

	std::vector< int > vec = { 1,2,3,4,5,6,7 };
	Movable m;
	m.Value = std::make_any< std::vector<int> >( std::move(vec) );

	invoker<int> inv = [num = std::move(m)]()
	{
		return std::any_cast<const std::vector<int>&>(num.Value).at(1);
	};

	int val = inv.Invoke();
	inv.Unbind();


	//Array< int, 5 > a = { 1, 3, 3, 3, 5 };

	//auto df = a.FindAll( 3 );
	//
	//for ( auto Begin = df.Begin(), End = df.End(); Begin != End; ++Begin )
	//{
	//	std::cout << *Begin << std::endl;
	//}


	//for ( auto& val : df )
	//{
	//	std::cout << val << std::endl;
	//}
}