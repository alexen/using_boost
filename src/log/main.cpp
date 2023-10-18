/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <dlfcn.h>

#include <memory>
#include <iostream>
#include <stdexcept>

#include <boost/thread.hpp>
#include <boost/bind/bind.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/exception/error_info.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/exception/enable_error_info.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/exception/errinfo_file_name.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>

#include <log/modules/alice/alice.h>
#include <log/modules/clark/clark.h>


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


void worker( using_boost::modules::LoggerSource& logger )
{
     LOGGER_INFO( logger ) << "Start working...";

     auto n = 250u;
     while( n-- )
     {
          LOGGER_INFO( logger ) << "Iteration #" << n << ": here we are!";
          boost::this_thread::sleep( boost::posix_time::milliseconds{ 150 } );
     }

     LOGGER_INFO( logger ) << "Work done.";
}


void testMtLogging( using_boost::modules::LoggerSource& logger )
{
     boost::thread_group tg;
     const auto fn = boost::bind( worker, boost::ref( logger ) );
     tg.create_thread( fn );
     tg.create_thread( fn );
     tg.create_thread( fn );
     tg.create_thread( fn );
     tg.join_all();
}


void unload( void* handle )
{
     /// @fixme Do not ignore returning value!
     dlclose( handle );
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     using_boost::modules::LoggerSource logger;
     try
     {
          boost::log::core::get()->remove_all_sinks();
          boost::log::add_common_attributes();
          boost::log::add_console_log( std::cerr,
               boost::log::keywords::format = "%ThreadID% >> %TimeStamp% (%Severity%): %Message%" );

          LOGGER_INFO( logger ) << "Start testing Boost.Log with modules";

          LOGGER_INFO( logger ) << "Create modules Alice and Clark";

          using_boost::modules::Alice alice;
          using_boost::modules::Clark clark;

          LOGGER_INFO( logger ) << "Run Alice methods";

          alice.init();
          alice.run();

          LOGGER_INFO( logger ) << "Run Clark methods";

          clark.init();
          clark.run();

          LOGGER_INFO( logger ) << "Now, we are finished!";
     }
     catch( const std::exception& e )
     {
          LOGGER_ERROR( logger ) << "exception: " << boost::diagnostic_information( e );
          return 1;
     }
     return 0;
}
