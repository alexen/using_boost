/// @file main.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <iostream>
#include <stdexcept>

#include <boost/exception/diagnostic_information.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>


int main( int argc, char** argv )
{
     try
     {
          std::string implicitValue;

          /// Пояснения к параметрам:
          /// - implicit_value() -- можно указать значение (явное), которое будет использовано как значение ключа, если ключ указан без параметра;
          ///   <program_name>                        [Enter] => [nothing]
          ///   <program_name> --implicit-value       [Enter] => Implicit value: "IMPLICIT"
          ///   <program_name> --implicit-value Hello [Enter] => Implicit value: "Hello"
          ///
          boost::program_options::options_description description { "Options" };
          description.add_options()
               ( "help", "show this help and quit" )
               ( "implicit-value", boost::program_options::value< std::string >( &implicitValue )->implicit_value( "IMPLICIT" ), "some implicit value" );
          boost::program_options::variables_map options;
          boost::program_options::store(
               boost::program_options::parse_command_line(
                    argc,
                    argv,
                    description
                    ),
               options
               );
          if( options.count( "help" ) )
          {
               std::cout << description << '\n';
               return EXIT_SUCCESS;
          }
          boost::program_options::notify( options );

          if( options.count( "implicit-value" ) )
          {
               std::cout << "Implicit value: " << std::quoted( implicitValue ) << '\n';
          }
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return EXIT_SUCCESS;
}
