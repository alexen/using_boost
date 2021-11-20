/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <vector>
#include <istream>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <iterator>

#include <boost/core/ignore_unused.hpp>
#include <boost/range/irange.hpp>
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
#include <boost/iostreams/filter/zstd.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/filter/symmetric.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/utility/string_view.hpp>

#include <iostreams/filters.h>
#include <iostreams/iterator_source.h>


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


struct CustomSymmetricFilterImpl
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
          if( debug_ )
          {
               const auto ilen = std::distance( ibeg, iend );
               std::clog << __FUNCTION__
                    << ": [" << (flush ? 'f' : ' ' ) << ']'
                    << " " << ilen << " -> " << std::distance( obeg, oend )
                    << " (" << boost::string_view( ibeg, ilen ) << ')'
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
          if( debug_ )
          {
               std::clog << __FUNCTION__ << '\n';
          }
     }

     void setDebugEnabled( bool f = true )
     {
          debug_ = f;
     }
private:
     bool debug_ = false;
};


template< typename Impl = CustomSymmetricFilterImpl, typename Alloc = std::allocator< char > >
struct CustomSymmetricFilterT : boost::iostreams::symmetric_filter< Impl, Alloc >
{
     using Base = boost::iostreams::symmetric_filter< Impl, Alloc >;

     CustomSymmetricFilterT( bool debug = false, std::streamsize bufferSize = boost::iostreams::default_device_buffer_size )
          : Base{ bufferSize }
     {
          this->filter().setDebugEnabled( debug );
     }
};


using CustomSymmetricFilter = CustomSymmetricFilterT<>;


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          static constexpr auto text =
               "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod "
               "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, "
               "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo "
               "consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse "
               "cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non "
               "proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

          boost::iostreams::zstd_compressor zc;
          CustomSymmetricFilter csf;

          std::istringstream is{ text };
          std::ostringstream os;
          boost::iostreams::filtering_ostream fos;
          fos.push( boost::ref( csf ) );
          fos.push( os );
          boost::iostreams::copy( is, fos );

          if( os.str() != text )
          {
               BOOST_THROW_EXCEPTION( std::runtime_error{ "result does not match" } );
          }
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
