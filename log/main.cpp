/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/log/trivial.hpp>


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          BOOST_LOG_TRIVIAL( debug ) << "This is debug";
          BOOST_LOG_TRIVIAL( info ) << "This is info";
          BOOST_THROW_EXCEPTION( std::runtime_error{ "some kind of failure" } );
     }
     catch( const std::exception& e )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: " << boost::diagnostic_information( e );
          return 1;
     }
     return 0;
}
