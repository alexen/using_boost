/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <iostreams/base64.h>

#include <unordered_set>

#include <boost/archive/iterators/transform_width.hpp>
#include <boost/archive/iterators/base64_from_binary.hpp>
#include <boost/archive/iterators/binary_from_base64.hpp>
#include <boost/iostreams/filter/counter.hpp>
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


struct Base64StreamEndSearcher : boost::iostreams::input_filter, boost::noncopyable
{
     static constexpr auto base64End = '=';

     template< typename Source >
     int get( Source& src )
     {
          const auto c = boost::iostreams::get( src );
          return c == base64End ? boost::iostreams::char_traits< char >::eof() : c;
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
          while( true )
          {
               const auto c = boost::iostreams::get( src );
               if( ignored_.count( c ) )
               {
                    continue;
               }
               return c;
          }
     }

private:
     const std::unordered_set< char > ignored_;
};


} // namespace input_filters
} // namespace impl
} // namespace {unnamed}


void encode( std::istream& is, std::ostream& os )
{
     boost::iostreams::filtering_istream fis;

     boost::iostreams::counter counter;
     fis.push( boost::ref( counter ) );
     fis.push( is );

     impl::encode( fis, os );

     const auto n = counter.characters() % 3;
     std::fill_n( std::ostreambuf_iterator< char >{ os }, n ? 3 - n : 0, '=' );
}


void decode( std::istream& is, std::ostream& os, std::initializer_list< char > ignored )
{
     static impl::input_filters::Base64StreamEndSearcher streamEndSearcher;
     impl::input_filters::IgnoredCharsRemover ignoredCharsRemover{ ignored };

     boost::iostreams::filtering_istream fis;

     fis.push( boost::ref( streamEndSearcher ) );
     fis.push( boost::ref( ignoredCharsRemover ) );
     fis.push( is );

     impl::decode( fis, os );
}


} // namespace base64
