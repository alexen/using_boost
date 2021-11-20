/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#pragma once

#include <cstring>
#include <set>

#include <boost/assert.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/get.hpp>


namespace using_boost {
namespace iostreams {
namespace filters {
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


struct CharRemover : boost::iostreams::dual_use_filter
{
     template< typename ...Args >
     explicit CharRemover( Args&& ...args )
          : chars_{ std::forward< Args >( args )... }
     {}

     template< typename Source >
     int get( Source& src )
     {
          while( true )
          {
               const auto c = boost::iostreams::get( src );
               if( !chars_.count( c ) )
               {
                    return c;
               }
          }
          BOOST_ASSERT( !"unreachable code" );
     }

     template< typename Sink >
     bool put( Sink& snk, const int c )
     {
          if( chars_.count( c ) )
          {
               return true;
          }
          return boost::iostreams::put( snk, c );
     }
private:
     std::set< char > chars_;
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


struct CharRemover : boost::iostreams::multichar_dual_use_filter
{
     template< typename ...Args >
     explicit CharRemover( Args&& ...args )
          : chars_{ std::forward< Args >( args )... }
     {}

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
                    else if( chars_.count( c ) )
                    {
                         continue;
                    }
                    s[ i ] = c;
                    break;
               }
          }
          return n;
     }

     template< typename Sink >
     std::streamsize write( Sink& snk, const char* s, const std::streamsize n )
     {
          std::streamsize i = 0;
          for( ; i < n; ++i )
          {
               const auto c = s[ i ];
               if( chars_.count( c ) )
               {
                    continue;
               }
               if( !boost::iostreams::put( snk, c ) )
               {
                    break;
               }
          }
          return i;
     }
private:
     std::set< char > chars_;
};


} // namespace multichar
} // namespace filters
} // namespace iostreams
} // namespace using_boost
