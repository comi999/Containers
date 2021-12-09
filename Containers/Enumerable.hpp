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
		Create,
		Delete,
		LShift,
		RShift,
		Dereference,
		Distance,
		Equality,
		Greater,
		GreaterEqual,
		Lesser,
		LesserEqual,
		Assign
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
			, m_Iterator( m_Enumerable->m_Operator( a_Iterator.m_Iterator, 0, 0, OperatorType::Create ) )
		{ }

		Iterator( Iterator&& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
			, m_Iterator( a_Iterator.m_Iterator )
		{ }

		Iterator( RIterator& a_RIterator )
			: m_Enumerable( a_RIterator.m_Enumerable )
			, m_Iterator( m_Enumerable->m_Operator( a_RIterator.m_Iterator, 0, 0, OperatorType::Create ) )
		{ }
		
		Iterator( RIterator&& a_RIterator )
			: m_Enumerable( a_RIterator.m_Enumerable )
			, m_Iterator( a_RIterator.m_Iterator )
		{ }

		~Iterator()
		{
			m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::Delete );
		}

		inline Iterator& operator++()
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( 1 ), m_Enumerable, OperatorType::RShift );
			return *this;
		}

		inline Iterator operator++( int )
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::Create );
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( 1 ), m_Enumerable, OperatorType::RShift );
			return Iterator( m_Enumerable, New );
		}

		inline Iterator& operator--()
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( -1 ), m_Enumerable, OperatorType::RShift );
			return *this;
		}

		inline Iterator operator--( int )
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::Create );
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( -1 ), m_Enumerable, OperatorType::RShift );
			return Iterator( m_Enumerable, New );
		}

		inline Iterator& operator+=( int a_Places )
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( a_Places ), m_Enumerable, OperatorType::RShift );
			return *this;
		}

		inline Iterator& operator-=( int a_Places )
		{
			m_Enumerable->m_Operator( m_Iterator, reinterpret_cast< void* >( -a_Places ), m_Enumerable, OperatorType::RShift );
			return *this;
		}

		inline Iterator operator+( int a_Places ) const
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::Create );
			return Iterator( m_Enumerable, m_Enumerable->m_Operator( New, reinterpret_cast< void* >( a_Places ), m_Enumerable, OperatorType::RShift ) );
		}

		inline Iterator operator-( int a_Places ) const
		{
			void* New = m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::Create );
			return Iterator( m_Enumerable, m_Enumerable->m_Operator( New, reinterpret_cast< void* >( -a_Places ), m_Enumerable, OperatorType::RShift ) );
		}

		inline int operator-( const Iterator& a_Iterator ) const
		{
			return reinterpret_cast< int >( m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::Distance ) );
		}
		
		inline Iterator& operator=( Iterator& a_Iterator )
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, 0, OperatorType::Assign );
			return *this;
		}

		inline T& operator*()
		{
			return *reinterpret_cast< T* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::Dereference ) );
		}

		inline const T& operator*() const
		{
			return *reinterpret_cast< T* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::Dereference ) );
		}

		inline T* operator->()
		{
			return *reinterpret_cast< T* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::Dereference ) );
		}

		inline const T* operator->() const
		{
			return *reinterpret_cast< T* >( m_Enumerable->m_Operator( m_Iterator, 0, 0, OperatorType::Dereference ) );
		}

		inline bool operator==( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, 0, OperatorType::Equality );
		}

		inline bool operator!=( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return !m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, 0, OperatorType::Equality );
		}

		inline bool operator>( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::Greater );
		}

		inline bool operator>=( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::GreaterEqual );
		}

		inline bool operator<( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::Lesser );
		}

		inline bool operator<=( const Iterator& a_Iterator ) const
		{
			_STL_ASSERT( m_Enumerable->m_Operator == a_Iterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return m_Enumerable->m_Operator( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, OperatorType::LesserEqual );
		}

	private:

		friend class Enumerable< T >;
		friend class Enumerable< T >::RIterator;

		Enumerable< T >* m_Enumerable;
		void*            m_Iterator;

	};

	class RIterator : private Iterator
	{
	public:

		using iterator_category = random_access_iterator_tag;
		using difference_type = ptrdiff_t;
		using value_type = T;
		using pointer = value_type*;
		using reference = value_type&;

	private:

		RIterator( Enumerable< T >* a_Enumerable, void* a_Iterator )
			: Iterator( a_Enumerable, a_Iterator )
		{ }

	public:

		RIterator( Iterator& a_Iterator )//Should this offset in negative direction by 1?
			: Iterator( a_Iterator )
		{ }

		RIterator( Iterator&& a_Iterator )
			: Iterator( a_Iterator )
		{ }

		RIterator( RIterator& a_RIterator )
			: Iterator( a_RIterator )
		{ }

		RIterator( RIterator&& a_RIterator )
			: Iterator( a_RIterator )
		{ }

		inline RIterator& operator++()
		{
			Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, reinterpret_cast< void* >( 1 ), Iterator::m_Enumerable, OperatorType::LShift );
			return *this;
		}

		inline RIterator operator++( int )
		{
			void* New = Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, 0, 0, OperatorType::Create );
			Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, reinterpret_cast< void* >( 1 ), Iterator::m_Enumerable, OperatorType::LShift );
			return RIterator( Iterator::m_Enumerable, New );
		}

		inline RIterator& operator--()
		{
			Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, reinterpret_cast< void* >( -1 ), Iterator::m_Enumerable, OperatorType::LShift );
			return *this;
		}

		inline RIterator operator--( int )
		{
			void* New = Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, 0, 0, OperatorType::Create );
			Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, reinterpret_cast< void* >( -1 ), Iterator::m_Enumerable, OperatorType::LShift );
			return RIterator( Iterator::m_Enumerable, New );
		}

		inline RIterator& operator+=( int a_Places )
		{
			Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, reinterpret_cast< void* >( a_Places ), Iterator::m_Enumerable, OperatorType::LShift );
			return *this;
		}

		inline RIterator& operator-=( int a_Places )
		{
			Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, reinterpret_cast< void* >( -a_Places ), Iterator::m_Enumerable, OperatorType::LShift );
			return *this;
		}

		inline RIterator operator+( int a_Places ) const
		{
			void* New = Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, 0, 0, OperatorType::Create );
			return RIterator( Iterator::m_Enumerable, Iterator::m_Enumerable->m_Operator( New, reinterpret_cast< void* >( a_Places ), Iterator::m_Enumerable, OperatorType::LShift ) );
		}

		inline RIterator operator-( int a_Places ) const
		{
			void* New = Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, 0, 0, OperatorType::Create );
			return RIterator( Iterator::m_Enumerable, Iterator::m_Enumerable->m_Operator( New, reinterpret_cast< void* >( -a_Places ), Iterator::m_Enumerable, OperatorType::LShift ) );
		}

		inline int operator-( const RIterator& a_RIterator ) const
		{
			return reinterpret_cast< int >( Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, a_RIterator.m_Iterator, Iterator::m_Enumerable, OperatorType::Distance ) );
		}

		inline RIterator& operator=( RIterator& a_RIterator )
		{
			_STL_ASSERT( Iterator::m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, a_RIterator.m_Iterator, 0, OperatorType::Assign );
			return *this;
		}

		inline T& operator*()
		{
			return *reinterpret_cast< T* >( Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, 0, 0, OperatorType::Dereference ) );
		}

		inline const T& operator*() const
		{
			return *reinterpret_cast< T* >( Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, 0, 0, OperatorType::Dereference ) );
		}

		inline T* operator->()
		{
			return *reinterpret_cast< T* >( Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, 0, 0, OperatorType::Dereference ) );
		}

		inline const T* operator->() const
		{
			return *reinterpret_cast< T* >( Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, 0, 0, OperatorType::Dereference ) );
		}

		inline bool operator==( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( Iterator::m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, a_RIterator.m_Iterator, 0, OperatorType::Equality );
		}

		inline bool operator!=( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( Iterator::m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return !Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, a_RIterator.m_Iterator, 0, OperatorType::Equality );
		}

		inline bool operator>( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( Iterator::m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, a_RIterator.m_Iterator, Iterator::m_Enumerable, OperatorType::Greater );
		}

		inline bool operator>=( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( Iterator::m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, a_RIterator.m_Iterator, Iterator::m_Enumerable, OperatorType::GreaterEqual );
		}

		inline bool operator<( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( Iterator::m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, a_RIterator.m_Iterator, Iterator::m_Enumerable, OperatorType::Lesser );
		}

		inline bool operator<=( const RIterator& a_RIterator ) const
		{
			_STL_ASSERT( Iterator::m_Enumerable->m_Operator == a_RIterator.m_Enumerable->m_Operator, "Iterators are different types." );
			return Iterator::m_Enumerable->m_Operator( Iterator::m_Iterator, a_RIterator.m_Iterator, Iterator::m_Enumerable, OperatorType::LesserEqual );
		}

	private:

		friend class Enumerable< T >;
		friend class Enumerable< T >::Iterator;

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
		m_Operator( m_Begin, 0, 0, OperatorType::Delete );
		m_Operator( m_End,   0, 0, OperatorType::Delete );
	}

	inline Iterator Begin()
	{
		return Iterator( this, m_Operator( m_Begin, 0, 0, OperatorType::Create ) );
	}

	inline const Iterator Begin() const
	{
		return Iterator( this, m_Operator( m_Begin, 0, 0, OperatorType::Create ) );
	}

	inline RIterator RBegin()
	{
		void* New = m_Operator( m_End, 0, 0, OperatorType::Create );
		m_Operator( New, reinterpret_cast< void* >( -1 ), this, OperatorType::RShift );
		return RIterator( this, New );
	}

	inline const RIterator RBegin() const
	{
		return RIterator( this, m_Operator( m_End, 0, 0, OperatorType::Create ) ) + 1;
	}

	inline CIterator CBegin()
	{
		return CIterator( this, m_Operator( m_Begin, 0, 0, OperatorType::Create ) );
	}

	inline const CIterator CBegin() const
	{
		return CIterator( this, m_Operator( m_Begin, 0, 0, OperatorType::Create ) );
	}

	inline Iterator End()
	{
		return Iterator( this, m_Operator( m_End, 0, 0, OperatorType::Create ) );
	}

	inline const Iterator End() const
	{
		return Iterator( this, m_Operator( m_End, 0, 0, OperatorType::Create ) );
	}

	inline RIterator REnd()
	{
		return RIterator( this, m_Operator( m_End, 0, 0, OperatorType::Create ) );
	}

	inline const RIterator REnd() const
	{
		return RIterator( this, m_Operator( m_End, 0, 0, OperatorType::Create ) );
	}

	inline CIterator CEnd()
	{
		return CIterator( this, m_Operator( m_End, 0, 0, OperatorType::Create ) );
	}

	inline const CIterator CEnd() const
	{
		return CIterator( this, m_Operator( m_End, 0, 0, OperatorType::Create ) );
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
		case OperatorType::Create:
		{
			return new Iter( *reinterpret_cast< Iter* >( a_ValueA ) );
		}
		case OperatorType::Delete:
		{
			delete reinterpret_cast< Iter* >( a_ValueA );
			return nullptr;
		}
		case OperatorType::LShift:
		{
			int Places = reinterpret_cast< int >( a_ValueB );

			if constexpr ( _Is_bidi_iter_v< Iter > || _Is_random_iter_v< Iter > )
			{
				advance( *reinterpret_cast< Iter* >( a_ValueA ), -Places );
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
				int Distance = 0;
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
		case OperatorType::RShift:
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
		case OperatorType::Dereference:
		{
			return &**reinterpret_cast< Iter* >( a_ValueA );
		}
		case OperatorType::Distance:
		{
			Iter& Left = *reinterpret_cast< Iter* >( a_ValueA );
			Iter& Right = *reinterpret_cast< Iter* >( a_ValueB );

			if constexpr ( _Is_random_iter_v< Iter > )
			{
				return Right - Left;
			}

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
		case OperatorType::Equality:
		{
			return reinterpret_cast< void* >( *reinterpret_cast< const Iter* >( a_ValueA ) == *reinterpret_cast< const Iter* >( a_ValueB ) );
		}
		case OperatorType::Greater:
		{
			if constexpr ( _Is_random_iter_v< Iter > )
			{
				return *reinterpret_cast< Iter* >( a_ValueA ) > *reinterpret_cast< Iter* >( a_ValueB );
			}
			else
			{
				if ( *reinterpret_cast< Iter* >( a_ValueA ) == *reinterpret_cast< Iter* >( a_ValueB ) )
				{
					return reinterpret_cast< void* >( false );
				}

				Iter Left = *reinterpret_cast< Iter* >( a_ValueA );
				Iter& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );
				Iter& Sentinel = *reinterpret_cast< Iter* >( a_ValueB );

				for ( ; Left != End; ++Left )
				{
					if ( Left == Sentinel )
					{
						return reinterpret_cast< void* >( false );
					}
				}

				return reinterpret_cast< void* >( true );
			}
		}
		case OperatorType::GreaterEqual:
		{
			if constexpr ( _Is_random_iter_v< Iter > )
			{
				return *reinterpret_cast< Iter* >( a_ValueA ) >= *reinterpret_cast< Iter* >( a_ValueB );
			}
			else
			{
				if ( *reinterpret_cast< Iter* >( a_ValueA ) == *reinterpret_cast< Iter* >( a_ValueB ) )
				{
					return reinterpret_cast< void* >( true );
				}

				Iter Left = *reinterpret_cast< Iter* >( a_ValueA );
				Iter& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );
				Iter& Sentinel = *reinterpret_cast< Iter* >( a_ValueB );

				for ( ; Left != End; ++Left )
				{
					if ( Left == Sentinel )
					{
						return reinterpret_cast< void* >( false );
					}
				}

				return reinterpret_cast< void* >( true );
			}
		}
		case OperatorType::Lesser:
		{
			if constexpr ( _Is_random_iter_v< Iter > )
			{
				return *reinterpret_cast< Iter* >( a_ValueB ) > *reinterpret_cast< Iter* >( a_ValueA );
			}
			else
			{
				if ( *reinterpret_cast< Iter* >( a_ValueA ) == *reinterpret_cast< Iter* >( a_ValueB ) )
				{
					return reinterpret_cast< void* >( false );
				}

				Iter Right = *reinterpret_cast< Iter* >( a_ValueB );
				Iter& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );
				Iter& Sentinel = *reinterpret_cast< Iter* >( a_ValueA );

				for ( ; Right != End; ++Right )
				{
					if ( Right == Sentinel )
					{
						return reinterpret_cast< void* >( false );
					}
				}

				return reinterpret_cast< void* >( true );
			}
		}
		case OperatorType::LesserEqual:
		{
			if constexpr ( _Is_random_iter_v< Iter > )
			{
				return *reinterpret_cast< Iter* >( a_ValueB ) >= *reinterpret_cast< Iter* >( a_ValueA );
			}
			else
			{
				if ( *reinterpret_cast< Iter* >( a_ValueB ) == *reinterpret_cast< Iter* >( a_ValueA ) )
				{
					return reinterpret_cast< void* >( true );
				}

				Iter Right = *reinterpret_cast< Iter* >( a_ValueB );
				Iter& End = *reinterpret_cast< Iter* >( a_Enumerable->m_End );
				Iter& Sentinel = *reinterpret_cast< Iter* >( a_ValueA );

				for ( ; Right != End; ++Right )
				{
					if ( Right == Sentinel )
					{
						return reinterpret_cast< void* >( false );
					}
				}

				return reinterpret_cast< void* >( true );
			}
		}
		case OperatorType::Assign:
		{
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