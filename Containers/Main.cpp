#include <iostream>
#include <set>
#include "Array.hpp"
#include "Enumerable.hpp"
#include <queue>
#include <list>
#include "Delegate.hpp"
#include <map>
#include "ContainerTraits.hpp"

using namespace std;

using t = ContainerTraits::GetIterator< vector< int > >;


using ti = ContainerTraits::GetIteratorType< t >;

template < typename T >
struct Cont
{

    struct iterator
    {
        using iterator_category = forward_iterator_tag;
        using difference_type = ptrdiff_t;
        using value_type = T;
        using pointer = value_type*;
        using reference = value_type&;

        iterator() = default;

        iterator( T* elem )
        {
            element = elem;
        }

        iterator( const iterator& iter )
        {
            element = iter.element;
        }

        iterator operator++(int)
        {
            T* temp = element;
            element = element + 1;
            return iterator( temp );
        }

        iterator& operator++()
        {
            element = element + 1;
            return *this;
        }

        const iterator& operator++() const
        {
            element = element + 1;
            return *this;
        }

        T& operator*()
        {
            return *element;
        }

        T* operator->()
        {
            return nullptr;
        }

        bool operator==( const iterator& other ) const
        {
            return element == other.element;
        }

        bool operator!=( const iterator& other ) const
        {
            return element != other.element;
        }

        T* element;
    };

    Cont()
    {
        for ( int i = 0; i < 10; ++i )
        {
            num[i] = i;
        }
    }

    iterator Begin()
    {
        return iterator( num );
    }

    iterator End()
    {
        return iterator( &num[10] );
    }

    T num[10];
};

