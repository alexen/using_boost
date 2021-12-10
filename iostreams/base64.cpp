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
namespace options {


/// Использовать URL and filename safe набор символов
/// @see https://datatracker.ietf.org/doc/html/rfc3548#section-4
enum class UseUrlSafeAlphabet: bool { No, Yes };


/// Использовать хвостовые символы
enum class PaddingRequired: bool { No, Yes };


} // namespace options



namespace {
namespace impl {
namespace boost_based {


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


} // namespace boost_based


template< typename OutputIterator >
void padding( const std::size_t bytes, OutputIterator out )
{
     const auto n = bytes % 3;
     std::fill_n( out, n ? 3 - n : 0, '=' );
}


void encode(
     std::istream& is
     , std::ostream& os
     , const options::UseUrlSafeAlphabet useUrlSafeAbc
     , const options::PaddingRequired paddingRequred
)
{
     static filters::multichar::Counter counter;
     static filters::multichar::CharReplacer replacer{ "+/", "-_" };

     boost::iostreams::filtering_istream fis;
     if( static_cast< bool >( paddingRequred ) )
     {
          counter.reset();
          fis.push( boost::ref( counter ) );
     }
     fis.push( is );

     boost::iostreams::filtering_ostream fos;
     if( static_cast< bool >( useUrlSafeAbc ) )
     {
          fos.push( boost::ref( replacer ) );
     }
     fos.push( os );

     boost_based::encode(
          std::istreambuf_iterator< char >{ fis }
          , std::istreambuf_iterator< char >{}
          , std::ostreambuf_iterator< char >{ fos }
          );

     if( static_cast< bool >( paddingRequred ) )
     {
          padding( counter.chars(), std::ostreambuf_iterator< char >{ fos } );
     }
}


void decode(
     std::istream& is
     , std::ostream& os
     , boost::string_view ignored
     , const options::UseUrlSafeAlphabet useUrlSafeAbc
)
{
     static filters::multichar::CharReplacer replacer{ "-_", "+/" };
     static filters::multichar::StreamInterrupter interrupter{ "=" };

     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( interrupter ) );
     if( static_cast< bool >( useUrlSafeAbc ) )
     {
          fis.push( boost::ref( replacer ) );
     }
     if( !ignored.empty() )
     {
          fis.push( std::move( filters::multichar::CharRemover{ ignored } ) );
     }
     fis.push( is );

     boost_based::decode(
          std::istreambuf_iterator< char >{ fis }
          , std::istreambuf_iterator< char >{}
          , std::ostreambuf_iterator< char >{ os }
          );
}


} // namespace impl
} // namespace {unnamed}


void encode( std::istream& is, std::ostream& os )
{
     impl::encode(
          is
          , os
          , options::UseUrlSafeAlphabet::No
          , options::PaddingRequired::Yes
          );
}


void decode( std::istream& is, std::ostream& os, boost::string_view ignored )
{
     impl::decode(
          is
          , os
          , ignored
          , options::UseUrlSafeAlphabet::No
          );
}


namespace url_safe {


void encode( std::istream& is, std::ostream& os, const bool padding )
{
     impl::encode(
          is
          , os
          , options::UseUrlSafeAlphabet::Yes
          , static_cast< options::PaddingRequired >( padding )
          );
}


void decode( std::istream& is, std::ostream& os, boost::string_view ignored )
{
     impl::decode(
          is
          , os
          , ignored
          , options::UseUrlSafeAlphabet::Yes
          );
}


} // namespace url_safe
} // namespace base64
} // namespace iostreams
} // namespace using_boost
