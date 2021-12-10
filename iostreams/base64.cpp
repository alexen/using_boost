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


static void padding( const std::size_t bytes, std::ostream& os )
{
     const auto n = bytes % 3;
     std::fill_n( std::ostreambuf_iterator< char >{ os }, n ? 3 - n : 0, '=' );
}


} // namespace impl
} // namespace {unnamed}


void encode( std::istream& is, std::ostream& os )
{
     boost::iostreams::filtering_istream fis;

     filters::multichar::Counter counter;
     fis.push( boost::ref( counter ) );
     fis.push( is );

     impl::encode( fis, os );
     impl::padding( counter.chars(), os );
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

     impl::decode( fis, os );
}


} // namespace base64
} // namespace iostreams
} // namespace using_boost
