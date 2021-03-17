/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/regex/v4/regex.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility/string_view.hpp>


using OnError = std::function< void( void ) >;

std::tuple< std::string, std::string > parseFormalizedFileName( const std::string& fileName, OnError&& raiseError )
{
     static constexpr auto expectedMatches = 2u;
     static const boost::regex re{ R"re(^(\d{3}-?\d{3}-?\d{6})_(\d{3}-?\d{3})_[0-9a-f]{32}\.zip$)re", boost::regex::icase };
     boost::smatch matches;
     if( boost::regex_search( fileName, matches, re ) && matches.size() == expectedMatches + 1 )
     {
          return std::make_tuple( matches[ 1 ], matches[ 2 ] );
     }
     raiseError();
     return {};
}


std::tuple< std::string, std::string > parseFormalizedFileName( const std::string& fileName )
{
     return parseFormalizedFileName( fileName,
          [ &fileName ]{ BOOST_THROW_EXCEPTION( std::invalid_argument{ "invalid transport package name format: " + fileName } ); } );
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          std::string rns, num;
          std::tie( rns, num ) = parseFormalizedFileName( "001-001-163500_001-001_218c384544b4489b9fb08c84bdc53183.ZIP" );
          std::cout
               << "rns: " << rns << '\n'
               << "num: " << num << '\n';
          std::tie( rns, num ) = parseFormalizedFileName( "BAD_PACKAGE_NAME.EXT" );
          std::cout
               << "rns: " << rns << '\n'
               << "num: " << num << '\n';
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
