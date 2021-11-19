/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#pragma once

#include <cstring>

#include <boost/assert.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/get.hpp>


namespace using_boost {
namespace iostreams {
namespace filters {


namespace {
namespace impl {


inline bool isVowel( const int c ) noexcept
{
     static constexpr auto vowels = "aeiouAEIOU";
     return std::strchr( vowels, c ) != nullptr;
}


} // namespace impl
} // namespace {unnamed}


namespace single_char {


struct Transparent : boost::iostreams::dual_use_filter
{
     template< typename Source >
     int get( Source& src )
     {
          return boost::iostreams::get( src );
     }

     template< typename Sink >
     bool put( Sink& snk, int c )
     {
          return boost::iostreams::put( snk, c );
     }
};


struct VowelRemover : boost::iostreams::input_filter
{
     template< typename Source >
     int get( Source& src )
     {
          while( true )
          {
               const auto c = boost::iostreams::get( src );
               if( !impl::isVowel( c ) )
               {
                    return c;
               }
          }
          BOOST_ASSERT( !"unreachable code" );
     }
};


} // namespace single_char
namespace multichar {


struct Transparent : boost::iostreams::multichar_dual_use_filter
{
     template< typename Source >
     std::streamsize read( Source& src, char* s, const std::streamsize n )
     {
          return boost::iostreams::read( src, s, n );
     }

     template< typename Sink >
     std::streamsize write( Sink& snk, const char* s, const std::streamsize n )
     {
          return boost::iostreams::write( snk, s, n );
     }
};


struct Counter : boost::iostreams::multichar_dual_use_filter
{
     explicit Counter( std::size_t n = 0 ) : chars_{ n } {}

     template< typename Source >
     std::streamsize read( Source& src, char* s, std::streamsize n )
     {
          const auto result = boost::iostreams::read( src, s, n );
          chars_ += result > 0 ? result : 0u;
          return result;
     }

     template< typename Sink >
     std::streamsize write( Sink& snk, const char* s, std::streamsize n )
     {
          const auto result = boost::iostreams::write( snk, s, n );
          chars_ += result > 0 ? result : 0u;
          return result;
     }

     std::size_t chars() const noexcept
     {
          return chars_;
     }

private:
     std::size_t chars_ = 0;
};


struct VowelRemover : boost::iostreams::multichar_input_filter
{
     template< typename Source >
     std::streamsize read( Source& src, char* s, std::streamsize n )
     {
          for( std::streamsize i = 0; i < n; ++i )
          {
               while( true )
               {
                    const auto c = boost::iostreams::get( src );
                    if( c == EOF )
                    {
                         return i ? i : EOF;
                    }
                    else if( c == boost::iostreams::WOULD_BLOCK )
                    {
                         return i;
                    }
                    else if( impl::isVowel( c ) )
                    {
                         continue;
                    }
                    s[ i ] = c;
                    break;
               }
          }
          return n;
     }
};


} // namespace multichar
} // namespace filters
} // namespace iostreams
} // namespace using_boost
