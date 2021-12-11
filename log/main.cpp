/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <log/init.h>
#include <log/handlers.h>

using namespace using_boost::log;


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
#if 0
          using_boost::log::init::limitSeverity( boost::log::trivial::info );
          using_boost::log::init::setLogRotation( "boost_log_trivial.%N", 1_KB );
#endif
          using_boost::log::handler::starter::run();
     }
     catch( const std::exception& e )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: " << boost::diagnostic_information( e );
          return 1;
     }
     return 0;
}
