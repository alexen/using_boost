/// @file main.cpp
/// @brief
/// @copyright Copyright 2022 InfoTeCS Internet Trust

#include <stdexcept>
#include <iostream>

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include "tools.h"
#include "uuid_exception.h"


void func()
{
     using namespace std::string_literals;

     static const auto uuid = boost::uuids::string_generator{}( "84de4630-8fa0-4631-a905-87a6467a08d1" );
     BOOST_THROW_EXCEPTION( alexen::error::Exception{}
          << alexen::error::details::ErrorUuid( uuid )
          << alexen::error::details::ErrorDescribedUuid( "just random UUID"s, uuid )
          );
}


int main( int argc, char **argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          func();
     }
     catch( const std::exception &e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
