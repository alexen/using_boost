/// @file
/// @brief


#include <vector>
#include <istream>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <algorithm>
#include <iterator>

#include <boost/core/ignore_unused.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/iostreams/filter/counter.hpp>
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


#define LOG_PREFIX( prefix, strm ) do{ std::cout << __FUNCTION__ << ": " << prefix << strm << '\n'; }while( false )
#define LOG( strm ) LOG_PREFIX( "", strm )
#define LOG_INDENT( n, strm ) LOG_PREFIX( std::string( n, ' ' ), strm )


namespace filters {
namespace symmetric {
namespace impl {


struct BlockFilter
{
     using char_type = char;

     bool filter( const char*& ibeg, const char* iend, char*& obeg, char* oend, const bool flush )
     {
          const auto n = std::min(
               std::distance( ibeg, iend ),
               std::distance( obeg, oend )
               );

          if( n > 0 )
          {
               processBlock( ibeg, static_cast< std::size_t >( n ) );
          }

          obeg = std::copy_n( ibeg, n, obeg );
          ibeg += n;

          const auto eos = ibeg == iend;
          if( flush && eos )
          {
               finalize();
          }

          return flush ? !eos: flush;
     }

     void close() {}

private:
     void processBlock( const char* s, std::size_t n )
     {
          std::cout << __FUNCTION__ << ": block [" << boost::string_view{ s, n } << "]\n";
     }

     void finalize()
     {
          if( !finalized_ )
          {
               std::cout << __FUNCTION__ << ": finalize";
               finalized_ = true;
          }
     }

     bool finalized_ = false;
};


} // namespace impl



using Transparent = using_boost::iostreams::filters::symmetric::FilterT< impl::BlockFilter >;


} // namespace symmetric
} // namespace filters


template< typename DstCont >
DstCont getSequence()
{
     using OutputDevice = boost::iostreams::back_insert_device< DstCont >;
     using OutputStream = boost::iostreams::stream< OutputDevice >;

     static constexpr auto start = 'A';
     static constexpr auto len = 10;

     DstCont dst;
     OutputStream os{ dst };

     for( auto c = start; c < start + len; ++c )
     {
          os << c;
          (std::cout << "Process: step#" << (c - start) << ": (buff: size: " << dst.size() << ", cap: " << dst.capacity() << ")")
               .write( dst.data(), dst.size() ).put( '\n' );
     }
     return dst;
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          const auto dst = getSequence< std::vector< char > >();
          (std::cout << "Result: ").write( dst.data(), dst.size() ).put( '\n' );
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
