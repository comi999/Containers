#pragma once
#include <type_traits>

using namespace std;

namespace ContainerTraits
{
    namespace // Private namespace
    {
        template < typename Container >
        struct HasSize
        {
        private:
            template < typename U > static char test( decltype( &U::Size ) );
            template < typename U > static char test( decltype( &U::size ) );
            template < typename U > static long test( ... );
        public:
            static constexpr bool Value = sizeof( test< Container >( 0 ) ) == sizeof( char );
        };

        template < typename Container >
        struct GetSizeImpl
        {
        private:
            template < typename U > 
            static size_t test( decltype( &U::size ), const Container& a_Container )
            {
                return a_Container.size();
            }

            template < typename U >
            static size_t test( decltype( &U::Size ), const Container& a_Container )
            {
                return a_Container.Size();
            }
        public:
            static size_t GetSize( const Container& a_Container )
            {
                return test< Container >( 0, a_Container );
            }
        };

        template < typename Container >
        struct HasBeginImpl
        {
        private:
            template < typename U > static char test( decltype( begin( std::declval< U >() ) )* );
            template < typename U > static char test( decltype( &U::Begin ) );
            template < typename U > static long test( ... );
        public:
            static constexpr bool Value = sizeof( test< Container >( 0 ) ) == sizeof( char );
        };

        template < typename Container >
        struct HasEndImpl
        {
        private:
            template < typename U > static char test( decltype( end( std::declval< U >() ) )* );
            template < typename U > static char test( decltype( &U::End ) );
            template < typename U > static long test( ... );
        public:
            static constexpr bool Value = sizeof( test< Container >( 0 ) ) == sizeof( char );
        };

        template < typename Container >
        struct GetBeginImpl
        {
        private:

            template < typename U >
            static auto test( decltype( begin( declval< U >() ) )*, Container& a_Container )
            {
                return a_Container.begin();
            }

            template < typename U >
            static auto test( decltype( begin( declval< U >() ) )*, const Container& a_Container )
            {
                return a_Container.begin();
            }

            template < typename U >
            static auto test( decltype( &U::Begin ), Container& a_Container )
            {
                return a_Container.Begin();
            }

            template < typename U >
            static auto test( decltype( &U::Begin ), const Container& a_Container )
            {
                return a_Container.Begin();
            }

        public:

            static auto GetBegin( Container& a_Container )
            {
                return test< Container >( 0, a_Container );
            }

            static auto GetBegin( const Container& a_Container )
            {
                return test< Container >( 0, a_Container );
            }
        };

        template < typename Container >
        struct GetEndImpl
        {
        private:

            template < typename U >
            static auto test( decltype( end( declval< U >() ) )*, const Container& a_Container )
            {
                return a_Container.end();
            }

            template < typename U >
            static auto test( decltype( &U::End ), const Container& a_Container )
            {
                return a_Container.End();
            }

        public:
            static auto GetEnd( const Container& a_Container )
            {
                return test< Container >( 0, a_Container );
            }
        };

        template < typename Container, bool IsIterable = HasBeginImpl< Container >::Value && HasEndImpl< Container >::Value >
        struct GetIterImpl
        {
            using Iterator = decltype( GetBeginImpl< Container >::GetBegin( declval< Container& >() ) );
            using CIterator = decltype( GetBeginImpl< Container >::GetBegin( declval< Container >() ) );
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
    static constexpr bool IsSizeable = HasSize< Container >::Value;

    template < typename Container, typename = enable_if_t< IsSizeable< Container >, void > >
    static size_t Size( const Container& a_Container )
    {
        return GetSizeImpl< Container >::GetSize( a_Container );
    }

    template < typename Container >
    static constexpr bool HasBegin = HasBeginImpl< Container >::Value;

    template < typename Container >
    static constexpr bool HasEnd = HasEndImpl< Container >::Value;

    template < typename Container >
    static constexpr bool IsIterable = HasBegin< Container > && HasEnd< Container >;

    template < typename Container, typename = enable_if_t< HasBeginImpl< Container >::Value, void > >
    static auto Begin( Container& a_Container )
    {
        return GetBeginImpl< Container >::GetBegin( a_Container );
    }

    template < typename Container, typename = enable_if_t< HasBeginImpl< Container >::Value, void > >
    static auto Begin( const Container& a_Container )
    {
        return GetBeginImpl< Container >::GetBegin( a_Container );
    }

    template < typename Container, typename = enable_if_t< HasEndImpl< Container >::Value, void > >
    static auto End( Container& a_Container )
    {
        return GetEndImpl< Container >::GetEnd( a_Container );
    }

    template < typename Container, typename = enable_if_t< HasEndImpl< Container >::Value, void > >
    static auto End( const Container& a_Container )
    {
        return GetEndImpl< Container >::GetEnd( a_Container );
    }

    template < typename Container >
    using GetIter = typename GetIterImpl< Container >::Iterator;

    template < typename Container >
    using GetCIter = typename GetIterImpl< Container >::CIterator;

    template < typename Iter >
    using GetIterType = typename GetIterTypeImpl< Iter >::Type;

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
    using DisableIfIterable = enable_if_t< !IsIterable< Container >, void >;

    template < typename Container, typename Type >
    using EnableIfContainer = enable_if_t< IsSizeable< Container > && IsIterable< Container > && is_same_v< GetIterType< GetIter< Container > > >, Type >;

    template < typename Container, typename Type >
    using DisableIfContainer = enable_if_t< !IsSizeable< Container > || !IsIterable< Container > || !is_same_v< GetIterType< GetIter< Container > > >, Type >;

};

#include "Enumerable.hpp"
static constexpr bool val0 = ContainerTraits::HasBeginImpl< Enumerable< int > >::Value;
using t = ContainerTraits::GetIterImpl< Enumerable< int > >::; //ContainerTraits::EnableIfContainer< Enumerable< int >, int >;
using i = ContainerTraits::GetIterType< t >;
static constexpr bool val = ContainerTraits::HasBeginImpl< Enumerable< int > >::Value;