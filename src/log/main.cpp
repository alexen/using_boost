/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <dlfcn.h>

#include <memory>
#include <iostream>
#include <stdexcept>

#include <boost/thread.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/exception/error_info.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/exception/enable_error_info.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/exception/errinfo_file_name.hpp>

#include <log/application.h>
#include <log/handlers.h>
#include <log/logger/logger.h>
#include <log/logger/sinks.h>
#include <log/modules/imodule.h>
#include <log/modules/dynlib/types.h>
#include <log/modules/dynlib/loader.h>


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


void unload( void* handle )
{
     /// @fixme Do not ignore returning value!
     dlclose( handle );
}


using DynLibList = std::list< using_boost::modules::dynlib::DynLibUptr >;
using ModulesUptrList = std::list< using_boost::modules::IModuleUptr >;


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          boost::log::core::get()->remove_all_sinks();
//          boost::log::core::get()->add_sink( using_boost::log::logger::sinks::makeSyslogSink() );
          boost::log::core::get()->add_sink( using_boost::log::logger::sinks::makeOstreamSink( std::cerr ) );
          boost::log::core::get()->add_sink(
               using_boost::log::logger::sinks::makeFileSink(
                    using_boost::log::logger::sinks::LogFileOptions{}
                         .logRotateSize( 10u * 1024u ) ) );

          DynLibList dynlibs;

          std::transform(
               argv + 1,
               argv + argc,
               std::back_inserter( dynlibs ),
               using_boost::modules::dynlib::load
               );

          ModulesUptrList modules;

          for( auto&& each: dynlibs )
          {
               modules.emplace_back( using_boost::modules::dynlib::call< using_boost::modules::ModuleCreatorFn >( each, "create" ) );
          }

          for( auto&& each: modules )
          {
               std::cout << "Working with " << each->name() << '\n';
               each->init();
               each->run();
          }
     }
     catch( const std::exception& e )
     {
          LOGGER( error ) << "exception: " << boost::diagnostic_information( e );
          return 1;
     }
     return 0;
}
