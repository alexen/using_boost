/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <log/init.h>
#include <log/handlers.h>
#include <log/application.h>


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          using_boost::log::init::syslogBackend();

          using_boost::log::app::Application app;
          app.run();
     }
     catch( const std::exception& e )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: " << boost::diagnostic_information( e );
          return 1;
     }
     return 0;
}
