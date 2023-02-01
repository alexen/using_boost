/// @file
/// @brief

#include <exception>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <stacktrace/exception.h>
#include <stacktrace/funcs/funcs.h>


struct Error : virtual std::exception, virtual boost::exception {};


using ErrorDetails = boost::error_info< struct ErrorDetails_, std::string >;
using ErrorCode = boost::error_info< struct ErrorCode_, int >;


void func_z() try
{
     using_boost::stacktrace::tools::main();
}
catch( const std::exception& e )
{
     THROW_EXCEPTION( Error{} << ErrorDetails{ boost::diagnostic_information( e ) } );
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          func_z();

          THROW_EXCEPTION( Error{} << ErrorDetails{ "custom exception" } << ErrorCode{ 94 } );
          THROW_EXCEPTION( std::runtime_error{ "std::runtime_error" } );
          THROW_EXCEPTION( std::system_error{ std::make_error_code( std::errc::invalid_seek ) } );
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
