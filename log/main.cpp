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

enum Severity
{
     Verbose,
     Usual,
     Suspicious,
     Bad,
     Deadly
};


std::ostream& operator<<( std::ostream& os, const Severity sev )
{
     switch( sev )
     {
          case Verbose:
              os << "~verbose~";
              break;
          case Usual:
              os << "~usual~";
              break;
          case Suspicious:
              os << "~suspicious~";
              break;
          case Bad:
              os << "~bad~";
              break;
          case Deadly:
              os << "~deadly~";
              break;
          default:
               os.setstate( std::ios::failbit );
     }
     return os;
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
#if 0
          using_boost::log::init::limitSeverity( boost::log::trivial::info );
          using_boost::log::init::setLogRotation( "boost_log_trivial.%N", 1_KB );
          using_boost::log::handler::starter::run();
#endif
          boost::log::sources::severity_logger< Severity > log;

          BOOST_LOG_SEV( log, Usual ) << "This is Usual";
          BOOST_LOG_SEV( log, Verbose ) << "This is Verbose";
          BOOST_LOG_SEV( log, Deadly ) << "This is Deadly";
          BOOST_LOG_SEV( log, Bad ) << "This is Bad";
     }
     catch( const std::exception& e )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: " << boost::diagnostic_information( e );
          return 1;
     }
     return 0;
}
