/// @file
/// @brief


#include <stdexcept>
#include <iostream>

#include <boost/variant.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/utility/string_view.hpp>


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


struct Printer
{
     explicit Printer( boost::string_view sv ) : _{ sv } { std::cout << __PRETTY_FUNCTION__ << '\n'; }
     explicit Printer( std::istream& is ) : _{ is } { std::cout << __PRETTY_FUNCTION__ << '\n'; }
     explicit Printer( const char* data, std::size_t size ) : Printer{{ data, size }} { std::cout << "2nd: " << __PRETTY_FUNCTION__ << '\n'; }

     boost::variant< boost::string_view, std::istream& > _;
};


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          static constexpr auto text = "text";
          static const char cc[] = { 't','e','x','t' };

          Printer{ text };
          Printer{ std::cin };
          Printer( &cc[ 0 ], sizeof( cc ) );
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
