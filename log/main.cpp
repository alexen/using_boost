/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <log/init.h>


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          using_boost::log::init::limitSeverity( boost::log::trivial::info );
          using_boost::log::init::setLogRotation( "boost_log_trivial.%N", 1_KB );

          for( auto i = 0u; i < 1_KB; ++i )
          {
               BOOST_LOG_TRIVIAL( debug ) << "This is debug";
               BOOST_LOG_TRIVIAL( info ) << "This is info";
          }
          BOOST_THROW_EXCEPTION( std::runtime_error{ "some kind of failure" } );
     }
     catch( const std::exception& e )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: " << boost::diagnostic_information( e );
          return 1;
     }
     return 0;
}
