/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <stdexcept>
#include <iostream>
#include <sstream>

#include <boost/tokenizer.hpp>
#include <boost/coroutine2/all.hpp>
#include <boost/exception/diagnostic_information.hpp>


using coro_t = boost::coroutines2::coroutine< std::string >;

void readlines( std::istream& is, coro_t::push_type& sink )
{
     std::string line;
     while( std::getline( is, line ) )
     {
          sink( line );
     }
}


void tokenize( coro_t::pull_type& source, coro_t::push_type& sink )
{
     for( auto&& each: source )
     {
          for( auto&& token: boost::tokenizer<>{ each } )
          {
               sink( token );
          }
     }
}


void tag_wrap( const std::string& tagName, coro_t::pull_type& source, coro_t::push_type& sink )
{
     for( auto&& each: source )
     {
          sink( "<" + tagName + ">" + each + "</" + tagName + ">" );
     }
}


void put( coro_t::pull_type& source, std::ostream& os )
{
     for( auto&& each: source )
     {
          os << __FUNCTION__ << ": " << each << std::endl;
     }
}


int main( int argc, char** argv )
{
     try
     {
          const std::string russianLyric =
               "О, сколько нам открытий чудных\n"
               "Готовит просвещения дух!\n"
               "И опыт, сын ошибок трудных,\n"
               "И гений, парадоксов друг,\n"
               "И случай, бог изобретатель...\n";

          const auto& data = russianLyric;

          std::istringstream istr{ data };
          coro_t::pull_type reader{ std::bind( readlines, std::ref( istr ), std::placeholders::_1 ) };
          coro_t::pull_type tokenizer{ std::bind( tokenize, std::ref( reader ), std::placeholders::_1 ) };
          coro_t::pull_type tagwrapper{ std::bind( tag_wrap, "line", std::ref( tokenizer ), std::placeholders::_1 ) };
          coro_t::pull_type writer{ std::bind( put, std::ref( tagwrapper ), std::ref( std::cout ) ) };
          writer();
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
