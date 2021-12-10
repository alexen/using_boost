/// @file
/// @brief


#include <stdexcept>
#include <iostream>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>
#include <boost/regex/v4/regex.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/utility/string_view.hpp>


std::tuple< std::string, std::string > parseFormalizedFileName( const std::string& fileName );
std::tuple< std::string, std::string > parseFormalizedFileName( const std::string& fileName, std::exception_ptr& eptr ) noexcept;



std::tuple< std::string, std::string > parseFormalizedFileName( const std::string& fileName )
{
     static constexpr auto expectedMatches = 2u;
     static const boost::regex re{ R"re(^(\d{3}-?\d{3}-?\d{6})_(\d{3}-?\d{3})_[0-9a-f]{32}\.zip$)re", boost::regex::icase };
     boost::smatch matches;
     if( !boost::regex_search( fileName, matches, re ) && matches.size() == expectedMatches + 1 )
     {
          BOOST_THROW_EXCEPTION( std::invalid_argument{ "invalid file name format: " + fileName } );
     }
     return std::make_tuple( matches[ 1 ], matches[ 2 ] );
}


std::tuple< std::string, std::string > parseFormalizedFileName( const std::string& fileName, std::exception_ptr& eptr ) noexcept
{
     try
     {
          return parseFormalizedFileName( fileName );
     }
     catch( ... )
     {
          eptr = std::current_exception();
     }
     return {};
}


#define PRINT_SORRY_IF( condition_, message_ ) \
     do { if( (condition_) ){ \
          std::cout << "sorry, some error occured: " << message_ << '\n'; } } while( false )


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          try
          {
               std::string rns, num;
               std::tie( rns, num ) = parseFormalizedFileName( "001-001-163500_001-001_218c384544b4489b9fb08c84bdc53183.ZIP" );
               std::cout
                    << "rns: " << rns << '\n'
                    << "num: " << num << '\n';

               std::tie( rns, num ) = parseFormalizedFileName( "BAD_PACKAGE_NAME.EXT" );
               std::cout
                    << "rns: " << rns << '\n'
                    << "num: " << num << '\n';
          }
          catch( const std::exception& e )
          {
               std::cout << "parsing file name error: " << e.what() << '\n';
          }

          std::exception_ptr eptr;
          {
               std::string rns, num;
               std::tie( rns, num ) = parseFormalizedFileName( "001-001-163500_001-001_218c384544b4489b9fb08c84bdc53183.ZIP", eptr );
               std::cout
                    << "rns: " << rns << '\n'
                    << "num: " << num << '\n';
               PRINT_SORRY_IF( eptr, "cannot parse file name" );

               std::tie( rns, num ) = parseFormalizedFileName( "BAD_PACKAGE_NAME.EXT", eptr );
               std::cout
                    << "rns: " << rns << '\n'
                    << "num: " << num << '\n';
               PRINT_SORRY_IF( eptr, "cannot parse file name" );
          }
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
