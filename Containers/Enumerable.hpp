#pragma once
#include <type_traits>
#include <iterator>

using namespace std;

template < typename T >
class Enumerable
{
	template < typename Iter >
	using EnableIfCompatible = std::enable_if_t< _Is_iterator_v< Iter >&& is_same_v< typename iterator_traits< Iter >::value_type, T >, void >;

	enum class OperatorType : char
	{
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

	class Iterator
	{
	public:

		using iterator_category = random_access_iterator_tag;
		using difference_type   = ptrdiff_t;
		using value_type        = T;
		using pointer           = value_type*;
		using reference         = value_type&;

	private:

		Iterator( Enumerable< T >* a_Enumerable, void* a_Iterator )
			: m_Enumerable( a_Enumerable )
			, m_Iterator( a_Iterator )
		{ }

	public:

		Iterator( Iterator& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_Iterator.m_Iterator, 0, 0, OperatorType::FClone ) )
		{ }

		Iterator( Iterator&& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_Iterator.m_Iterator, 0, 0, OperatorType::FClone ) )
		{ }

		Iterator( RIterator& a_RIterator )
			: m_Enumerable( a_RIterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_RIterator.m_Iterator, 0, m_Enumerable, OperatorType::FConvert ) )
		{ }

		Iterator( RIterator&& a_RIterator )
			: m_Enumerable( a_RIterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_RIterator.m_Iterator, 0, m_Enumerable, OperatorType::FConvert ) )
		{ }
		
		~Iterator()
		{
			m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FDelete );
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

		inline T& operator*()
		{
			return *reinterpret_cast< T* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FDereference ) );
		}

		inline const T& operator*() const
		{
			return *reinterpret_cast< T* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FDereference ) );
		}

		inline T* operator->()
		{
			return *reinterpret_cast< T* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FDereference ) );
		}

		inline const T* operator->() const
		{
			return *reinterpret_cast< T* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::FDereference ) );
		}

		inline bool operator==( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, 0, OperatorType::FEquality );
		}

		inline bool operator!=( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return !m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, 0, OperatorType::FEquality );
		}

		inline bool operator>( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::FGreaterEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator>=( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::FGreaterEqual );
		}

		inline bool operator<( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::FLesserEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator<=( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::FLesserEqual );
		}

	private:

		friend class Enumerable< T >;
		friend class Enumerable< T >::RIterator;

		Enumerable< T >* m_Enumerable;
		void*            m_Iterator;

	};

	class RIterator
	{
	public:

		using iterator_category = random_access_iterator_tag;
		using difference_type = ptrdiff_t;
		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;

	private:

		RIterator( Enumerable< T >* a_Enumerable, void* a_Iterator )
			: m_Enumerable( a_Enumerable )
			, m_Iterator( a_Iterator )
		{ }

	public:

		RIterator( Iterator& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_Iterator.m_Iterator, 0, m_Enumerable, OperatorType::RConvert ) )
		{ }

		RIterator( Iterator&& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_Iterator.m_Iterator, 0, m_Enumerable, OperatorType::RConvert ) )
		{ }

		RIterator( RIterator& a_RIterator )
			: m_Enumerable( a_RIterator.m_Enumerable )
			, m_Iterator( m_Operator( a_RIterator.m_Iterator, 0, 0, OperatorType::RClone ) )
		{ }

		RIterator( RIterator&& a_RIterator )
			: m_Enumerable( a_RIterator.m_Enumerable )
			, m_Iterator( m_Operator( a_RIterator.m_Iterator, 0, 0, OperatorType::RClone ) )
		{ }

		~RIterator()
		{
			m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RDelete );
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

		inline T& operator*()
		{
			return *reinterpret_cast< T* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RDereference ) );
		}

		inline const T& operator*() const
		{
			return *reinterpret_cast< T* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RDereference ) );
		}

		inline T* operator->()
		{
			return *reinterpret_cast< T* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RDereference ) );
		}

		inline const T* operator->() const
		{
			return *reinterpret_cast< T* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::RDereference ) );
		}

		inline bool operator==( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, 0, OperatorType::REquality );
		}

		inline bool operator!=( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return !m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, 0, OperatorType::REquality );
		}

		inline bool operator>( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, m_Enumerable, OperatorType::RGreaterEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator>=( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, m_Enumerable, OperatorType::RGreaterEqual );
		}

		inline bool operator<( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, m_Enumerable, OperatorType::RLesserEqual )
				== reinterpret_cast< void* >( 2 );
		}

		inline bool operator<=( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_RIterator.m_Iterator, m_Enumerable, OperatorType::RLesserEqual );
		}

	private:

		friend class Enumerable< T >;
		friend class Enumerable< T >::Iterator;

		Enumerable< T >* m_Enumerable;
		void* m_Iterator;
	};

	class CIterator : private Iterator
	{
	public:

		using iterator_category = random_access_iterator_tag;
		using difference_type = ptrdiff_t;
		using value_type = const T;
		using pointer = value_type*;
		using reference = value_type&;

	private:

		CIterator( Enumerable< T >* a_Enumerable, void* a_Iterator )
			: Iterator( a_Enumerable, a_Iterator )
		{ }

	public:

		CIterator( const Iterator& a_Iterator )
			: Iterator( const_cast< Iterator& >( a_Iterator ) )
		{ }

		CIterator( const Iterator&& a_Iterator )
			: Iterator( const_cast< Iterator&& >( a_Iterator ) )
		{ }

		CIterator( const CIterator& a_CIterator )
			: Iterator( const_cast< CIterator& >( a_CIterator ) )
		{ }

		CIterator( const CIterator&& a_CIterator )
			: Iterator( const_cast< CIterator&& >( a_CIterator ) )
		{ }

		inline CIterator& operator++()
		{
			return reinterpret_cast< CIterator& >( Iterator::operator++() );
		}

		inline CIterator operator++( int )
		{
			return Iterator::operator++( 0 );
		}

		inline CIterator& operator--()
		{
			return reinterpret_cast< CIterator& >( Iterator::operator--() );
		}

		inline CIterator operator--( int )
		{
			return Iterator::operator--( 0 );
		}

		inline CIterator& operator+=( int a_Places )
		{
			return reinterpret_cast< CIterator& >( Iterator::operator+=( a_Places ) );
		}

		inline CIterator& operator-=( int a_Places )
		{
			return reinterpret_cast< CIterator& >( Iterator::operator-=( a_Places ) );
		}

		inline CIterator operator+( int a_Places ) const
		{
			return Iterator::operator+( a_Places );
		}

		inline CIterator operator-( int a_Places ) const
		{
			return Iterator::operator-( a_Places );
		}

		inline int operator-( Iterator a_Iterator ) const
		{
			return Iterator::operator-( a_Iterator );
		}

		inline const T& operator*() const
		{
			return Iterator::operator*();
		}

		inline const T* operator->() const
		{
			return Iterator::operator->();
		}

		inline bool operator==( const RIterator& a_RIterator ) const
		{
			return Iterator::operator==( a_RIterator );
		}

		inline bool operator!=( const RIterator& a_RIterator ) const
		{
			return Iterator::operator!=( a_RIterator );
		}

		inline bool operator>( const RIterator& a_RIterator ) const
		{
			return Iterator::operator>( a_RIterator );
		}

		inline bool operator>=( const RIterator& a_RIterator ) const
		{
			return Iterator::operator>=( a_RIterator );
		}

		inline bool operator<( const RIterator& a_RIterator ) const
		{
			return Iterator::operator<( a_RIterator );
		}

		inline bool operator<=( const RIterator& a_RIterator ) const
		{
			return Iterator::operator<=( a_RIterator );
		}

	private:

		friend class Enumerable< T >;

	};

