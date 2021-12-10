#include <iostream>
#include <set>
#include "Array.hpp"
#include "Enumerable.hpp"
#include <queue>
#include <list>
#include "Delegate.hpp"
#include <map>

using namespace std;

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

struct Num
{
    int num = 0;
    Num( int _num ) : num( _num ) {  }
    
    Num operator-( const Num& _num )
    {
        return Num( num - _num.num );
    }
};

int main()
{
    Num num0 = 10;
    Num num1 = 3;
    Num num2 = num0 - num1;

    Array< int, 7 > a = { 2, 3, 1, 10, 11, 12, 24 };
    Array< int, 7 > b = { 2, 4, 5, 1, 10, 11, 24 };
    Array< int > c = { 4, 5, 6, 7 };

    Cont< int > cont;

    map< int, int > m;

    for ( int i = 0; i < 10; ++i )
    {
        m.emplace( make_pair( i, i ) );
    }

    

    Enumerable< pair< const int, int > > e( m.begin(), m.end() );

    auto beg = e.Begin();
    auto end = e.End();
    auto rbeg = e.RBegin();
    auto rend = e.REnd();

    for ( rend -= 1; rbeg != rend; --rend )
    {
        cout << (*rend ).first << ":" << (*rend ).second << endl;
        cout << rend - rbeg << endl;
    }

    auto distance = rend - rbeg;

    auto rval0 = *rbeg;
    auto rval1 = *rend;

    /*using RIt = Enumerable< int >::RIterator;
    auto iter = RIt( e.Begin() ) - 1;
    auto iterE = RIt( e.End() );
    auto iterVal = *iter;
    auto iterEVal = *iterE;

    for ( ; iter != iterE; --iter )
    {
        cout << *iter << endl;
    }*/
}