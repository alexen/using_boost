/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <boost/logic/tribool.hpp>
#include <boost/logic/tribool_io.hpp>


void print( const std::string& context, boost::logic::tribool state )
{
     std::cout << '[' << context << "] state == " << state << ": ";
     if( state )
     {
          std::cout << "looks like state is true\n";
     }
     if( !state )
     {
          std::cout << "looks like state is false\n";
     }
     if( boost::logic::indeterminate( state ) )
     {
          std::cout << "looks like state is indetermined\n";
     }
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          boost::logic::tribool state;
          print( "initialized", state );
          state = false;
          print( "set to false", state );
          state = true;
          print( "set to true", state );
          state = boost::logic::indeterminate;
          print( "set to indetermined", state );
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
