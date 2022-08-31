/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <iostream>
#include <stdexcept>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <log/application.h>
#include <log/handlers.h>
#include <log/logger/logger.h>


struct ICertificate {};

using ICertificateUptr = std::unique_ptr< ICertificate >;


std::ostream& operator<<( std::ostream& os, const ICertificate& )
{
     return os << __PRETTY_FUNCTION__;
}


std::ostream& operator<<( std::ostream& os, const ICertificateUptr& p )
{
     if( p )
     {
          os << *p;
     }
     else
     {
          os << __PRETTY_FUNCTION__;
     }
     return os;
}


namespace impl {

struct Certificate : ICertificate {};

} // namespace impl


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          boost::log::core::get()->remove_all_sinks();
          boost::log::core::get()->add_sink( using_boost::log::logger::sinks::makeSyslogSink() );
          boost::log::core::get()->add_sink( using_boost::log::logger::sinks::makeOstreamSink( std::cout ) );

          LOGGER( info ) << "This is simple logger";
          LOGGER( trace ) << "Tracing message";

          const auto severity = boost::log::trivial::debug;

          LOGGER_SEV( severity ) << "Hello!";

          ICertificateUptr cp;

          LOGGER( info ) << "We have certificate: " << cp;

//          using_boost::log::app::Application app;
//          app.run();
//
//          using_boost::log::handler::starter::run();
//
//          BOOST_THROW_EXCEPTION( std::runtime_error{ "error" } );
     }
     catch( const std::exception& e )
     {
          LOGGER( error ) << "exception: " << boost::diagnostic_information( e );
          return 1;
     }
     return 0;
}
