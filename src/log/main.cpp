/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <iostream>
#include <stdexcept>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/thread.hpp>

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


void worker()
{
     LOGGER( info ) << "Start working...";

     auto n = 250u;
     while( n-- )
     {
          LOGGER( info ) << "Iteration #" << n << ": here we are!";
          boost::this_thread::sleep( boost::posix_time::milliseconds{ 150 } );
     }

     LOGGER( info ) << "Work done.";
}


void testMtLogging()
{
     boost::thread_group tg;
     tg.create_thread( worker );
     tg.create_thread( worker );
     tg.create_thread( worker );
     tg.create_thread( worker );
     tg.join_all();
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          boost::log::core::get()->remove_all_sinks();
//          boost::log::core::get()->add_sink( using_boost::log::logger::sinks::makeSyslogSink() );
//          boost::log::core::get()->add_sink( using_boost::log::logger::sinks::makeOstreamSink( std::cout ) );
          boost::log::core::get()->add_sink( using_boost::log::logger::sinks::makeFileSink( using_boost::log::logger::sinks::LogFileOptions{}.logRotateSize( 10u * 1024u ) ) );

     }
     catch( const std::exception& e )
     {
          LOGGER( error ) << "exception: " << boost::diagnostic_information( e );
          return 1;
     }
     return 0;
}