int main()
{
    using RIter = Enumerable< int >::RIterator;
    using FIter = Enumerable< int >::Iterator;

    Cont< int >   Forward;
    vector< int > Random = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    list< int >   Bidi   = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    Enumerable< int > F( Forward.Begin(), Forward.End() );
    Enumerable< int > R( Random.begin(),  Random.end()  );
    Enumerable< int > B( Bidi.begin(),    Bidi.end()    );

    auto F_FB = F.Begin();
    auto F_RB = F.RBegin();
    auto F_FE = F.End();
    auto F_RE = F.REnd();

    auto R_FB = R.Begin();
    auto R_RB = R.RBegin();
    auto R_FE = R.End();
    auto R_RE = R.REnd();

    auto B_FB = B.Begin();
    auto B_RB = B.RBegin();
    auto B_FE = B.End();
    auto B_RE = B.REnd();

    // Creating Iterator from Reverse iterator shifts it correctly.
    { _ASSERT( FIter( F_RB )     == F_FE ); }
    { _ASSERT( FIter( F_RE )     == F_FB ); }
    { _ASSERT( FIter( F_RB + 1 ) == F_FE - 1 ); }
    { _ASSERT( FIter( F_RE - 1 ) == F_FB + 1 ); }

    { _ASSERT( RIter( F_FB )     == F_RE ); }
    { _ASSERT( RIter( F_FE )     == F_RB ); }
    { _ASSERT( RIter( F_FB + 1 ) == F_RE - 1 ); }
    { _ASSERT( RIter( F_FE - 1 ) == F_RB + 1 ); }

    { _ASSERT( FIter( B_RB )     == B_FE ); }
    { _ASSERT( FIter( B_RE )     == B_FB ); }
    { _ASSERT( FIter( B_RB + 1 ) == B_FE - 1 ); }
    { _ASSERT( FIter( B_RE - 1 ) == B_FB + 1 ); }

    { _ASSERT( RIter( B_FB )     == B_RE ); }
    { _ASSERT( RIter( B_FE )     == B_RB ); }
    { _ASSERT( RIter( B_FB + 1 ) == B_RE - 1 ); }
    { _ASSERT( RIter( B_FE - 1 ) == B_RB + 1 ); }

    { _ASSERT( FIter( R_RB )     == R_FE ); }
    { _ASSERT( FIter( R_RE )     == R_FB ); }
    { _ASSERT( FIter( R_RB + 1 ) == R_FE - 1 ); }
    { _ASSERT( FIter( R_RE - 1 ) == R_FB + 1 ); }

    { _ASSERT( RIter( R_FB )     == R_RE ); }
    { _ASSERT( RIter( R_FE )     == R_RB ); }
    { _ASSERT( RIter( R_FB + 1 ) == R_RE - 1 ); }
    { _ASSERT( RIter( R_FE - 1 ) == R_RB + 1 ); }
    // Adding integer to iterator gives back correct temp
    { _ASSERT( *( F_FB + 1 ) == 1 ); }
    { _ASSERT( *( F_RB + 1 ) == 8 ); }
    { _ASSERT( *( F_FE - 1 ) == 9 ); }
    { _ASSERT( *( F_RE - 1 ) == 0 ); }

    { _ASSERT( *( B_FB + 1 ) == 1 ); }
    { _ASSERT( *( B_RB + 1 ) == 8 ); }
    { _ASSERT( *( B_FE - 1 ) == 9 ); }
    { _ASSERT( *( B_RE - 1 ) == 0 ); }

    { _ASSERT( *( R_FB + 1 ) == 1 ); }
    { _ASSERT( *( R_RB + 1 ) == 8 ); }
    { _ASSERT( *( R_FE - 1 ) == 9 ); }
    { _ASSERT( *( R_RE - 1 ) == 0 ); }
    // Incrementing iterator by integer gives correct value
    { _ASSERT( *( FIter( F_FB ) += 1 ) == 1 ); }
    { _ASSERT( *( RIter( F_RB ) += 1 ) == 8 ); }
    { _ASSERT( *( FIter( F_FE ) -= 1 ) == 9 ); }
    { _ASSERT( *( RIter( F_RE ) -= 1 ) == 0 ); }

    { _ASSERT( *( FIter( B_FB ) += 1 ) == 1 ); }
    { _ASSERT( *( RIter( B_RB ) += 1 ) == 8 ); }
    { _ASSERT( *( FIter( B_FE ) -= 1 ) == 9 ); }
    { _ASSERT( *( RIter( B_RE ) -= 1 ) == 0 ); }

    { _ASSERT( *( FIter( R_FB ) += 1 ) == 1 ); }
    { _ASSERT( *( RIter( R_RB ) += 1 ) == 8 ); }
    { _ASSERT( *( FIter( R_FE ) -= 1 ) == 9 ); }
    { _ASSERT( *( RIter( R_RE ) -= 1 ) == 0 ); }
    // Distance between iterators is correct.
    { _ASSERT( ( F_FB - F_FE ) == -10 ); }
    { _ASSERT( ( F_FE - F_FB ) ==  10 ); }
    { _ASSERT( ( F_RB - F_RE ) == -10 ); }
    { _ASSERT( ( F_RE - F_RB ) ==  10 ); }

    { _ASSERT( ( B_FB - B_FE ) == -10 ); }
    { _ASSERT( ( B_FE - B_FB ) ==  10 ); }
    { _ASSERT( ( B_RB - B_RE ) == -10 ); }
    { _ASSERT( ( B_RE - B_RB ) ==  10 ); }

    { _ASSERT( ( R_FB - R_FE ) == -10 ); }
    { _ASSERT( ( R_FE - R_FB ) ==  10 ); }
    { _ASSERT( ( R_RB - R_RE ) == -10 ); }
    { _ASSERT( ( R_RE - R_RB ) ==  10 ); }

    { _ASSERT( ( ( F_FB + 1 ) - ( F_FE - 1 ) ) == -8 ); }
    { _ASSERT( ( ( F_FE - 1 ) - ( F_FB + 1 ) ) ==  8 ); }
    { _ASSERT( ( ( F_RB + 1 ) - ( F_RE - 1 ) ) == -8 ); }
    { _ASSERT( ( ( F_RE - 1 ) - ( F_RB + 1 ) ) ==  8 ); }

    { _ASSERT( ( ( B_FB + 1 ) - ( B_FE - 1 ) ) == -8 ); }
    { _ASSERT( ( ( B_FE - 1 ) - ( B_FB + 1 ) ) ==  8 ); }
    { _ASSERT( ( ( B_RB + 1 ) - ( B_RE - 1 ) ) == -8 ); }
    { _ASSERT( ( ( B_RE - 1 ) - ( B_RB + 1 ) ) ==  8 ); }

    { _ASSERT( ( ( R_FB + 1 ) - ( R_FE - 1 ) ) == -8 ); }
    { _ASSERT( ( ( R_FE - 1 ) - ( R_FB + 1 ) ) ==  8 ); }
    { _ASSERT( ( ( R_RB + 1 ) - ( R_RE - 1 ) ) == -8 ); }
    { _ASSERT( ( ( R_RE - 1 ) - ( R_RB + 1 ) ) ==  8 ); }
    // Check increment and decrement operators work.
    { _ASSERT( *( FIter( F_FB )++ ) == 0 ); }
    { _ASSERT( *( RIter( F_RB )++ ) == 9 ); }
    { _ASSERT(  ( FIter( F_FE )-- ) == F_FE ); }
    { _ASSERT(  ( RIter( F_RE )-- ) == F_RE ); }

    { _ASSERT( *( FIter( B_FB )++ ) == 0 ); }
    { _ASSERT( *( RIter( B_RB )++ ) == 9 ); }
    { _ASSERT(  ( FIter( B_FE )-- ) == B_FE ); }
    { _ASSERT(  ( RIter( B_RE )-- ) == B_RE ); }

    { _ASSERT( *( FIter( R_FB )++ ) == 0 ); }
    { _ASSERT( *( RIter( R_RB )++ ) == 9 ); }
    { _ASSERT(  ( FIter( R_FE )-- ) == R_FE ); }
    { _ASSERT(  ( RIter( R_RE )-- ) == R_RE ); }

    { _ASSERT( *( ++FIter( F_FB ) ) == 1 ); }
    { _ASSERT( *( ++RIter( F_RB ) ) == 8 ); }
    { _ASSERT( *( --FIter( F_FE ) ) == 9 ); }
    { _ASSERT( *( --RIter( F_RE ) ) == 0 ); }

    { _ASSERT( *( ++FIter( B_FB ) ) == 1 ); }
    { _ASSERT( *( ++RIter( B_RB ) ) == 8 ); }
    { _ASSERT( *( --FIter( B_FE ) ) == 9 ); }
    { _ASSERT( *( --RIter( B_RE ) ) == 0 ); }

    { _ASSERT( *( ++FIter( R_FB ) ) == 1 ); }
    { _ASSERT( *( ++RIter( R_RB ) ) == 8 ); }
    { _ASSERT( *( --FIter( R_FE ) ) == 9 ); }
    { _ASSERT( *( --RIter( R_RE ) ) == 0 ); }

    { _ASSERT( F_FB >= F_FE == false ); }
    { _ASSERT( F_FB >  F_FE == false ); }
    { _ASSERT( F_FE >= F_FB == true  ); }
    { _ASSERT( F_FE >  F_FB == true  ); }
    { _ASSERT( F_FB >= F_FB == true  ); }
    { _ASSERT( F_FB >  F_FB == false ); }
    { _ASSERT( F_FE >= F_FE == true  ); }
    { _ASSERT( F_FE >  F_FE == false ); }

    { _ASSERT( B_FB >= B_FE == false ); }
    { _ASSERT( B_FB >  B_FE == false ); }
    { _ASSERT( B_FE >= B_FB == true  ); }
    { _ASSERT( B_FE >  B_FB == true  ); }
    { _ASSERT( B_FB >= B_FB == true  ); }
    { _ASSERT( B_FB >  B_FB == false ); }
    { _ASSERT( B_FE >= B_FE == true  ); }
    { _ASSERT( B_FE >  B_FE == false ); }

    { _ASSERT( R_FB >= R_FE == false ); }
    { _ASSERT( R_FB >  R_FE == false ); }
    { _ASSERT( R_FE >= R_FB == true  ); }
    { _ASSERT( R_FE >  R_FB == true  ); }
    { _ASSERT( R_FB >= R_FB == true  ); }
    { _ASSERT( R_FB >  R_FB == false ); }
    { _ASSERT( R_FE >= R_FE == true  ); }
    { _ASSERT( R_FE >  R_FE == false ); }

    { _ASSERT( ( F_FB + 1 ) >= ( F_FE - 1 ) == false ); }
    { _ASSERT( ( F_FB + 1 ) >  ( F_FE - 1 ) == false ); }
    { _ASSERT( ( F_FE - 1 ) >= ( F_FB + 1 ) == true  ); }
    { _ASSERT( ( F_FE - 1 ) >  ( F_FB + 1 ) == true  ); }
    { _ASSERT( ( F_FB + 1 ) >= ( F_FB + 1 ) == true  ); }
    { _ASSERT( ( F_FB + 1 ) >  ( F_FB + 1 ) == false ); }
    { _ASSERT( ( F_FE - 1 ) >= ( F_FE - 1 ) == true  ); }
    { _ASSERT( ( F_FE - 1 ) >  ( F_FE - 1 ) == false ); }

    { _ASSERT( ( B_FB + 1 ) >= ( B_FE - 1 ) == false ); }
    { _ASSERT( ( B_FB + 1 ) >  ( B_FE - 1 ) == false ); }
    { _ASSERT( ( B_FE - 1 ) >= ( B_FB + 1 ) == true  ); }
    { _ASSERT( ( B_FE - 1 ) >  ( B_FB + 1 ) == true  ); }
    { _ASSERT( ( B_FB + 1 ) >= ( B_FB + 1 ) == true  ); }
    { _ASSERT( ( B_FB + 1 ) >  ( B_FB + 1 ) == false ); }
    { _ASSERT( ( B_FE - 1 ) >= ( B_FE - 1 ) == true  ); }
    { _ASSERT( ( B_FE - 1 ) >  ( B_FE - 1 ) == false ); }

    { _ASSERT( ( R_FB + 1 ) >= ( R_FE - 1 ) == false ); }
    { _ASSERT( ( R_FB + 1 ) >  ( R_FE - 1 ) == false ); }
    { _ASSERT( ( R_FE - 1 ) >= ( R_FB + 1 ) == true  ); }
    { _ASSERT( ( R_FE - 1 ) >  ( R_FB + 1 ) == true  ); }
    { _ASSERT( ( R_FB + 1 ) >= ( R_FB + 1 ) == true  ); }
    { _ASSERT( ( R_FB + 1 ) >  ( R_FB + 1 ) == false ); }
    { _ASSERT( ( R_FE - 1 ) >= ( R_FE - 1 ) == true  ); }
    { _ASSERT( ( R_FE - 1 ) >  ( R_FE - 1 ) == false ); }

    { _ASSERT( F_RB >= F_RE == false ); }
    { _ASSERT( F_RB >  F_RE == false ); }
    { _ASSERT( F_RE >= F_RB == true  ); }
    { _ASSERT( F_RE >  F_RB == true  ); }
    { _ASSERT( F_RB >= F_RB == true  ); }
    { _ASSERT( F_RB >  F_RB == false ); }
    { _ASSERT( F_RE >= F_RE == true  ); }
    { _ASSERT( F_RE >  F_RE == false ); }

    { _ASSERT( B_RB >= B_RE == false ); }
    { _ASSERT( B_RB >  B_RE == false ); }
    { _ASSERT( B_RE >= B_RB == true  ); }
    { _ASSERT( B_RE >  B_RB == true  ); }
    { _ASSERT( B_RB >= B_RB == true  ); }
    { _ASSERT( B_RB >  B_RB == false ); }
    { _ASSERT( B_RE >= B_RE == true  ); }
    { _ASSERT( B_RE >  B_RE == false ); }

    { _ASSERT( R_RB >= R_RE == false ); }
    { _ASSERT( R_RB >  R_RE == false ); }
    { _ASSERT( R_RE >= R_RB == true  ); }
    { _ASSERT( R_RE >  R_RB == true  ); }
    { _ASSERT( R_RB >= R_RB == true  ); }
    { _ASSERT( R_RB >  R_RB == false ); }
    { _ASSERT( R_RE >= R_RE == true  ); }
    { _ASSERT( R_RE >  R_RE == false ); }

    { _ASSERT( ( F_RB + 1 ) >= ( F_RE - 1 ) == false ); }
    { _ASSERT( ( F_RB + 1 ) >  ( F_RE - 1 ) == false ); }
    { _ASSERT( ( F_RE - 1 ) >= ( F_RB + 1 ) == true  ); }
    { _ASSERT( ( F_RE - 1 ) >  ( F_RB + 1 ) == true  ); }
    { _ASSERT( ( F_RB + 1 ) >= ( F_RB + 1 ) == true  ); }
    { _ASSERT( ( F_RB + 1 ) >  ( F_RB + 1 ) == false ); }
    { _ASSERT( ( F_RE - 1 ) >= ( F_RE - 1 ) == true  ); }
    { _ASSERT( ( F_RE - 1 ) >  ( F_RE - 1 ) == false ); }

    { _ASSERT( ( B_RB + 1 ) >= ( B_RE - 1 ) == false ); }
    { _ASSERT( ( B_RB + 1 ) >  ( B_RE - 1 ) == false ); }
    { _ASSERT( ( B_RE - 1 ) >= ( B_RB + 1 ) == true  ); }
    { _ASSERT( ( B_RE - 1 ) >  ( B_RB + 1 ) == true  ); }
    { _ASSERT( ( B_RB + 1 ) >= ( B_RB + 1 ) == true  ); }
    { _ASSERT( ( B_RB + 1 ) >  ( B_RB + 1 ) == false ); }
    { _ASSERT( ( B_RE - 1 ) >= ( B_RE - 1 ) == true  ); }
    { _ASSERT( ( B_RE - 1 ) >  ( B_RE - 1 ) == false ); }

    { _ASSERT( ( R_RB + 1 ) >= ( R_RE - 1 ) == false ); }
    { _ASSERT( ( R_RB + 1 ) >  ( R_RE - 1 ) == false ); }
    { _ASSERT( ( R_RE - 1 ) >= ( R_RB + 1 ) == true  ); }
    { _ASSERT( ( R_RE - 1 ) >  ( R_RB + 1 ) == true  ); }
    { _ASSERT( ( R_RB + 1 ) >= ( R_RB + 1 ) == true  ); }
    { _ASSERT( ( R_RB + 1 ) >  ( R_RB + 1 ) == false ); }
    { _ASSERT( ( R_RE - 1 ) >= ( R_RE - 1 ) == true  ); }
    { _ASSERT( ( R_RE - 1 ) >  ( R_RE - 1 ) == false ); }

    { _ASSERT( F_FB <= F_FE == true  ); }
    { _ASSERT( F_FB <  F_FE == true  ); }
    { _ASSERT( F_FE <= F_FB == false ); }
    { _ASSERT( F_FE <  F_FB == false ); }
    { _ASSERT( F_FB <= F_FB == true  ); }
    { _ASSERT( F_FB <  F_FB == false ); }
    { _ASSERT( F_FE <= F_FE == true  ); }
    { _ASSERT( F_FE <  F_FE == false ); }

    { _ASSERT( B_FB <= B_FE == true  ); }
    { _ASSERT( B_FB <  B_FE == true  ); }
    { _ASSERT( B_FE <= B_FB == false ); }
    { _ASSERT( B_FE <  B_FB == false ); }
    { _ASSERT( B_FB <= B_FB == true  ); }
    { _ASSERT( B_FB <  B_FB == false ); }
    { _ASSERT( B_FE <= B_FE == true  ); }
    { _ASSERT( B_FE <  B_FE == false ); }

    { _ASSERT( R_FB <= R_FE == true  ); }
    { _ASSERT( R_FB <  R_FE == true  ); }
    { _ASSERT( R_FE <= R_FB == false ); }
    { _ASSERT( R_FE <  R_FB == false ); }
    { _ASSERT( R_FB <= R_FB == true  ); }
    { _ASSERT( R_FB <  R_FB == false ); }
    { _ASSERT( R_FE <= R_FE == true  ); }
    { _ASSERT( R_FE <  R_FE == false ); }

    { _ASSERT( ( F_FB + 1 ) <= ( F_FE - 1 ) == true  ); }
    { _ASSERT( ( F_FB + 1 ) <  ( F_FE - 1 ) == true  ); }
    { _ASSERT( ( F_FE - 1 ) <= ( F_FB + 1 ) == false ); }
    { _ASSERT( ( F_FE - 1 ) <  ( F_FB + 1 ) == false ); }
    { _ASSERT( ( F_FB + 1 ) <= ( F_FB + 1 ) == true  ); }
    { _ASSERT( ( F_FB + 1 ) <  ( F_FB + 1 ) == false ); }
    { _ASSERT( ( F_FE - 1 ) <= ( F_FE - 1 ) == true  ); }
    { _ASSERT( ( F_FE - 1 ) <  ( F_FE - 1 ) == false ); }

    { _ASSERT( ( B_FB + 1 ) <= ( B_FE - 1 ) == true  ); }
    { _ASSERT( ( B_FB + 1 ) <  ( B_FE - 1 ) == true  ); }
    { _ASSERT( ( B_FE - 1 ) <= ( B_FB + 1 ) == false ); }
    { _ASSERT( ( B_FE - 1 ) <  ( B_FB + 1 ) == false ); }
    { _ASSERT( ( B_FB + 1 ) <= ( B_FB + 1 ) == true  ); }
    { _ASSERT( ( B_FB + 1 ) <  ( B_FB + 1 ) == false ); }
    { _ASSERT( ( B_FE - 1 ) <= ( B_FE - 1 ) == true  ); }
    { _ASSERT( ( B_FE - 1 ) <  ( B_FE - 1 ) == false ); }

    { _ASSERT( ( R_FB + 1 ) <= ( R_FE - 1 ) == true  ); }
    { _ASSERT( ( R_FB + 1 ) <  ( R_FE - 1 ) == true  ); }
    { _ASSERT( ( R_FE - 1 ) <= ( R_FB + 1 ) == false ); }
    { _ASSERT( ( R_FE - 1 ) <  ( R_FB + 1 ) == false ); }
    { _ASSERT( ( R_FB + 1 ) <= ( R_FB + 1 ) == true  ); }
    { _ASSERT( ( R_FB + 1 ) <  ( R_FB + 1 ) == false ); }
    { _ASSERT( ( R_FE - 1 ) <= ( R_FE - 1 ) == true  ); }
    { _ASSERT( ( R_FE - 1 ) <  ( R_FE - 1 ) == false ); }

    { _ASSERT( F_RB <= F_RE == true  ); }
    { _ASSERT( F_RB <  F_RE == true  ); }
    { _ASSERT( F_RE <= F_RB == false ); }
    { _ASSERT( F_RE <  F_RB == false ); }
    { _ASSERT( F_RB <= F_RB == true  ); }
    { _ASSERT( F_RB <  F_RB == false ); }
    { _ASSERT( F_RE <= F_RE == true  ); }
    { _ASSERT( F_RE <  F_RE == false ); }

    { _ASSERT( B_RB <= B_RE == true  ); }
    { _ASSERT( B_RB <  B_RE == true  ); }
    { _ASSERT( B_RE <= B_RB == false ); }
    { _ASSERT( B_RE <  B_RB == false ); }
    { _ASSERT( B_RB <= B_RB == true  ); }
    { _ASSERT( B_RB <  B_RB == false ); }
    { _ASSERT( B_RE <= B_RE == true  ); }
    { _ASSERT( B_RE <  B_RE == false ); }

    { _ASSERT( R_RB <= R_RE == true  ); }
    { _ASSERT( R_RB <  R_RE == true  ); }
    { _ASSERT( R_RE <= R_RB == false ); }
    { _ASSERT( R_RE <  R_RB == false ); }
    { _ASSERT( R_RB <= R_RB == true  ); }
    { _ASSERT( R_RB <  R_RB == false ); }
    { _ASSERT( R_RE <= R_RE == true  ); }
    { _ASSERT( R_RE <  R_RE == false ); }

    { _ASSERT( ( F_RB + 1 ) <= ( F_RE - 1 ) == true  ); }
    { _ASSERT( ( F_RB + 1 ) <  ( F_RE - 1 ) == true  ); }
    { _ASSERT( ( F_RE - 1 ) <= ( F_RB + 1 ) == false ); }
    { _ASSERT( ( F_RE - 1 ) <  ( F_RB + 1 ) == false ); }
    { _ASSERT( ( F_RB + 1 ) <= ( F_RB + 1 ) == true  ); }
    { _ASSERT( ( F_RB + 1 ) <  ( F_RB + 1 ) == false ); }
    { _ASSERT( ( F_RE - 1 ) <= ( F_RE - 1 ) == true  ); }
    { _ASSERT( ( F_RE - 1 ) <  ( F_RE - 1 ) == false ); }

    { _ASSERT( ( B_RB + 1 ) <= ( B_RE - 1 ) == true  ); }
    { _ASSERT( ( B_RB + 1 ) <  ( B_RE - 1 ) == true  ); }
    { _ASSERT( ( B_RE - 1 ) <= ( B_RB + 1 ) == false ); }
    { _ASSERT( ( B_RE - 1 ) <  ( B_RB + 1 ) == false ); }
    { _ASSERT( ( B_RB + 1 ) <= ( B_RB + 1 ) == true  ); }
    { _ASSERT( ( B_RB + 1 ) <  ( B_RB + 1 ) == false ); }
    { _ASSERT( ( B_RE - 1 ) <= ( B_RE - 1 ) == true  ); }
    { _ASSERT( ( B_RE - 1 ) <  ( B_RE - 1 ) == false ); }

    { _ASSERT( ( R_RB + 1 ) <= ( R_RE - 1 ) == true  ); }
    { _ASSERT( ( R_RB + 1 ) <  ( R_RE - 1 ) == true  ); }
    { _ASSERT( ( R_RE - 1 ) <= ( R_RB + 1 ) == false ); }
    { _ASSERT( ( R_RE - 1 ) <  ( R_RB + 1 ) == false ); }
    { _ASSERT( ( R_RB + 1 ) <= ( R_RB + 1 ) == true  ); }
    { _ASSERT( ( R_RB + 1 ) <  ( R_RB + 1 ) == false ); }
    { _ASSERT( ( R_RE - 1 ) <= ( R_RE - 1 ) == true  ); }
    { _ASSERT( ( R_RE - 1 ) <  ( R_RE - 1 ) == false ); }

    vector< int > v;
    Enumerable< int > enumerable( v );
}