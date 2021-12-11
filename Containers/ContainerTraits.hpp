#pragma once
#include <vector>
#include <stack>

using namespace std;

namespace ContainerTraits
{
    namespace // Private namespace
    {
        template < typename Container >
        struct IsSizeableImpl
        {
        private:
            template < typename U > static char test( decltype( U::size ) );
            template < typename U > static long test( ... );
        public:
            static constexpr bool Value = sizeof( test< Container >( 0 ) ) == sizeof( char );
        };

        template <typename Container, typename = void>
        struct IsIterableImpl
        {
            static constexpr bool Value = false;
        };

        template < typename Container >
        struct IsIterableImpl< Container, void_t< decltype( begin( declval< Container >() ) ), decltype( end( declval< Container >() ) ) > >
        {
            static constexpr bool Value = true;
        };

        template < typename Container, bool IsIterable = IsIterableImpl< Container >::Value >
        struct GetIterImpl
        {
            using Iterator = decltype( begin( declval< Container& >() ) );
            using CIterator = decltype( end( declval< Container >() ) );
        };

        template < typename Container >
        struct GetIterImpl< Container, false >
        {
            using Iterator = void;
            using CIterator = void;
        };

        template < typename Iter, bool IsIterator = _Is_iterator_v< Iter > >
        struct GetIterTypeImpl
        {
            using Type = remove_reference_t< typename iterator_traits< Iter >::reference >;
        };

        template < typename Iter >
        struct GetIterTypeImpl< Iter, false >
        {
            using Type = void;
        };
    }; // End private namespace

    template < typename Container >
    static constexpr bool IsSizeable = IsSizeableImpl< Container >::Value;

    template < typename Container >
    static constexpr bool IsIterable = IsIterableImpl< Container >::Value;

    template < typename Container >
    using GetIter = typename GetIterImpl< Container >::Iterator;

    template < typename Container >
    using GetCIter = typename GetIterImpl< Container >::CIterator;

    template < typename Iter >
    using GetIterType = typename GetIterTypeImpl< Iter >::Type;/*

    template < typename Iter, typename Type >
    using EnableIfIterType = enable_if_t< is_same_v< GetIterType< Iter >, Type >, void >;

    template < typename Iter, typename Type >
    using DisableIfIterType = enable_if_t< !is_same_v< GetIterType< Iter >, Type >, void >;

    template < typename Container >
    using EnableIfSizeable = enable_if_t< IsSizeable< Container >, void >;

    template < typename Container >
    using DisableIfSizeable = enable_if_t< !IsSizeable< Container >, void >;

    template < typename Container >
    using EnableIfIterable = enable_if_t< IsIterable< Container >, void >;

    template < typename Container >
    using DisableIfIterable = enable_if_t< !IsIterable< Container >, void >;*/

};

