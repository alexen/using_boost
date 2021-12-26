/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <stdexcept>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <log/application.h>
#include <log/init.h>
#include <log/handlers.h>
#include <log/logger/logger.h>


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          using_boost::log::logger::init::sinks::syslog();

          LOGGER( info ) << "This is simple logger";
          LOGGER( trace ) << "Tracing message";

          using_boost::log::app::Application app;
          app.run();

          using_boost::log::handler::starter::run();

          BOOST_THROW_EXCEPTION( std::runtime_error{ "error" } );
     }
     catch( const std::exception& e )
     {
          LOGGER( error ) << "exception: " << boost::diagnostic_information( e );
          return 1;
     }
     return 0;
}
