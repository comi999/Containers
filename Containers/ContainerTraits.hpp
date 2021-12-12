#pragma once
#include <type_traits>
#include <iterator>

using namespace std;

namespace ContainerTraits
{
    namespace // Private namespace
    {
        template < typename Container, bool IsIterable >
        struct GetIterImpl;

        template < typename Container >
        struct HasSize
        {
        private:
            template < typename U > static char test( decltype( declval< U >().Size() )* );
            template < typename U > static char test( decltype( declval< U >().size() )* );
            template < typename U > static long test( ... );
        public:
            static constexpr bool Value = sizeof( test< Container >( 0 ) ) == sizeof( char );
        };

        template < typename Container >
        struct GetSizeImpl
        {
        private:
            template < typename U > 
            static size_t test( decltype( declval< U >().size() )*, const Container& a_Container )
            {
                return a_Container.size();
            }

            template < typename U >
            static size_t test( decltype( declval< U >().Size() )*, const Container& a_Container )
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
            template < typename U > static char test( decltype( declval< U >().begin() )* );
            template < typename U > static char test( decltype( declval< U >().Begin() )* );
            template < typename U > static long test( ... );
        public:
            static constexpr bool Value = sizeof( test< Container >( 0 ) ) == sizeof( char );
        };

        template < typename Container >
        struct HasCBeginImpl
        {
        private:
            template < typename U > static char test( decltype( declval< U >().cbegin() )* );
            template < typename U > static char test( decltype( declval< U >().CBegin() )* );
            template < typename U > static long test( ... );
        public:
            static constexpr bool Value = sizeof( test< Container >( 0 ) ) == sizeof( char );
        };

        template < typename Container >
        struct HasRBeginImpl
        {
        private:
            template < typename U > static char test( decltype( declval< U >().rbegin() )* );
            template < typename U > static char test( decltype( declval< U >().RBegin() )* );
            template < typename U > static long test( ... );
        public:
            static constexpr bool Value = sizeof( test< Container >( 0 ) ) == sizeof( char );
        };

        template < typename Container >
        struct HasCRBeginImpl
        {
        private:
            template < typename U > static char test( decltype( declval< U >().crbegin() )* );
            template < typename U > static char test( decltype( declval< U >().CRBegin() )* );
            template < typename U > static long test( ... );
        public:
            static constexpr bool Value = sizeof( test< Container >( 0 ) ) == sizeof( char );
        };

        template < typename Container >
        struct GetBeginImpl
        {
        private:

            template < typename U >
            static auto test( decltype( declval< U >().begin() )*, Container& a_Container )
            {
                return a_Container.begin();
            }

            template < typename U >
            static auto test( decltype( declval< U >().begin() )*, const Container& a_Container )
            {
                return a_Container.begin();
            }

            template < typename U >
            static auto test( decltype( declval< U >().Begin() )*, Container& a_Container )
            {
                return a_Container.Begin();
            }

            template < typename U >
            static auto test( decltype( declval< U >().Begin() )*, const Container& a_Container )
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
                return test< const Container >( 0, a_Container );
            }

        };

        template < typename Container >
        struct GetRBeginImpl
        {
        private:

            template < typename U >
            static auto test( decltype( declval< U >().rbegin() )*, Container& a_Container )
            {
                return a_Container.rbegin();
            }

            template < typename U >
            static auto test( decltype( declval< U >().rbegin() )*, const Container& a_Container )
            {
                return a_Container.rbegin();
            }

            template < typename U >
            static auto test( decltype( declval< U >().RBegin() )*, Container& a_Container )
            {
                return a_Container.RBegin();
            }

            template < typename U >
            static auto test( decltype( declval< U >().RBegin() )*, const Container& a_Container )
            {
                return a_Container.RBegin();
            }

        public:

            static auto GetRBegin( Container& a_Container )
            {
                return test< Container >( 0, a_Container );
            }

            static auto GetRBegin( const Container& a_Container )
            {
                return test< const Container >( 0, a_Container );
            }

        };

        template < typename Container >
        struct HasEndImpl
        {
        private:
            template < typename U > static char test( decltype( declval< U >().end() )* );
            template < typename U > static char test( decltype( declval< U >().End() )* );
            template < typename U > static long test( ... );
        public:
            static constexpr bool Value = sizeof( test< Container >( 0 ) ) == sizeof( char );
        };

        template < typename Container >
        struct HasCEndImpl
        {
        private:
            template < typename U > static char test( decltype( declval< U >().cend() )* );
            template < typename U > static char test( decltype( declval< U >().CEnd() )* );
            template < typename U > static long test( ... );
        public:
            static constexpr bool Value = sizeof( test< Container >( 0 ) ) == sizeof( char );
        };

        template < typename Container >
        struct HasREndImpl
        {
        private:
            template < typename U > static char test( decltype( declval< U >().rend() )* );
            template < typename U > static char test( decltype( declval< U >().REnd() )* );
            template < typename U > static long test( ... );
        public:
            static constexpr bool Value = sizeof( test< Container >( 0 ) ) == sizeof( char );
        };

        template < typename Container >
        struct HasCREndImpl
        {
        private:
            template < typename U > static char test( decltype( declval< U >().crend() )* );
            template < typename U > static char test( decltype( declval< U >().CREnd() )* );
            template < typename U > static long test( ... );
        public:
            static constexpr bool Value = sizeof( test< Container >( 0 ) ) == sizeof( char );
        };

        template < typename Container >
        struct GetEndImpl
        {
        private:

