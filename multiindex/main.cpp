/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <multiindex/document.h>


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          std::cout
               << using_boost::multiindex::Document::findByText( "УПРУП" ) << '\n'
               << using_boost::multiindex::Document::findByText( "???" ) << '\n'
               << using_boost::multiindex::Document::findByText( "ОСП" ) << '\n'
               << using_boost::multiindex::Document::findByCode( "00UOD" ) << '\n'
               << using_boost::multiindex::Document::findByCode( "---" ) << '\n'
               << using_boost::multiindex::Document::findByText( "DER" ) << '\n'
               ;
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
