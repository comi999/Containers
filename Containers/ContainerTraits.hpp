#pragma once
#include <vector>
#include <stack>

using namespace std;

namespace ContainerTraits
{
    namespace
    {
        template < typename Container >
        auto IsIterableImpl( int )
            -> decltype ( begin( declval< Container& >() ) != end( declval< Container& >() ),
                          ++declval< decltype( begin( declval< Container& >() ) )& >(),
                          void( *begin( declval< Container& >() ) ),
                          std::true_type {} );

        template <typename T>
        std::false_type isIterableImpl( ... );

        template < typename Container >
        using IsIterable = decltype( IsIterableImpl< Container >( 0 ) );


        template < typename Container >
        class IsIterableImpl
        {

        };
    }
    
    
    



    template < typename Iter, typename T >
    static constexpr bool IsCorrectIter = _Is_iterator_v< Iter > && is_same_v< typename iterator_traits< Iter >::value_type, T >;

    template < typename Container >
    static constexpr bool IsIterable = HasMember_begin< Container >::Value && HasMember_end< Container >::Value;

    template < typename Container >
    static constexpr bool IsSizeable = HasMember_size< Container >::Value;

    //template < typename Container >
    //using EnableIfContainer = enable_if_t< IsIterable< Container >&& IsSizeable< Container >, void >;
    //
    //template < typename Iter >
    //using EnableIfCorrectIter = enable_if_t< _Is_iterator_v< Iter >&& is_same_v< typename iterator_traits< Iter >::value_type, T >, void >;

    template < typename Container, bool IsContainer = IsIterable< Container >&& IsSizeable< Container > >
    struct GetIteratorImpl
    {
        using Type = decltype( Container::begin );
    };

    template < typename Container >
    struct GetIteratorImpl< Container, false >
    {
        using Type = void;
    };

    using iter = GetIteratorImpl< vector< int > >::Type;
    static constexpr bool val = is_iterable< vector< int > >::value;

}