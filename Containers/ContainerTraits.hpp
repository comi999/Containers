#pragma once
#include <vector>
#include <stack>

using namespace std;

namespace ContainerTraits
{


    namespace
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
        struct GetIteratorImpl
        {
            using Iterator = decltype( begin( declval< Container& >() ) );
            using CIterator = decltype( end( declval< Container >() ) );
        };

        template < typename Container >
        struct GetIteratorImpl< Container, false >
        {
            using Iterator = void;
            using CIterator = void;
        };

        template < typename Iter, bool IsIterator = _Is_iterator_v< Iter > >
        struct GetIteratorTypeImpl
        {
            using Type = remove_reference_t< decltype( *declval< Iter& >() ) >;
        };

        template < typename Iter >
        struct GetIteratorTypeImpl< Iter, false >
        {
            using Type = void;
        };
    };

    template < typename Container >
    static constexpr bool IsSizeable = IsSizeableImpl< Container >::Value;

    template < typename Container >
    static constexpr bool IsIterable = IsIterableImpl< Container >::Value;

    template < typename Container >
    using GetIterator = typename GetIteratorImpl< Container >::Iterator;

    template < typename Container >
    using GetCIterator = typename GetIteratorImpl< Container >::Iterator;

    template < typename Iter >
    using GetIteratorType = typename GetIteratorTypeImpl< Iter >::Type;

    /*template < typename Iter, typename T >
    using EnableIfIterType = enable_if_t< is_same_v< GetIterType< Iter >, T >, void >;

    template < typename Iter, typename T >
    using DisableIfIterType = enable_if_t< !is_same_v< GetIterType< Iter >, T >, void >;

    template < typename Container, typename T >
    using EnableIfContainerWithIterType = enable_if_t< is_same_v< GetIterType< Container >, T >, void >;

    template < typename Iter, typename T >
    using DisableIfIterType = enable_if_t< !is_same_v< GetIterType< Iter >, T >, void >;*/

    using iter = GetIterator< vector< int > >;

    iter test;

    /*template < typename Container >
    class HasIter
    {
        template < typename U > static char test( decltype( begin( declval< U& >() ) != end( declval< U& >() ) ), decltype( ++declval< decltype( begin( declval< U& >() ) )& >() ), decltype( *begin( declval< U& >() ) ), true_type {} );
        template < typename U > static long test( ... );
    public:
        static constexpr bool Value = sizeof( test< Container >( 0 ) ) > sizeof( char );
    };

    static constexpr bool val = HasIter< int >::Value;

    template < typename Container >
    class GetIterImpl
    {
        
    };

    template < typename Container >
    class IsIterableImpl
    {
        template < typename U > static char test( decltype( begin( declval< U& >() ) != end( declval< U& >() ), decltype( ++declval< decltype( begin( declval< U& >() ) )& >() ), decltype( *begin( declval< U& >() ) ) ) );
        template < typename U > static long test( ... );
    public:
        static constexpr bool Value = sizeof( test< Container >( 0 ) ) == sizeof( char );
    };
    
    
    using Type = decltype( begin( declval< vector< int >& >() ) );*/

    //template < typename Iter, typename T >
    //static constexpr bool IsCorrectIter = _Is_iterator_v< Iter > && is_same_v< typename iterator_traits< Iter >::value_type, T >;

    //template < typename Container >
    //static constexpr bool IsIterable = HasMember_begin< Container >::Value && HasMember_end< Container >::Value;

    //template < typename Container >
    //static constexpr bool IsSizeable = HasMember_size< Container >::Value;

    ////template < typename Container >
    ////using EnableIfContainer = enable_if_t< IsIterable< Container >&& IsSizeable< Container >, void >;
    ////
    ////template < typename Iter >
    ////using EnableIfCorrectIter = enable_if_t< _Is_iterator_v< Iter >&& is_same_v< typename iterator_traits< Iter >::value_type, T >, void >;

    //template < typename Container, bool IsContainer = IsIterable< Container >&& IsSizeable< Container > >
    //struct GetIteratorImpl
    //{
    //    using Type = decltype( Container::begin );
    //};

    //template < typename Container >
    //struct GetIteratorImpl< Container, false >
    //{
    //    using Type = void;
    //};

    //using iter = GetIteratorImpl< vector< int > >::Type;
    //static constexpr bool val = is_iterable< vector< int > >::value;

};

