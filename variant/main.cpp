/// @file
/// @brief


#include <stdexcept>
#include <iostream>

#include <boost/variant.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>


enum class Number
{
     One, Two, Three
};


std::ostream& operator<<( std::ostream& os, const Number n )
{
     switch( n )
     {
          case Number::One:
               os << "One";
               break;
          case Number::Two:
               os << "Two";
               break;
          case Number::Three:
               os << "Three";
               break;
     }
     return os;
}


struct Data
{
     Data( const std::string& s ) : number{ s } {}
     Data( unsigned n ) : number{ n } {}
     Data( Number n ) : number{ n } {}

     boost::variant< std::string, unsigned, Number > number;
};


std::ostream& operator<<( std::ostream& os, const Data& d )
{
     os << d.number;
     return os;
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          std::cout << Data{ 50 } << '\n';
          std::cout << Data{ "1011" } << '\n';
          std::cout << Data{ Number::Three } << '\n';
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
