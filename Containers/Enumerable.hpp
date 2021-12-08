#pragma once
#include <type_traits>
#include <iterator>

using namespace std;

template < typename T >
class Enumerable
{
	template < typename Iter >
	using EnableIfCompatible = std::enable_if_t< _Is_iterator_v< Iter >&& is_same_v< typename iterator_traits< Iter >::value_type, T >, void >;

	enum class ModifyType
	{
		Create,
		Delete,
		Shift,
		Dereference
	};

	enum class CompareType
	{
		Distance,
		Equality,
		Lesser,
		Greater
	};

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

		Iterator( Iterator& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
			, m_Iterator( m_Enumerable.m_Modifier( &a_Iterator, 0, ModifyType::Create ) )
		{ }

		Iterator( Iterator&& a_Iterator )
			: m_Enumerable( a_Iterator.m_Enumerable )
			, m_Iterator( a_Iterator.m_Iterator )
		{ }

		~Iterator()
		{
			m_Enumerable.m_Modifier( m_Iterator, 0, ModifyType::Delete );
		}

		inline Iterator& operator++()
		{
			return *reinterpret_cast< Iterator* >( m_Enumerable.m_Modifier( this, 1, ModifyType::Shift ) );
		}

		inline Iterator operator++( int )
		{
			void* New = m_Enumerable.m_Modifier( this, 0, ModifyType::Create );
			m_Enumerable.m_Modifier( this, 1, ModifyType::Shift );
			return Iterator( m_Enumerable, New );
		}

		inline Iterator& operator--()
		{
			return *reinterpret_cast< Iterator* >( m_Enumerable.m_Modifier( this, -1, ModifyType::Shift ) );
		}

		inline Iterator operator--( int )
		{
			void* New = m_Enumerable.m_Modifier( this, 0, ModifyType::Create );
			m_Enumerable.m_Modifier( this, -1, ModifyType::Shift );
			return Iterator( m_Enumerable, New );
		}

		inline Iterator& operator+=( int a_Places )
		{
			return *reinterpret_cast< Iterator* >( m_Enumerable.m_Modifier( this, a_Places, ModifyType::Shift ) );
		}

		inline Iterator& operator-=( int a_Places )
		{
			return *reinterpret_cast< Iterator* >( m_Enumerable.m_Modifier( this, -a_Places, ModifyType::Shift ) );
		}

		inline Iterator operator+( int a_Places )
		{
			Iterator New( *this );
			m_Enumerable.m_Modifier( &New, a_Places, ModifyType::Shift );
			return Iterator( New );
		}

		inline Iterator operator-( int a_Places )
		{
			Iterator New( *this );
			m_Enumerable.m_Modifier( &New, -a_Places, ModifyType::Shift );
			return Iterator( New );
		}

		inline int operator-( Iterator a_Iterator )
		{
			return 0;//return m_Enumerable.m_Difference( m_Iterator, a_Iterator.m_Iterator, m_Enumerable );
		}
		
		inline T& operator*()
		{
			auto val = *this;
			return *reinterpret_cast< T* >( m_Enumerable.m_Modifier( this, 0, ModifyType::Dereference ) );
		}

		inline T* operator->()
		{
			return *reinterpret_cast< T* >( m_Enumerable.m_Modifier( this, 0, ModifyType::Dereference ) );
		}

		inline bool operator==( const Iterator& a_Iterator ) const
		{
			return false; //return m_Enumerable.m_IteratorEqual( m_Iterator, a_Iterator.m_Iterator );
		}

		inline bool operator!=( const Iterator& a_Iterator ) const
		{
			return !m_Enumerable.m_Comparer( m_Iterator, a_Iterator.m_Iterator, m_Enumerable, CompareType::Equality );
		}

	private:

		friend class Enumerable< T >;

		Enumerable< T >& m_Enumerable;
		void*            m_Iterator;
	};

	template < typename Iter, typename = EnableIfCompatible< Iter > >
	Enumerable( Iter a_Begin, Iter a_End )
		: m_Begin( new Iter( a_Begin ) )
		, m_End( new Iter( a_End ) )
		, m_Size( distance( a_Begin, a_End ) )
		, m_Modifier( Modifier< Iter > )
		, m_Comparer( Comparer< Iter > )
	{ }

