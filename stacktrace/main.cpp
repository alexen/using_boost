/// @file
/// @brief

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <stacktrace/tools/funcs.h>
#include <stacktrace/stacktrace_exception.h>


struct Error : std::runtime_error, virtual boost::exception
{
     explicit Error( const std::string& what ) : std::runtime_error{ what } {}
};


using ErrorDetails = boost::error_info< struct ErrorDetails_, std::string >;
using ErrorCode = boost::error_info< struct ErrorCode_, int >;


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          THROW_EXCEPTION( Error{ "custom exception" }
               << ErrorDetails{ "with details" }
               << ErrorCode{ -171 } );
          THROW_EXCEPTION( Error{ "custom exception" } );
          THROW_EXCEPTION( std::runtime_error{ "std::runtime_error" } );
          THROW_EXCEPTION( std::system_error{ std::make_error_code( std::errc::invalid_seek ) } );

          using_boost::stacktrace::tools::main();
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
