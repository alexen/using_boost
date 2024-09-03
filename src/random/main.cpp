/// @file main.cpp
/// @brief
/// @copyright Copyright 2024 InfoTeCS Internet Trust

#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <boost/random.hpp>


void generateNumbers( unsigned count, std::ostream& os )
{
     static boost::random::mt19937 gen{ static_cast< std::uint32_t >( clock() ) };
     while( --count )
     {
          os << gen() << ' ';
     }
}


void generateBytes( unsigned count, std::ostream& os )
{
     static boost::random::mt19937 gen{ static_cast< std::uint32_t >( clock() ) };
     boost::random::uniform_int_distribution< std::uint8_t > distr{ 0x00, 0xff };
     std::generate_n(
          std::ostreambuf_iterator< char >{ os }
          , count
          , [ & ]{ return distr( gen ); }
          );
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          boost::random::mt19937 gen{ static_cast< boost::random::mt19937::result_type >( std::clock() ) };
          boost::random::uniform_int_distribution< std::uint8_t > distr{ 0x00, 0xff };
          unsigned n = 10;
          while( --n )
          {
               std::cout << (int) distr( gen ) << '\n';
          }
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
