/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <stdexcept>
#include <iostream>
#include <fstream>

#include <boost/make_shared.hpp>

#include <boost/core/ignore_unused.hpp>
#include <boost/core/null_deleter.hpp>

#include <boost/exception/diagnostic_information.hpp>

#include <boost/log/trivial.hpp>

#include <log/init.h>


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          namespace init_ = using_boost::log::init;

          init_::addSink( boost::make_shared< std::ofstream >( "log.txt" ) );
          init_::addSink( boost::shared_ptr< std::ostream >{ &std::clog, boost::null_deleter{} } );
          init_::addSink( boost::shared_ptr< std::ostream >{ &std::cout, boost::null_deleter{} } );

          BOOST_LOG_TRIVIAL( info ) << "This is simple logger";
          BOOST_LOG_TRIVIAL( trace ) << "Tracing message";

          BOOST_THROW_EXCEPTION( std::runtime_error{ "error" } );
     }
     catch( const std::exception& e )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: " << boost::diagnostic_information( e );
          return 1;
     }
     return 0;
}
