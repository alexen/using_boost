/// @file main.cpp
/// @brief
/// @copyright Copyright 2022 InfoTeCS Internet Trust

#include <stdexcept>
#include <iostream>

#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/exception/all.hpp>
#include <boost/variant/variant.hpp>

#include "error/exception.h"
#include "error/details/for_uuid.h"
#include "error/details/format.h"

#include "tools.h"


enum class Enum
{
     Field1,
     Field2
};

std::ostream& operator<<( std::ostream&, const Enum );


void func_a()
{
     using namespace std::string_literals;

     static const auto uuid = boost::uuids::string_generator{}( "84de4630-8fa0-4631-a905-87a6467a08d1" );
     BOOST_THROW_EXCEPTION( alexen::error::Exception{}
          << alexen::error::details::ErrorUuid( uuid )
          << alexen::error::details::ErrorDescribedUuid( "just random UUID"s, uuid )
          );
}


void func_fmt()
{
     BOOST_THROW_EXCEPTION( alexen::error::Exception{}
          << alexen::error::details::ErrorTextFormat{ boost::format( "formatted string num=%1% enum=%2%" ) % 1 % Enum::Field1 }
          );
}


using ErrorException = boost::error_info< struct Exception_, std::exception_ptr >;


void func_b()
{
     try
     {
          BOOST_THROW_EXCEPTION( std::runtime_error{ "this is std::runtime_error" } );
     }
     catch( const std::exception& e )
     {
          BOOST_THROW_EXCEPTION( alexen::error::Exception{}
               << alexen::error::details::ErrorDescription{ boost::diagnostic_information( e ) } );
     }
}


void func_c()
{
     try
     {
          BOOST_THROW_EXCEPTION( std::runtime_error{ "this is std::runtime_error" } );
     }
     catch( const std::exception& e )
     {
          BOOST_THROW_EXCEPTION( alexen::error::Exception{}
               << boost::errinfo_at_line{ __LINE__ }
               << boost::errinfo_type_info_name{ typeid( e ).name() }
               << boost::errinfo_nested_exception{ boost::current_exception() } );
     }
}

struct ExceptionOne : virtual std::exception, virtual boost::exception {};
struct ExceptionTwo : virtual std::exception, virtual boost::exception {};

struct JointException : virtual ExceptionOne, virtual ExceptionTwo {};


void func_d()
{
     try
     {
          BOOST_THROW_EXCEPTION( ExceptionOne{} );
     }
     catch( const JointException& e )
     {
          std::cout << "WARNING: " << boost::diagnostic_information( e ) << '\n';
     }
}


std::ostream& operator<<( std::ostream& os, const Enum e )
{
     switch( e )
     {
          case Enum::Field1:
               os << "FIELD_1";
               break;
          case Enum::Field2:
               os << "FIELD_2";
               break;
          default:
               os.setstate( std::ios_base::failbit );
     }
     return os;
}


int main( int argc, char **argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          func_d();
     }
     catch( const std::exception &e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
