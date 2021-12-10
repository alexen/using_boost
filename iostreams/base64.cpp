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

#include <iostreams/filters.h>


namespace using_boost {
namespace iostreams {
namespace base64 {


namespace {
namespace impl {


template< typename InputIterator, typename OutputIterator >
void encode( InputIterator begin, InputIterator end, OutputIterator out )
{
     using Base64EncodingIterator =
          boost::archive::iterators::base64_from_binary<
               boost::archive::iterators::transform_width< InputIterator, 6, 8 >
          >;
     std::copy(
          Base64EncodingIterator{ begin }
          , Base64EncodingIterator{ end }
          , out
     );
}


template< typename InputIterator, typename OutputIterator >
void decode( InputIterator begin, InputIterator end, OutputIterator out )
{
     using Base64DecodingIterator =
          boost::archive::iterators::transform_width<
               boost::archive::iterators::binary_from_base64< InputIterator >
               , 8
               , 6
               >;
     std::copy(
          Base64DecodingIterator{ begin }
          , Base64DecodingIterator{ end }
          , out
          );
}


template< typename OutputIterator >
void padding( const std::size_t bytes, OutputIterator out )
{
     const auto n = bytes % 3;
     std::fill_n( out, n ? 3 - n : 0, '=' );
}


} // namespace impl
} // namespace {unnamed}


void encode( std::istream& is, std::ostream& os )
{
     boost::iostreams::filtering_istream fis;

     filters::multichar::Counter counter;
     fis.push( boost::ref( counter ) );
     fis.push( is );

     impl::encode(
          std::istreambuf_iterator< char >{ fis }
          , std::istreambuf_iterator< char >{}
          , std::ostreambuf_iterator< char >{ os }
          );
     impl::padding( counter.chars(), std::ostreambuf_iterator< char >{ os } );
}


void decode( std::istream& is, std::ostream& os, boost::string_view ignored )
{
     static filters::multichar::StreamInterrupter streamInterrupter{ "=" };

     boost::iostreams::filtering_istream fis;

     fis.push( boost::ref( streamInterrupter ) );
     if( !ignored.empty() )
     {
          fis.push( std::move( filters::multichar::CharRemover{ ignored } ) );
     }
     fis.push( is );

     impl::decode(
          std::istreambuf_iterator< char >{ fis }
          , std::istreambuf_iterator< char >{}
          , std::ostreambuf_iterator< char >{ os }
          );
}


} // namespace base64
} // namespace iostreams
} // namespace using_boost