private:

	template < typename Iter, typename = EnableIfCompatible< Iter > >
	Enumerable( Iter a_Begin, Iter a_End, size_t a_Size )
		: m_Begin( new Iter( a_Begin ) )
		, m_End( new Iter( a_End ) )
		, m_Size( a_Size )
		, m_Operator( Operator< Iter > )
	{ }

public:

	template < typename Iter, typename = EnableIfCompatible< Iter > >
	Enumerable( Iter a_Begin, Iter a_End )
		: m_Begin( new Iter( a_Begin ) )
		, m_End( new Iter( a_End ) )
		, m_Size( distance( a_Begin, a_End ) )
		, m_Operator( Operator< Iter > )
	{ }

	~Enumerable()
	{
		m_Operator( m_Begin, 0, 0, OperatorType::FDelete );
		m_Operator( m_End,   0, 0, OperatorType::FDelete );
	}

	inline Iterator Begin()
	{
		return Iterator( this, m_Operator( 0, 0, this, OperatorType::FBegin ) );
	}

	inline const Iterator Begin() const
	{
		return Iterator( this, m_Operator( 0, 0, this, OperatorType::FBegin ) );
	}

	inline RIterator RBegin()
	{
		return RIterator( this, m_Operator( 0, 0, this, OperatorType::RBegin ) );
	}

	inline const RIterator RBegin() const
	{
		return RIterator( this, m_Operator( 0, 0, this, OperatorType::RBegin ) );
	}

	inline Iterator End()
	{
		return Iterator( this, m_Operator( 0, 0, this, OperatorType::FEnd ) );
	}

	inline const Iterator End() const
	{
		return Iterator( this, m_Operator( 0, 0, this, OperatorType::FEnd ) );
	}

	inline RIterator REnd()
	{
		return RIterator( this, m_Operator( 0, 0, this, OperatorType::REnd ) );
	}

	inline const RIterator REnd() const
	{
		return RIterator( this, m_Operator( 0, 0, this, OperatorType::REnd ) );
	}

	inline size_t Size() const
	{
		return m_Size;
	}

