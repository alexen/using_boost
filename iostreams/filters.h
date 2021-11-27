/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#pragma once

#include <set>

#include <boost/assert.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/get.hpp>
#include <boost/utility/string_view.hpp>


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
               if( not ignored( c ) )
               {
                    return c;
               }
          }
          BOOST_ASSERT( !"unreachable code" );
     }

     template< typename Sink >
     bool put( Sink& snk, const int c )
     {
          if( ignored( c ) )
          {
               return true;
          }
          return boost::iostreams::put( snk, c );
     }
private:
     bool ignored( int c ) const noexcept
     {
          return chars_.find( c ) != boost::string_view::npos;
     }
     boost::string_view chars_;
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
     std::streamsize read( Source& src, char* out, const std::streamsize n )
     {
          const auto result = boost::iostreams::read( src, out, n );
          if( result > 0 )
          {
               const auto end = std::remove_if(
                    out,
                    out + result,
                    [ this ]( const char c ){ return ignored( c ); }
                    );
               return std::distance( out, end );
          }
          return result;
     }

     template< typename Sink >
     std::streamsize write( Sink& snk, const char* s, const std::streamsize n )
     {
          std::streamsize i = 0;
          for( ; i < n; ++i )
          {
               const auto c = s[ i ];
               if( ignored( c ) )
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
     bool ignored( const int c ) const noexcept
     {
          return chars_.find( c ) != boost::string_view::npos;
     }
     boost::string_view chars_;
};


struct StreamInterrupter : boost::iostreams::multichar_dual_use_filter
{
     template< typename Iter >
     explicit StreamInterrupter( Iter begin, Iter end )
          : chars_{ begin, end }
     {}

     explicit StreamInterrupter( boost::string_view cc )
          : StreamInterrupter{ cc.cbegin(), cc.cend() }
     {}

     template< typename Source >
     std::streamsize read( Source& src, char* out, std::streamsize n )
     {
          if( !eos_ && (n = boost::iostreams::read( src, out, n )) > 0 )
          {
               const auto end = out + n;
               const auto endstrm = std::find_if( out, end,
                    [ this ]( const char c ){ return eos( c ); } );
               eos_ = endstrm != end;
               return std::distance( out, endstrm );
          }
          return eos_ ? EOF : n;
     }

     template< typename Sink >
     std::streamsize write( Sink& snk, const char* in, const std::streamsize n )
     {
          if( !eos_ )
          {
               const auto end = in + n;
               const auto endstrm = std::find_if(
                    in
                    , end
                    , [ this ]( const char c ){ return eos( c ); }
                    );
               eos_ = endstrm != end;
               boost::iostreams::write( snk, in, std::distance( in, endstrm ) );
          }
          return n;
     }

     template< typename Device >
     void close( Device&, std::ios::openmode )
     {
          eos_ = false;
     }
private:
     bool eos( const int c ) const noexcept
     {
          return chars_.count( c ) > 0;
     }
     std::set< char > chars_;
     bool eos_ = false;
};


} // namespace multichar
} // namespace filters
} // namespace iostreams
} // namespace using_boost