            template < typename U >
            static auto test( decltype( declval< U >().end() )*, Container& a_Container )
            {
                return a_Container.end();
            }

            template < typename U >
            static auto test( decltype( declval< U >().end() )*, const Container& a_Container )
            {
                return a_Container.end();
            }

            template < typename U >
            static auto test( decltype( declval< U >().End() )*, Container& a_Container )
            {
                return a_Container.End();
            }

            template < typename U >
            static auto test( decltype( declval< U >().End() )*, const Container& a_Container )
            {
                return a_Container.End();
            }

        public:

            static auto GetEnd( Container& a_Container )
            {
                return test< Container >( 0, a_Container );
            }

            static auto GetEnd( const Container& a_Container )
            {
                return test< const Container >( 0, a_Container );
            }

        };

        template < typename Container >
        struct GetREndImpl
        {
        private:

            template < typename U >
            static auto test( decltype( declval< U >().rend() )*, Container& a_Container )
            {
                return a_Container.rend();
            }

            template < typename U >
            static auto test( decltype( declval< U >().rend() )*, const Container& a_Container )
            {
                return a_Container.rend();
            }

            template < typename U >
            static auto test( decltype( declval< U >().REnd() )*, Container& a_Container )
            {
                return a_Container.REnd();
            }

            template < typename U >
            static auto test( decltype( declval< U >().REnd() )*, const Container& a_Container )
            {
                return a_Container.REnd();
            }

        public:

            static auto GetREnd( Container& a_Container )
            {
                return test< Container >( 0, a_Container );
            }

            static auto GetREnd( const Container& a_Container )
            {
                return test< const Container >( 0, a_Container );
            }

        };

        template < typename Container, bool IsIterable = HasBeginImpl< Container >::Value && HasEndImpl< Container >::Value >
        struct GetIterImpl
        {
            using Type = decltype( GetBeginImpl< Container >::GetBegin( declval< Container& >() ) );
        };

        template < typename Container, bool IsIterable = HasCBeginImpl< Container >::Value && HasCEndImpl< Container >::Value >
        struct GetCIterImpl
        {
            using Type = decltype( GetBeginImpl< Container >::GetBegin( declval< const Container& >() ) );
        };

        template < typename Container, bool IsIterable = HasRBeginImpl< Container >::Value && HasREndImpl< Container >::Value >
        struct GetRIterImpl
        {
            using Type = decltype( GetRBeginImpl< Container >::GetRBegin( declval< Container& >() ) );
        };

        template < typename Container, bool IsIterable = HasCRBeginImpl< Container >::Value && HasCREndImpl< Container >::Value >
        struct GetCRIterImpl
        {
            using Type = decltype( GetRBeginImpl< Container >::GetRBegin( declval< const Container& >() ) );
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
    static constexpr bool HasCBegin = HasCBeginImpl< Container >::Value;

    template < typename Container >
    static constexpr bool HasRBegin = HasRBeginImpl< Container >::Value;

    template < typename Container >
    static constexpr bool HasCRBegin = HasCRBeginImpl< Container >::Value;

    template < typename Container >
    static constexpr bool HasEnd = HasEndImpl< Container >::Value;

    template < typename Container >
    static constexpr bool HasCEnd = HasCEndImpl< Container >::Value;

    template < typename Container >
    static constexpr bool HasREnd = HasREndImpl< Container >::Value;

    template < typename Container >
    static constexpr bool HasCREnd = HasCREndImpl< Container >::Value;

    template < typename Container >
    static constexpr bool IsIterable = HasBegin< Container > && HasEnd< Container >;

    template < typename Container >
    static auto Begin( Container& a_Container )
    {
        return GetBeginImpl< Container >::GetBegin( a_Container );
    }

    template < typename Container >
    static auto Begin( const Container& a_Container )
    {
        return GetBeginImpl< Container >::GetBegin( a_Container );
    }

    template < typename Container >
    static auto End( Container& a_Container )
    {
        return GetEndImpl< Container >::GetEnd( a_Container );
    }

    template < typename Container >
    static auto End( const Container& a_Container )
    {
        return GetEndImpl< Container >::GetEnd( a_Container );
    }

    template < typename Container >
    static auto RBegin( Container& a_Container )
    {
        return GetRBeginImpl< Container >::GetRBegin( a_Container );
    }

    template < typename Container >
    static auto RBegin( const Container& a_Container )
    {
        return GetRBeginImpl< Container >::GetRBegin( a_Container );
    }

    template < typename Container >
    static auto REnd( Container& a_Container )
    {
        return GetREndImpl< Container >::GetREnd( a_Container );
    }

    template < typename Container >
    static auto REnd( const Container& a_Container )
    {
        return GetREndImpl< Container >::GetREnd( a_Container );
    }

    template < typename Container >
    using GetIter = typename GetIterImpl< Container >::Type;

    template < typename Container >
    using GetCIter = typename GetCIterImpl< Container >::Type;

    template < typename Container >
    using GetRIter = typename GetRIterImpl< Container >::Type;

    template < typename Container >
    using GetCRIter = typename GetCRIterImpl< Container >::Type;

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
    using EnableIfContainer = enable_if_t< IsSizeable< Container > && IsIterable< Container > && is_same_v< GetIterType< GetIter< Container > >, Type >, void >;

    template < typename Container, typename Type >
    using DisableIfContainer = enable_if_t< !IsSizeable< Container > || !IsIterable< Container > || !is_same_v< GetIterType< GetIter< Container > >, Type >, void >;

};