private:

	template < typename Iter >
	static void* Operator( void* a_ValueA, void* a_ValueB, Enumerable< T >* a_Enumerable, OperatorType a_OperatorType )
	{
		switch ( a_OperatorType )
		{
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
				auto* Old = reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA );
				Iter* New = new Iter( Old->base() );
				++*New;
				return New;
			}

			Iter& Old = *reinterpret_cast< Iter* >( a_ValueA );
			Iter& New = *( new Iter( Old ) );

			if ( Old == *reinterpret_cast< Iter* >( a_Enumerable->m_End ) )
			{
				New = *reinterpret_cast< Iter* >( a_Enumerable->m_Begin );;
				return &New;
			}

			New = Old;
			return &++New;
		}
		case OperatorType::RConvert:
		{
			if constexpr ( _Is_bidi_iter_v< Iter > || _Is_random_iter_v< Iter > )
			{
				Iter* Old = reinterpret_cast< Iter* >( a_ValueA );
				auto* New = new reverse_iterator< Iter >( make_reverse_iterator( *Old ) );
				++*New;
				return New;
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
				return new reverse_iterator< Iter >( *reinterpret_cast< Iter* >( a_Enumerable->m_End ) );
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
			return &**reinterpret_cast< Iter* >( a_ValueA );
		}
		case OperatorType::RDereference:
		{
			if constexpr ( _Is_bidi_iter_v< Iter > || _Is_random_iter_v< Iter > )
			{
				return &**reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA );
			}

			return &**reinterpret_cast< Iter* >( a_ValueA );
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

				for ( Left = *reinterpret_cast< const Iter* >( a_Enumerable->m_Begin ); Left != Right; ++Left, ++Length );

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
				auto Left = *reinterpret_cast< reverse_iterator< Iter >* >( a_ValueA );  auto LeftVal = *Left;
				auto& Right = *reinterpret_cast< reverse_iterator< Iter >* >( a_ValueB );

				if ( Left == Right )
				{
					return 0;
				}

				int Length = 0;
				auto End = ++make_reverse_iterator( *reinterpret_cast< Iter* >( a_Enumerable->m_Begin ) );
				for ( ; Left != Right && Left != End; ++Left, ++Length );

				if ( Left == Right )
				{
					return reinterpret_cast< void* >( Length );
				}

				for ( Left = *reinterpret_cast< reverse_iterator< Iter >* >( a_Enumerable->m_Begin ); Left != Right; ++Left, ++Length );
				return reinterpret_cast< void* >( Length - a_Enumerable->m_Size );
			}

			Iter Left = *reinterpret_cast< Iter* >( a_ValueA );
			Iter& Right = *reinterpret_cast< Iter* >( a_ValueB );

			if ( Left == Right )
			{
				return 0;
			}

			int Length = 0;
			Iter& Beg = *reinterpret_cast< Iter* >( a_Enumerable->m_Begin );
			Iter& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );

			if ( Left == End )
			{
				Left = Beg;
				++Length;
			}

			for ( ; Left != Right && Left != End; ++Left, ++Length );

			if ( Left == Right )
			{
				return reinterpret_cast< void* >( Length );
			}

			for ( Left = Beg; Left != Right; ++Left, ++Length );
			return reinterpret_cast< void* >( Length - a_Enumerable->m_Size );
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
			return nullptr;
		}
		case OperatorType::RGreaterEqual:
		{
			return nullptr;
		}
		case OperatorType::FLesserEqual:
		{
			return nullptr;
		}
		case OperatorType::RLesserEqual:
		{
			return nullptr;
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
	void* ( *m_Operator )( void*, void*, Enumerable< T >*, OperatorType );
};