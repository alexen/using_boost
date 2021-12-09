/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#pragma once

#include <set>
#include <map>
#include <iomanip>

#include <boost/assert.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/filter/symmetric.hpp>
#include <boost/iostreams/get.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/transform_width.hpp>
#include <boost/range/irange.hpp>


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
     template< typename Iter >
     explicit CharRemover( Iter begin, Iter end )
          : chars_{ begin, end }
     {}

     explicit CharRemover( boost::string_view cc )
          : CharRemover{ cc.cbegin(), cc.cend() }
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
     bool ignored( const int c ) const noexcept
     {
          return chars_.count( c );
     }
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
     template< typename Iter >
     explicit CharRemover( Iter begin, Iter end )
          : chars_{ begin, end }
     {}

     explicit CharRemover( boost::string_view cc )
          : CharRemover{ cc.cbegin(), cc.cend() }
     {}

     template< typename Source >
     std::streamsize read( Source& src, char* out, std::streamsize n )
     {
          if( (n = boost::iostreams::read( src, out, n )) > 0 )
          {
               const auto end = std::remove_if(
                    out,
                    out + n,
                    [ this ]( const char c ){ return ignored( c ); }
                    );
               return std::distance( out, end );
          }
          return n;
     }

     template< typename Sink >
     std::streamsize write( Sink& snk, const char* s, const std::streamsize n )
     {
          for( std::streamsize i = 0; i < n; ++i )
          {
               const auto c = s[ i ];
               if( !ignored( c ) )
               {
                    if( !boost::iostreams::put( snk, c ) )
                    {
                         return i;
                    }
               }
          }
          return n;
     }
private:
     bool ignored( const int c ) const noexcept
     {
          return chars_.count( c );
     }
     std::set< char > chars_;
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
               if( boost::iostreams::write( snk, in, std::distance( in, endstrm ) ) == EOF )
               {
                    return EOF;
               }
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


struct CharMultiplier : boost::iostreams::multichar_dual_use_filter
{
     explicit CharMultiplier( int n ) : n_{ n } {}

     template< typename Source >
     std::streamsize read( Source& src, char* dst, const std::streamsize n )
     {
          for( std::streamsize i = 0; i < n; )
          {
               while( left_-- > 0 )
               {
                    dst[ i++ ] = c_;
                    if( i >= n )
                    {
                         return n;
                    }
               }
               const auto c = boost::iostreams::get( src );
               if( c == EOF )
               {
                    return i > 0 ? i : EOF;
               }
               else if( c == boost::iostreams::WOULD_BLOCK )
               {
                    return i;
               }
               c_ = c;
               left_ = n_;
          }
          return n;
     }

     template< typename Sink >
     std::streamsize write( Sink& dst, const char* src, const std::streamsize n )
     {
          for( std::streamsize i = 0; i < n; ++i )
          {
               auto steps = n_;
               while( steps-- )
               {
                    if( !boost::iostreams::put( dst, src[ i ] ) )
                    {
                         return i;
                    }
               }
          }
          return n;
     }

private:
     const int n_;
     char c_ = 0;
     int left_ = 0;
};


/// Фильтр, заменяющий в потоке символы из @a from на символы из @a to
///
/// @note Размеры последовательностей @a from и @a to должны совпадать.
///
/// Замена осуществляется следующим образом: каждый символ из последовательности @a from
/// заменяется символом из последовательности @a to, стоящим на той же позиции.
///
/// Т.е. если @a from = "aouei", а @a to = "AOUEI", то:
/// - 'a' => 'A'
/// - 'o' => 'O'
/// - 'u' => 'U'
/// - 'e' => 'E'
/// - 'i' => 'I'
///
/// "Education" => "EdUcAtIOn"
///
/// @note Символы из последовательности @a to могут повторяться, символы из @a from - нет.
///
/// Для @a from = "aouei" и @a to = "AO***" получим:
/// - 'a' => 'A'
/// - 'o' => 'O'
/// - 'u' => '*'
/// - 'e' => '*'
/// - 'i' => '*'
///
/// "Education" => "Ed*cAt*On"
///
/// @note Разумеется, регистр имеет значение!
///
/// @code{.cpp}
///
/// std::istringstream is{ "Some text data" };
/// std::ostringstream os;
///
/// boost::iostreams::filtering_istream fis;
/// fis.push( CharReplacer{ "aou", "XYZ" } );
/// fis.push( is );
///
/// boost::iostreams::copy( fis, os );
///
/// @endcode
///
struct CharReplacer : boost::iostreams::multichar_dual_use_filter
{
     CharReplacer( boost::string_view from, boost::string_view to )
          : dict_{ makeDict( from, to ) }
     {}

     template< typename Source >
     std::streamsize read( Source& src, char* dst, std::streamsize n )
     {
          if( (n = boost::iostreams::read( src, dst, n )) > 0 )
          {
               std::transform( dst, dst + n, dst,
                    [ this ]( const char c )
                    {
                         const auto found = dict_.find( c );
                         return found != dict_.end() ? found->second : c;
                    }
                    );
          }
          return n;
     }

     template< typename Sink >
     std::streamsize write( Sink& dst, const char* src, const std::streamsize n )
     {
          for( std::streamsize i = 0; i < n; ++i )
          {
               const auto c = src[ i ];
               const auto found = dict_.find( c );
               if( !boost::iostreams::put( dst, found != dict_.end() ? found->second : c ) )
               {
                    return i;
               }
          }
          return n;
     }

private:
     using CharDict = std::map< char, char >;
     static CharDict makeDict( boost::string_view from, boost::string_view to )
     {
          BOOST_ASSERT( from.size() == to.size() );
          CharDict dict;
          for( auto&& i: boost::irange( from.size() ) )
          {
               dict.emplace( from[ i ], to[ i ] );
          }
          return dict;
     }

