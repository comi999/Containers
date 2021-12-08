#include <iostream>
#include <set>
#include "Array.hpp"
#include "Enumerable.hpp"
#include <queue>
#include <list>
#include "Delegate.hpp"

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
    
    Enumerable< int > e( a.Begin(), a.End() );
    auto iterBegin = e.Begin();
    auto iterBeginVal = *iterBegin;
    iterBeginVal = *e.Begin();
    auto iterEnd = e.End();
    auto iterEndVal = *( e.End() - 1 );

    auto res3 = *iterBegin;
    auto res2 = *( iterBegin += 1 );

    auto res0 = ( iterBegin + 5 );
    auto res1 = ( iterEnd - 1 );
    auto val0 = *res0;
    auto val1 = *res1;

    auto dist = res1 - res0;
}