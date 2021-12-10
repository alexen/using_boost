/// @file
/// @brief


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
          using using_boost::multiindex::Document;

          std::cout
               << Document::findByText( {} ) << '\n'
               << Document::findByCode( {} ) << '\n'
               << Document::findByText( "ОСП" ) << '\n'
               << Document::findByText( "DER" ) << '\n'
               << Document::findByText( "УПРУП" ) << '\n'
               << Document::findByText( "???" ) << '\n'
               << Document::findByText( "ЗПЭД" ) << '\n'
               << Document::findByCode( "00UOD" ) << '\n'
               << Document::findByCode( "---" ) << '\n'
               << Document::findByText( "DER" ) << '\n'
               << Document::findByCode( "SZV-ETD" ) << '\n'
               << Document::findByType( Document::Type::DisconnectingStatement ) << '\n'
               ;
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
