#pragma once
#include <vector>
#include <stack>
#include <array>

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

        template < typename Container, typename = void >
        struct IsIterableImpl
        {
            static constexpr bool Value = false;
        };

        template < typename Container >
        struct IsIterableImpl< Container, void_t< decltype( begin( declval< Container >() ) ), decltype( end( declval< Container >() ) ) > >
        {
            static constexpr bool Value = true;
        };

        /*template < typename Container, typename = void >
        struct HasBeginImpl
        {
            static constexpr bool Value = false;
        };

        template < typename Container >
        struct HasBeginImpl < Container, void_t< decltype( &Container::Begin ), decltype( &Container::End ) > >
        {
            static constexpr bool Value = true;
        };*/

        template < typename Container >
        struct HasBeginImpl
        {
        private:
            template < typename U > static char test( decltype( &U::Begin ) );
            template < typename U > static char test( decltype( declval< U >().begin() ) );
            template < typename U > static long test( ... );
        public:
            static constexpr bool Value = sizeof( test< Container >( 0 ) ) == sizeof( char );
        };

        //template < typename Container >
        //struct GetBeginImpl
        //{
        //private:
        //    template < typename U >
        //    static auto test( decltype( &U::Begin ), const Container& a_Container )
        //    {
        //        return a_Container.Begin();
        //    }

        //    template < typename U >
        //    static auto test( decltype( &U::begin ), const Container& a_Container )
        //    {
        //        return a_Container.begin();
        //    }
        //public:
        //    static auto GetBegin( const Container& a_Container )
        //    {
        //        return test< Container >( 0, a_Container );
        //    }
        //};

        //template < typename Container >
        //struct GetEndImpl
        //{
        //private:
        //    template < typename U >
        //    static auto test( decltype( &U::End ), const Container& a_Container )
        //    {
        //        return a_Container.End();
        //    }

        //    template < typename U >
        //    static auto test( decltype( &U::end ), const Container& a_Container )
        //    {
        //        return a_Container.end();
        //    }
        //public:
        //    static auto GetEnd( const Container& a_Container )
        //    {
        //        return test< Container >( 0, a_Container );
        //    }
        //};

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
    static constexpr bool IsSizeable = HasSize< Container >::Value;

    template < typename Container, typename = enable_if_t< IsSizeable< Container >, void > >
    static size_t Size( const Container& a_Container )
    {
        return GetSizeImpl< Container >::GetSize( a_Container );
    }

    /*template < typename Container, typename = enable_if_t< HasBegin< Container >::Value, void > >
    static auto Begin( const Container& a_Container )
    {
        return GetBeginImpl< Container >::GetBegin( a_Container );
    }

    template < typename Container, typename = enable_if_t< HasEnd< Container >::Value, void > >
    static auto End( const Container& a_Container )
    {
        return GetEndImpl< Container >::GetEnd( a_Container );
    }*/

    template < typename Container >
    static constexpr bool IsIterable = IsIterableImpl< Container >::Value;

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

};

struct cont
{
    void Begin()
    {

    }
};

static constexpr bool val0 = ContainerTraits::HasBeginImpl< array< int, 10 > >::Value;
static constexpr bool val1 = ContainerTraits::HasBeginImpl< cont >::Value;
static constexpr bool val2 = ContainerTraits::HasBeginImpl< int >::Value;
using t = ContainerTraits::GetIterType< array< int, 10 >::iterator >;
