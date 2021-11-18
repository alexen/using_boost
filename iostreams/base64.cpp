/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <iostreams/base64.h>

#include <set>

#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/char_traits.hpp>


namespace base64 {


namespace {
namespace impl {


static void encode( std::istream& is, std::ostream& os )
{
     using Base64EncodingIterator =
          boost::archive::iterators::base64_from_binary<
               boost::archive::iterators::transform_width<
                    std::istreambuf_iterator< char >
                    , 6
                    , 8
               >
          >;
     std::copy(
          Base64EncodingIterator{ std::istreambuf_iterator< char >{ is } }
          , Base64EncodingIterator{ std::istreambuf_iterator< char >{} }
          , std::ostreambuf_iterator< char >{ os }
     );
}


static void decode( std::istream& is, std::ostream& os )
{
     using Base64DecodingIterator =
          boost::archive::iterators::transform_width<
               boost::archive::iterators::binary_from_base64<
                    std::istreambuf_iterator< char >
               >
               , 8
               , 6
               >;
     std::copy(
          Base64DecodingIterator{ std::istreambuf_iterator< char >{ is } }
          , Base64DecodingIterator{ std::istreambuf_iterator< char >{} }
          , std::ostreambuf_iterator< char >{ os }
          );
}


namespace input_filters {


struct Base64StreamEndDetector : boost::iostreams::input_filter, boost::noncopyable
{
     static constexpr auto END_OF_BASE64 = '=';

     template< typename Source >
     int get( Source& src )
     {
          const auto c = boost::iostreams::get( src );
          return c == END_OF_BASE64 ? EOF : c;
     }
};


struct IgnoredCharsRemover : boost::iostreams::input_filter, boost::noncopyable
{
     template< typename ...Args >
     explicit IgnoredCharsRemover( Args&& ...args )
          : ignored_{ std::forward< Args >( args )... } {}

     template< typename Source >
     int get( Source& src )
     {
          const auto c = boost::iostreams::get( src );
          return ignored_.count( c ) ? boost::iostreams::WOULD_BLOCK : c;
     }

private:
     std::set< char > ignored_;
};


struct CharCounter : boost::iostreams::multichar_input_filter
{
     explicit CharCounter( unsigned n = 0 ) : chars_{ n } {}

     template< typename Source >
     std::streamsize read( Source& src, char* s, std::streamsize n )
     {
          const auto result = boost::iostreams::read( src, s, n );
          chars_ += result > 0 ? result : 0;
          return result;
     }

     unsigned chars() const noexcept
     {
          return chars_;
     }

private:
     unsigned chars_ = 0;
};


} // namespace input_filters
} // namespace impl
} // namespace {unnamed}


void encode( std::istream& is, std::ostream& os )
{
     boost::iostreams::filtering_istream fis;

     impl::input_filters::CharCounter counter;
     fis.push( boost::ref( counter ) );
     fis.push( is );

     impl::encode( fis, os );

     const auto n = counter.chars() % 3;
     std::fill_n( std::ostreambuf_iterator< char >{ os }, n ? 3 - n : 0, '=' );
}


void decode( std::istream& is, std::ostream& os, std::initializer_list< char > ignored )
{
     static impl::input_filters::Base64StreamEndDetector streamEndDetector;
     impl::input_filters::IgnoredCharsRemover ignoredCharsRemover{ ignored };

     boost::iostreams::filtering_istream fis;

     fis.push( boost::ref( streamEndDetector ) );
     fis.push( boost::ref( ignoredCharsRemover ) );
     fis.push( is );

     impl::decode( fis, os );
}


} // namespace base64
