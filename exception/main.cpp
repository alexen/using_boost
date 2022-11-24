/// @file main.cpp
/// @brief
/// @copyright Copyright 2022 InfoTeCS Internet Trust

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include "tools.h"


int main( int argc, char **argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          alexen::tools::doSmth( alexen::tools::Enum::Field2 );
     }
     catch( const std::exception &e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