     CharDict dict_;
};


} // namespace multichar
namespace symmetric {
namespace impl {


struct Monitor
{
     using char_type = char;

     /*
      * Attempts to filter the sequence [i1, i2), storing the result in the sequence [o1, o2).
      * If flush is true, writes as much output to [o1, o2) as possible.
      * If flush is false, returns false to indicate that a “natural” end of stream
      * has been detected. Otherwise, returns true to indicate that additional characters,
      * not yet stored in [o1, o2), are available for output.
      */
     bool filter( const char*& ibeg, const char* iend, char*& obeg, char* oend, const bool flush )
     {
          if( os_ )
          {
               const auto ilen = std::distance( ibeg, iend );
               *os_ << __FUNCTION__
                    << ": (" << (flush ? 'f' : ' ' )
                    << ") <" << std::setw( 4 ) << ilen
                    << "> -> <" << std::setw( 4 ) << std::distance( obeg, oend )
                    << "> [" << boost::string_view( ibeg, ilen ) << ']'
                    << '\n';
          }

          while( ibeg != iend && obeg != oend )
          {
               *obeg++ = *ibeg++;
          }

          return flush ? ibeg != iend : flush;
     }
     void close()
     {
          if( os_ )
          {
               *os_ << __FUNCTION__ << '\n';
          }
     }

     void setOstream( std::ostream& os )
     {
          os_ = &os;
     }
private:
     std::ostream* os_ = nullptr;
};


/// Фильтр Base64-кодирования работает, но может быть использован только в качестве
/// учебного примера из-за следующих архитектурных недостатков:
/// -# фильтр не учитывает размера выходного буфера [obeg, oend) и работает <b>только
///    в качестве фильтра выходного потока</b>, когда размер выходного буфера гарантированно
///    превышает размер входного;
/// -# фильтр использует промежуточный внутренний буфер, т.к. при конвертации необходимо следить
///    за кратностью обрабатываемых входных данных и переносить остаток на следующий цикл
///    обработки.
///
struct Base64Encoder
{
     using char_type = char;
     using buffer = std::vector< char_type >;
     using base64_encode_iterator =
          boost::archive::iterators::base64_from_binary<
               boost::archive::iterators::transform_width<
                    buffer::const_iterator
                    , 6
                    , 8
               >
          >;

     bool filter( const char*& ibeg, const char* iend, char*& obeg, char* oend, const bool flush )
     {
          boost::ignore_unused( oend );

          if( ibeg != iend )
          {
               ibeg = prepareBuffer( ibeg, std::distance( ibeg, iend ) );
          }

          obeg = encodeBuffer( obeg );
          ibeg = saveRemaining( ibeg, iend );

          if( flush && ibeg == iend )
          {
               obeg = appendTail( obeg );
          }

          return flush ? ibeg != iend : flush;
     }

     void close() {}

private:
     const char* prepareBuffer( const char* src, const std::size_t n )
     {
          const auto end = src + (n - n % 3 - buffer_.size());
          buffer_.insert( buffer_.end(), src, end );
          return end;
     }

     char* encodeBuffer( char* dst )
     {
          nbytes_ += buffer_.size();
          return std::copy(
               base64_encode_iterator{ buffer_.cbegin() }
               , base64_encode_iterator{ buffer_.cend() }
               , dst
               );
     }

     const char* saveRemaining( const char* begin, const char* end )
     {
          buffer_.assign( begin, end );
          return end;
     }

     char* appendTail( char* dst )
     {
          static constexpr char tail = '=';
          if( !std::exchange( tailed_, true ) )
          {
               const auto n =  nbytes_ % 3;
               return std::fill_n( dst, n ? 3 - n : 0, tail );
          }
          return dst;
     }

     buffer buffer_;
     unsigned nbytes_ = 0;
     bool tailed_ = false;
};


struct Counter
{
     using char_type = char;

     bool filter( const char*& ibeg, const char* iend, char*& obeg, char* oend, const bool flush )
     {
          const auto n = std::min(
               std::distance( ibeg, iend ),
               std::distance( obeg, oend )
               );

          obeg = std::copy_n( ibeg, n, obeg );
          ibeg += n;

          bytes_ += n;

          return flush ? ibeg != iend : flush;
     }

     void close() {}

     unsigned bytes() const noexcept
     {
          return bytes_;
     }

private:
     unsigned bytes_ = 0;
};


} // namespace impl


template< typename Impl = impl::Monitor, typename Alloc = std::allocator< char > >
struct MonitorT : boost::iostreams::symmetric_filter< Impl, Alloc >
{
     using Base = boost::iostreams::symmetric_filter< Impl, Alloc >;

     explicit MonitorT()
          : Base{ boost::iostreams::default_device_buffer_size }
     {}

     explicit MonitorT( std::ostream& os )
          : MonitorT{}
     {
          this->filter().setOstream( os );
     }
};


template< typename Impl, typename Alloc = std::allocator< typename Impl::char_type > >
struct FilterT : boost::iostreams::symmetric_filter< Impl, Alloc >
{
     using Base = boost::iostreams::symmetric_filter< Impl, Alloc >;
     FilterT() : Base{ boost::iostreams::default_device_buffer_size } {}
};


using Monitor = MonitorT<>;
using Base64Encoder = FilterT< impl::Base64Encoder >;
using Counter = FilterT< impl::Counter >;


} // namespace symmetric
} // namespace filters
} // namespace iostreams
} // namespace using_boost
