/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <stdexcept>
#include <iostream>

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
}


void copy_by_readsome( std::istream& is, std::ostream& os )
{
     std::cout << __FUNCTION__ << ":\n";
     char buffer[ 1 ];
     while( true )
     {
          const auto nread = is.readsome( buffer, sizeof( buffer ) );
          if( nread > 0 )
          {
               os.write( buffer, nread );
          }
          else
          {
               break;
          }
     }
}


void copy_by_rdbuf( std::istream& is, std::ostream& os )
{
     std::cout << __FUNCTION__ << ":\n";
     os << is.rdbuf();
     os.flush();
}


int main( int argc, char** argv )
{
     try
     {
          const auto readers = {
               copy_by_char,
               copy_by_read,
               copy_by_readsome,
               copy_by_rdbuf
          };
          const std::string data = "This is very big data!";

          for( auto&& reader: readers )
          {
               {
                    DirectIteratorStream< std::string > istr{ data };
                    reader( istr, std::cout );
                    std::cout << std::endl;
               }
               {
                    ReverseIteratorStream< std::string > istr{ data };
                    reader( istr, std::cout );
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
