/// @file first.cpp
/// @brief
///


#include <stdexcept>
#include <iostream>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>

#include <boost/filesystem/path.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>


namespace ip = boost::asio::ip;
using tcp = ip::tcp;
namespace http = boost::beast::http;


//POST https://docs-test.fss.ru/ws-insurer-v11/FileOperationsLnPort HTTP/1.1
//Accept-Encoding: gzip,deflate
//Content-Type: text/xml;charset=UTF-8
//SOAPAction: "http://ru/ibs/fss/ln/ws/FileOperationsLn.wsdl/getPrivateLNData"
//Content-Length: 15358
//Host: docs-test.fss.ru
//Connection: Keep-Alive
//User-Agent: Apache-HttpClient/4.1.1 (java 1.5)


void main_simplePost( int, char** )
{
     static constexpr auto host = "docs-test.fss.ru";
     static constexpr auto protocol = "https";
     static constexpr auto path = "/ws-insurer-v11/FileOperationsLnPort";
     static constexpr auto httpVersion = 10u;

     static const boost::filesystem::path soapBodyPath = "./soap_body.xml";
     static constexpr auto soapAction =
          "http://ru/ibs/fss/ln/ws/FileOperationsLn.wsdl/getPrivateLNData";
     static constexpr auto contentType = "text/xml;charset=UTF-8";

     boost::asio::io_context ioctx;

     boost::asio::ssl::context sslctx{ boost::asio::ssl::context::sslv23_client };

     tcp::resolver resolver{ ioctx };
     boost::asio::ssl::stream< tcp::socket > stream{ ioctx, sslctx };

     boost::asio::connect( stream.next_layer(), resolver.resolve( host, protocol ) );

     stream.handshake( boost::asio::ssl::stream_base::client );

     boost::system::error_code error;
     http::file_body::value_type body;
     body.open( soapBodyPath.c_str(), boost::beast::file_mode::read, error );
     if( error )
     {
          BOOST_THROW_EXCEPTION( boost::system::system_error{ error } );
     }

     http::request< http::file_body > request{ http::verb::post, path, httpVersion };
     request.set( http::field::host, host );
     request.set( http::field::user_agent, __PRETTY_FUNCTION__ );
     request.set( http::field::content_length, std::to_string( body.size() ) );
     request.set( http::field::soapaction, soapAction );
     request.set( http::field::content_type, contentType );
     request.body() = std::move( body );

     http::write( stream, request );

     boost::beast::flat_buffer buffer;
     http::response< http::string_body > response;

     http::read( stream, buffer, response );

     if( response.result() == boost::beast::http::status::ok )
     {
          if( response.body().empty() )
          {
               BOOST_THROW_EXCEPTION( std::runtime_error{ "empty response received" } );
          }
          std::cout << response.body();
     }

     stream.shutdown();
}


void main_urlResolver( int, char** argv )
{
     std::cout << "IP addresses: \n";
     boost::asio::io_service io_service;
     boost::asio::ip::tcp::resolver resolver( io_service );
     boost::asio::ip::tcp::resolver::query query( argv[ 1 ], "" );
     for(
          boost::asio::ip::tcp::resolver::iterator i = resolver.resolve( query );
          i != boost::asio::ip::tcp::resolver::iterator();
          ++i
     )
     {
          boost::asio::ip::tcp::endpoint end = *i;
          std::cout << end.address() << ' ';
     }
     std::cout << '\n';
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          main_urlResolver( argc, argv );
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
