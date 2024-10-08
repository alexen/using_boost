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
#include <boost/range/irange.hpp>

#include <log/modules/imodule.h>
#include <log/modules/dynlib/types.h>
#include <log/modules/dynlib/loader.h>

#include <log/logger/initializer.h>


using DynLibList = std::list< using_boost::modules::dynlib::DynLibUptr >;
using ModulesUptrList = std::list< using_boost::modules::IModuleUptr >;


void test_logRotation( int argc, char** argv )
{

     const auto logDir = "./logs";
     const auto rotationSize = 10u * 1024u;
     const auto maxFiles = 5u;
     const auto minLevel = boost::log::trivial::debug;

     using_boost::log::logger::initialize(
          logDir
          , rotationSize
          , maxFiles
          , minLevel
          );

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
     }
     auto ms = 10u;
     for( auto idx: boost::irange( 5 ) )
     {
          BOOST_LOG_TRIVIAL( debug ) << "Iteration #" << idx << " STARTS...";
          std::for_each(
               std::begin( modules )
               , std::end( modules )
               , std::mem_fn( &using_boost::modules::IModule::run )
               );
          BOOST_LOG_TRIVIAL( trace ) << "Timeout: " << ms << " ms";
          boost::this_thread::sleep( boost::posix_time::milliseconds{ ms } );
          BOOST_LOG_TRIVIAL( debug ) << "Iteration #" << idx << " DONE!";
     }
}


struct Struct {
     Struct() {}
     ~Struct() {
          BOOST_LOG_TRIVIAL( info ) << "Struct::~Struct()";
     }
};


Struct s;


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          BOOST_LOG_TRIVIAL( info ) << "Start!";
     }
     catch( const std::exception& e )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: " << boost::diagnostic_information( e );
          return 1;
     }
     BOOST_LOG_TRIVIAL( info ) << "End.";
     return 0;
}
