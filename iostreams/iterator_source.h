/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#pragma once

#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/stream.hpp>


namespace {
namespace aux {


template< typename Iterator, typename Char >
std::streamsize read( Iterator& curr, const Iterator& end, Char* s, std::streamsize n )
{
     if( n > 0 && curr != end )
     {
          n = std::min( n, static_cast< std::streamsize >( std::distance( curr, end ) ) );
          std::copy( curr, std::next( curr, n ), s );
          std::advance( curr, n );
          return n;
     }
     return std::char_traits< Char >::eof();
}


} // namespace aux
} // namespace {unnamed}


template< typename Container >
class DirectIteratorSource : public boost::iostreams::source {
public:
     explicit DirectIteratorSource( const Container& c )
          : curr_{ std::begin( c ) }
          , end_{ std::end( c ) }
     {}

     std::streamsize read( char_type* s, std::streamsize n )
     {
          return aux::read( curr_, end_, s, n );
     }

private:
     typename Container::const_iterator curr_;
     const typename Container::const_iterator end_;
};


template< typename Container >
class ReverseIteratorSource : public boost::iostreams::source {
public:
     explicit ReverseIteratorSource( const Container& c )
          : curr_{ c.rbegin() }
          , end_{ c.rend() }
     {}

     std::streamsize read( char_type* s, std::streamsize n )
     {
          return aux::read( curr_, end_, s, n );
     }

private:
     typename Container::const_reverse_iterator curr_;
     const typename Container::const_reverse_iterator end_;
};


template< typename Container >
using DirectIteratorOstream = boost::iostreams::stream< DirectIteratorSource< Container > >;

template< typename Container >
using ReverseIteratorOstream = boost::iostreams::stream< ReverseIteratorSource< Container > >;
