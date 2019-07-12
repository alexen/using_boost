/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <vector>
#include <istream>
#include <stdexcept>
#include <iostream>

#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/invert.hpp>
#include <boost/iostreams/operations.hpp>
#include <boost/iostreams/read.hpp>
#include <boost/iostreams/flush.hpp>
#include <boost/iostreams/tee.hpp>
#include <boost/iostreams/close.hpp>
#include <boost/iostreams/filter/counter.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include "iterator_source.h"


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


int main( int argc, char** argv )
{
     try
     {
          const auto readers = {
               copy_by_char,
               copy_by_read,
               copy_by_readsome,
               copy_by_rdbuf,
               copy_by_boost_iostreams_copy
          };
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

          const auto& data = russianLyric;

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
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
