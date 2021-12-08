#pragma once
#include <type_traits>
#include <iterator>

using namespace std;

template < typename T >
class Enumerable
{
	template < typename U >
	using EnableIfCorrectIterator = std::enable_if_t< _Is_iterator_v< U >&& is_same_v< typename iterator_traits< U >::value_type, T >, void >;

public:

	class Iterator
	{
	public:

		using iterator_category = random_access_iterator_tag;
		using difference_type   = ptrdiff_t;
		using value_type        = T;
		using pointer           = value_type*;
		using reference         = value_type&;

	private:

		Iterator( Enumerable< T >& a_Enumerable, void* a_Iterator )
			: m_Enumerable( a_Enumerable )
			, m_Iterator( a_Iterator )
		{ }

	public:

		~Iterator()
		{
			m_Enumerable.m_Delete( m_Iterator );
		}

		Iterator( const Iterator& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
		{
			void* New;
			m_Enumerable.m_Create( a_Iterator.m_Iterator, New );
			m_Iterator = New;
		}

		Iterator( const Iterator&& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
		{
			void* New;
			m_Enumerable.m_Create( a_Iterator.m_Iterator, New );
			m_Iterator = New;
		}

		Iterator& operator++()
		{
			m_Enumerable.m_Increment( *this );
			return *this;
		}

		Iterator operator++(int)
		{
			void* New;
			m_Enumerable.m_Create( m_Iterator, New );
			Iterator Temp( m_Enumerable, New );
			m_Enumerable.m_Increment( *this );
			return Temp;
		}

		Iterator& operator--()
		{
			m_Enumerable.m_Decrement( *this );
			return *this;
		}

		Iterator operator--(int)
		{
			void* New;
			m_Enumerable.m_Create( m_Iterator, New );
			Iterator Temp( m_Enumerable, New );
			m_Enumerable.m_Decrement( *this );
			return Temp;
		}

		void operator+=( int a_Places )
		{
			m_Enumerable.m_Shift( *this, a_Places );
		}

		void operator-=( int a_Places )
		{
			m_Enumerable.m_Shift( *this, a_Places );
		}

		Iterator operator+( int a_Places )
		{
			void* New;
			m_Enumerable.m_Create( m_Iterator, New );
			Iterator Temp( m_Enumerable, New );
			m_Enumerable.m_Shift( Temp, a_Places );
			return Temp;
		}

		Iterator operator-( int a_Places )
		{
			void* New;
			m_Enumerable.m_Create( m_Iterator, New );
			Iterator Temp( m_Enumerable, New );
			m_Enumerable.m_Shift( Temp, -a_Places );
			return Temp;
		}

		int operator-( Iterator a_Iterator )
		{
			int num0 = m_Enumerable.m_Dereference( *this );
			int num1 = m_Enumerable.m_Dereference( a_Iterator );
			return m_Enumerable.m_Difference( m_Iterator, a_Iterator.m_Iterator );
		}
		
		T& operator*()
		{
			return m_Enumerable.m_Dereference( *this );
		}

		T* operator->()
		{
			return m_Enumerable.m_Dereference( *this );
		}

		bool operator==( const Iterator& a_Iterator ) const
		{
			return m_Enumerable.m_IteratorEqual( m_Iterator, a_Iterator.m_Iterator );
		}

		bool operator!=( const Iterator& a_Iterator ) const
		{
			return !m_Enumerable.m_IteratorEqual( m_Iterator, a_Iterator.m_Iterator );
		}

	private:

		friend class Enumerable< T >;

		Enumerable< T >& m_Enumerable;
		void*            m_Iterator;
	};

	template < typename U, typename = EnableIfCorrectIterator< U > >
	Enumerable( U a_Begin, U a_End )
	{
		m_Begin = new U( a_Begin );
		m_End   = new U( a_End );
		m_Size  = distance( a_Begin, a_End );
		m_Create = Create< U >;
		m_Delete = Delete< U >;
		m_Increment = Increment< U >;
		m_Decrement = Decrement< U >;
		m_Shift = Shift< U >;
		m_Dereference = Dereference< U >;
		m_Difference = Difference< U >;
		m_IteratorEqual = IteratorEqual< U >;
		m_IteratorLesser;
		m_IteratorGreater;
	}

	~Enumerable()
	{
		m_Delete( m_Begin );
		m_Delete( m_End );
	}

	inline Iterator Begin()
	{
		return Iterator( *this, m_Begin );
	}

	inline Iterator End()
	{
		return Iterator( *this, m_End );
	}

	inline size_t Size() const
	{
		return m_Size;
	}

	template < typename U >
	static void Increment( Iterator& a_Iterator )
	{
		++*reinterpret_cast< U* >( a_Iterator.m_Iterator );
	}

	template < typename U >
	static void Decrement( Iterator& a_Iterator )
	{
		if constexpr ( _Is_bidi_iter_v< U > )
		{
			--*reinterpret_cast< U* >( a_Iterator.m_Iterator );
		}
		else
		{
			auto Distance = distance( *reinterpret_cast< U* >( a_Iterator.m_Enumerable.m_Begin ), *reinterpret_cast< U* >( a_Iterator.m_Iterator ) );
			*reinterpret_cast< U* >( a_Iterator.m_Iterator ) = *reinterpret_cast< U* >( a_Iterator.m_Enumerable.m_Begin );
			while ( Distance-- > 1 ) ++*reinterpret_cast< U* >( a_Iterator.m_Iterator );
		}
	}

	template < typename U >
	static void Shift( Iterator& a_Iterator, int a_Places )
	{
		if ( !a_Places )
		{
			return;
		}

		if constexpr ( _Is_random_iter_v< U > )
		{
			*reinterpret_cast< U* >( a_Iterator.m_Iterator ) += a_Places;
		}
		else
		{
			if ( a_Places < 0 )
			{
				auto Distance = distance( *reinterpret_cast< U* >( a_Iterator.m_Enumerable.m_Begin ), *reinterpret_cast< U* >( a_Iterator.m_Iterator ) );
				*reinterpret_cast< U* >( a_Iterator.m_Iterator ) = *reinterpret_cast< U* >( a_Iterator.m_Enumerable.m_Begin );
				while ( Distance-- > -a_Places ) ++*reinterpret_cast< U* >( a_Iterator.m_Iterator );
			}
			else
			{
				for ( ; a_Places > 0; --a_Places ) ++(*reinterpret_cast< U* >( a_Iterator.m_Iterator ));
			}
		}
	}

	template < typename U >
	static void Create( void* a_Iterator, void*& o_NewIterator )
	{
		o_NewIterator = new U( *reinterpret_cast< U* >( a_Iterator ) );
	}

	template < typename U >
	static void Delete( void* a_Iterator )
	{
		delete reinterpret_cast< U* >( a_Iterator );
	}

	template < typename U >
	static T& Dereference( Iterator& a_Iterator )
	{
		return **reinterpret_cast< U* >( a_Iterator.m_Iterator );
	}

	template < typename U >
	static int Difference( void* a_Left, void* a_Right )
	{
		return distance( *reinterpret_cast< U* >( a_Left ), *reinterpret_cast< U* >( a_Right ) );
	}

	template < typename U >
	static bool IteratorEqual( const void* a_IteratorA, const void* a_IteratorB )
	{
		return *reinterpret_cast< const U* >( a_IteratorA ) == *reinterpret_cast< const U* >( a_IteratorB );
	}

	//template < typename U >
	//static bool IteratorGreater( const void*)

	void*  m_Begin;
	void*  m_End;
	size_t m_Size;

	void( *m_Create    )( void*, void*& );
	void( *m_Delete    )( void* );
	void( *m_Increment )( Iterator& );
	void( *m_Decrement )( Iterator& );
	void( *m_Shift     )( Iterator&, int );

	T&  ( *m_Dereference )( Iterator& );
	int ( *m_Difference  )( void* a_Left, void* a_Right );

	bool( *m_IteratorEqual   )( const void*, const void* );
	bool( *m_IteratorLesser  )( const void*, const void* );
	bool( *m_IteratorGreater )( const void*, const void* );

};