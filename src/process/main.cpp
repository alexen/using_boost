/// @file main.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <boost/process/env.hpp>


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          const auto env = boost::this_process::environment();
          std::cout << env[ "SOME_VAR" ] << '\n';
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