	~Enumerable()
	{
		Iterator Begin( *this, m_Begin );
		Iterator End  ( *this, m_End );
		m_Modifier( &Begin, 0, ModifyType::Delete );
		m_Modifier( &End,   0, ModifyType::Delete );
	}

	inline Iterator Begin()
	{
		return Iterator( *this, m_Modifier( m_Begin, 0, ModifyType::Create ) );
	}

	inline Iterator End()
	{
		return Iterator( *this, m_Modifier( m_End, 0, ModifyType::Create ) );
	}

	inline size_t Size() const
	{
		return m_Size;
	}

private:

	template < typename Iter >
	static void* Modifier( void* a_Iterator, int a_Value, ModifyType a_ModifyType )
	{
		switch ( a_ModifyType )
		{
		case ModifyType::Create:
		{
			return new Iter( *reinterpret_cast< Iter* >( a_Iterator ) );
		}
		case ModifyType::Delete:
		{
			delete reinterpret_cast< Iter* >( a_Iterator );
			return nullptr;
		}
		case ModifyType::Shift:
		{
			if constexpr ( _Is_bidi_iter_v< Iter > || _Is_random_iter_v< Iter > )
			{
				advance( *reinterpret_cast< Iter* >( reinterpret_cast< Iterator* >( a_Iterator )->m_Iterator ), a_Value );
				return a_Iterator;
			}

			if ( !a_Value )
			{
				return a_Iterator;
			}
			else if ( a_Value > 0 )
			{
				Iter& Subject = *reinterpret_cast< Iter* >( reinterpret_cast< Iterator* >( a_Iterator )->m_Iterator );
				for ( ; a_Value > 0; --a_Value ) ++Subject;
				return a_Iterator;
			}
			else if ( a_Value < 0 )
			{
				const Iter& Begin = *reinterpret_cast< Iter* >( reinterpret_cast< Iterator* >( a_Iterator )->m_Enumerable.m_Begin );
				Iter& Subject = *reinterpret_cast< Iter* >( reinterpret_cast< Iterator* >( a_Iterator )->m_Iterator );
				auto Distance = distance( Begin, Subject );
				Subject = Begin;
				while ( --Distance > static_cast< decltype( Distance ) >( -a_Value - 1 ) ) ++Subject;
				return a_Iterator;
			}
		}
		case ModifyType::Dereference:
		{
			auto& iter = *reinterpret_cast< Iter* >( reinterpret_cast< Iterator* >( a_Iterator )->m_Iterator );
			auto& val = *iter;
			return &val;
		}
		}
	}

	template < typename Iter >
	static int Comparer( const void* a_Left, const void* a_Right, const Enumerable< T >& a_Enumerable, CompareType a_CompareType )
	{
		switch ( a_CompareType )
		{
		case CompareType::Distance:
		{
			Iter& Left  = *const_cast< Iter* >( reinterpret_cast< const Iter* >( a_Left  ) );
			Iter& Right = *const_cast< Iter* >( reinterpret_cast< const Iter* >( a_Right ) );

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
				const Iter& End = *reinterpret_cast< Iter* >( a_Enumerable.m_End );
				for ( ; Left != Right && Left != End; ++Left, ++Length );

				if ( Left == Right )
				{
					return -Length;
				}

				for ( Left = *reinterpret_cast< const Iter* >( a_Enumerable.m_Begin ); Left != Right; ++Left, ++Length );

				return a_Enumerable.m_Size - Length;
			}
		}
		case CompareType::Equality:
		{
			return *reinterpret_cast< const Iter* >( a_Left ) == *reinterpret_cast< const Iter* >( a_Right );
		}
		case CompareType::Greater:
		{

		}
		case CompareType::Lesser:
		{

		}
		}

		return 0;
	}

	void*  m_Begin;
	void*  m_End;
	size_t m_Size;

	void* ( *m_Modifier )( void*, int, ModifyType );
	int   ( *m_Comparer )( const void*, const void*, const Enumerable< T >&, CompareType );
};