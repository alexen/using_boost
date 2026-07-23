/// @file
/// @brief

#include <set>
#include <string>
#include <iterator>
#include <stdexcept>
#include <iostream>

#include <boost/algorithm/algorithm.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/utility/string_view.hpp>


enum class Number
{
     One, Two, Three
};


static std::ostream& operator<<( std::ostream& os, const Number n )
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


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          std::string stack = "привет!";
          std::wstring needle = L"рив";

          std::cout << std::boolalpha << boost::algorithm::icontains( stack, needle, std::locale( "C.UTF-8" ) ) << '\n';

//          return 0;

          std::set< Number > nn {
               Number::One,
               Number::Two,
               Number::Three,
               Number::Two,
               Number::One
          };

          std::set< std::string > ss {
               "~~One~~",
               "~~Two~~",
               "~~Three~~"
          };

          std::string str = "HELLO, WORLD!";
          boost::algorithm::to_lower(str); // modifies str

          std::cout << str << '\n';

          for( auto&& each: nn )
          {
               std::cout << each << ' ';
          }
          std::cout << '\n';

//          boost::algorithm::join( nn, ", " );

          std::copy(
               std::cbegin( nn ),
               std::cend( nn ),
               std::ostream_iterator< Number >( std::cout, "/" )
               );

//          std::cout << '\n' << boost::algorithm::join( nums, ", " ) << '\n';
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
