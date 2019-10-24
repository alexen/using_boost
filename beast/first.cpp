/// @file first.cpp
/// @brief
///
/// @copyright Copyright 2019 InfoTeCS.

#include <stdexcept>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http/buffer_body.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>


using tcp = boost::asio::ip::tcp;
namespace http = boost::beast::http;


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          static constexpr auto path = "/";
          static constexpr auto httpVersion = 10;

          boost::asio::io_context ioctx;

          tcp::resolver resolver{ ioctx };
          tcp::socket socket{ ioctx };

          boost::asio::connect( socket, resolver.resolve( "localhost", "9999" ) );

          http::request< http::string_body > request{ http::verb::get, path, httpVersion };
          request.set( http::field::host, "localhost" );
          request.set( http::field::user_agent, __PRETTY_FUNCTION__ );

          http::write( socket, request );
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
