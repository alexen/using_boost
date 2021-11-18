/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <vector>
#include <istream>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>

#include <boost/core/ignore_unused.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/invert.hpp>
#include <boost/iostreams/operations.hpp>
#include <boost/iostreams/read.hpp>
#include <boost/iostreams/flush.hpp>
#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/close.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/stdio.hpp>
#include <boost/iostreams/filter/counter.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/utility/string_view.hpp>

#include "iterator_source.h"


namespace {
namespace aux {


const std::string russianLyric =
     "О, сколько нам открытий чудных\n"
     "Готовит просвещения дух!\n"
     "И опыт, сын ошибок трудных,\n"
     "И гений, парадоксов друг,\n"
     "И случай, бог изобретатель...\n";

const std::string englishLyric =
     "Now when I'm nearly done my days\n"
     "And grown too stiff to sweep or sew\n"
     "I sit and think, 'till I'm amaze\n"
     "About what lots of things I know...\n";


} // namespace aux
} // namespace {unnamed}



void copy_by_char( std::istream& is, std::ostream& os )
{
     std::cout << __FUNCTION__ << ":\n";
     std::copy(
          std::istream_iterator< char >{ is >> std::noskipws },
          std::istream_iterator< char >{},
          std::ostream_iterator< char >{ os }
          );
     os.flush();
}


void copy_by_read( std::istream& is, std::ostream& os )
{
     std::cout << __FUNCTION__ << ":\n";
     char buffer[ 5 ];
     do
     {
          is.read( buffer, sizeof( buffer ) );
          os.write( buffer, is.gcount() );
     }
     while( is && os );
     os.flush();
}


void copy_by_readsome( std::istream& is, std::ostream& os )
{
     std::cout << __FUNCTION__ << ":\n";
     char chunk[ 10 ];
     std::streamsize nread = 0;
     while( (nread = is.readsome( chunk, sizeof( chunk ) )) > 0 )
     {
          os.write( chunk, nread );
     }
     os.flush();
}


void copy_by_rdbuf( std::istream& is, std::ostream& os )
{
     std::cout << __FUNCTION__ << ":\n";
     os << is.rdbuf() << std::flush;
}


void copy_by_boost_iostreams_copy( std::istream& is, std::ostream& os )
{
     std::cout << __FUNCTION__ << ":\n";
     boost::iostreams::copy( is, os );
}


void test_readers()
{
     const auto readers = {
          copy_by_char,
          copy_by_read,
          copy_by_readsome,
          copy_by_rdbuf,
          copy_by_boost_iostreams_copy
     };

     const auto& data = aux::russianLyric;

     std::cout << "Size: " << data.size() << '\n';

     for( auto&& reader: readers )
     {
          {
               DirectIteratorOstream< std::string > istr{ data };
               reader( istr, std::cout );
               std::cout << std::endl;
          }
          {
               ReverseIteratorOstream< std::string > istr{ data };
               reader( istr, std::cout );
               std::cout << std::endl;
          }
          {
               boost::iostreams::filtering_istream istr{ boost::make_iterator_range( data ) };
               reader( istr, std::cout );
               std::cout << std::endl;
          }
          {
               boost::iostreams::filtering_istream istr{ boost::make_iterator_range( data ) };
               std::vector< char > buffer;
               boost::iostreams::filtering_ostream ostr{ boost::iostreams::back_inserter( buffer ) };
               reader( istr, ostr );
               std::cout.write( buffer.data(), buffer.size() );
               std::cout << std::endl;
          }
          {
               boost::iostreams::filtering_istream istr{ boost::make_iterator_range( data ) };
               std::vector< char > buffer;
               boost::iostreams::stream<
                    boost::iostreams::back_insert_device< std::vector< char > >
               > ostr{ buffer };
               reader( istr, ostr );
               std::cout.write( buffer.data(), buffer.size() );
               std::cout << std::endl;
          }
          {
               boost::iostreams::filtering_istream istr{ boost::make_iterator_range( data ) };
               boost::iostreams::filtering_ostream ostr;
               std::vector< char > buffer;
               ostr.push( boost::iostreams::counter{} );
               ostr.push( boost::iostreams::back_inserter( buffer ) );
               reader( istr, ostr );
               std::cout.write( buffer.data(), buffer.size() );
               std::cout << std::endl;
               ostr.pop();
               if( const auto c = ostr.component< boost::iostreams::counter >( 0 ) )
               {
                    const auto& counter = *c;
                    std::cout << "\tChars: " << counter.characters() << '\n'
                              << "\tLines: " << counter.lines() << '\n';
               }
          }
          {
               boost::iostreams::stream< boost::iostreams::array_source > istr{
                    boost::iostreams::array_source{ data.data(), data.size() }
               };

               using buffer_t = std::vector< char >;
               using buffer_output_device_t = boost::iostreams::back_insert_device< buffer_t >;
               using buffer_ostream_t = boost::iostreams::stream< buffer_output_device_t >;

               buffer_t buffer;
               buffer_ostream_t ostr{ buffer };
               reader( istr, ostr );
               std::cout.write( buffer.data(), buffer.size() );
               std::cout << std::endl;
          }
     }
}


namespace custom_filters {


namespace {
namespace impl {


bool isVowel( const int c ) noexcept
{
     static constexpr auto vowels = "aeiouAEIOU";
     return std::strchr( vowels, c ) != nullptr;
}


} // namespace impl
} // namespace {unnamed}


namespace single_char {


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
          BOOST_ASSERT_MSG( false, "this code must be UNREACHABLE" );
     }
};


} // namespace single_char
namespace multichar {


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
} // namespace custom_filters



int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          const std::string text =
               "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod "
               "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, "
               "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo "
               "consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse "
               "cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non "
               "proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

          const auto expected =
               "Lrm psm dlr st mt, cnscttr dpscng lt, sd d smd tmpr ncddnt t lbr t dlr mgn lq. "
               "t nm d mnm vnm, qs nstrd xrcttn llmc lbrs ns t lqp x  cmmd cnsqt. Ds t rr dlr "
               "n rprhndrt n vlptt vlt ss cllm dlr  fgt nll prtr. xcptr snt ccct cpdtt nn prdnt, "
               "snt n clp q ffc dsrnt mllt nm d st lbrm.";

          std::cout << "Source: " << text << '\n';

          std::istringstream is{ text };
          boost::iostreams::filtering_istream fis;
          fis.push( custom_filters::single_char::VowelRemover{} );
          fis.push( custom_filters::multichar::VowelRemover{} );
          fis.push( is );

          std::ostringstream oss;
          boost::iostreams::copy( fis, oss );
          std::cout << "Result: " << oss.str() << '\n';
          if( oss.str() != expected )
          {
               BOOST_THROW_EXCEPTION( std::runtime_error{ "bad result" } );
          }
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
