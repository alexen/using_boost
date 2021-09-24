/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <boost/stacktrace.hpp>


void funcC()
{
     std::cout << boost::stacktrace::stacktrace();
}


void funcB()
{
     funcC();
}


void funcA()
{
     funcB();
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          funcA();
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
