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
          using PositionalOptions = std::vector< std::string >;
          PositionalOptions posOpts;

          /// Общие параметры для всей программы
          boost::program_options::options_description generic{ "Generic options" };
          generic.add_options()
               ( "help", "show this help and quit" )
               ( "version", "show version and quit" )
               ;

          /// Параметры из некоторой другой категории, назовем их "контролирующими"
          boost::program_options::options_description control{ "Control options" };
          control.add_options()
               ( "implicit-value", boost::program_options::value< std::string >( &implicitValue )->implicit_value( "IMPLICIT" ), "some implicit value" )
               ;

          /// Параметры, которые должны обрабатываться, но не должны отображаться при печати справочной информации
          boost::program_options::options_description invisible{ "Invisible options" };
          invisible.add_options()
               ( "pos", boost::program_options::value< PositionalOptions >( &posOpts ), "some positional options (must be hidden!)" )
               ;

          const auto maxOptions = 3;
          boost::program_options::positional_options_description positional;
          positional.add( "pos", maxOptions );

          /// Делаем два набора опций: для отображения (печати справки) и общий набор всех параметров для парсинга
          ///
          /// - набор отображаемых параметров: "printable" = "generic" + "control"
          ///
          boost::program_options::options_description printable{ "Application options" };
          printable.add( generic ).add( control );
          ///
          /// - общий набор (всех!) параметров для парсинга: "printable" += "invisible"
          ///
          boost::program_options::options_description total{ "Total options" };
          total.add( printable ).add( invisible );

          /// Обрабатываем все имеющиеся параметры - "total"
          boost::program_options::variables_map vm;
          boost::program_options::store(
               boost::program_options::command_line_parser{ argc, argv }
                    .options( total )
                    .positional( positional )
                    .run()
               , vm
               );

          if( vm.count( "help" ) )
          {
               /// А вот отображаем на экран только "printable" параметры!
               std::cout << printable << '\n';
               return EXIT_SUCCESS;
          }
          if( vm.count( "version" ) )
          {
               std::cout << "1.2.3" << '\n';
               return EXIT_SUCCESS;
          }
          boost::program_options::notify( vm );

          if( vm.count( "implicit-value" ) )
          {
               std::cout << "* Implicit value: " << std::quoted( implicitValue ) << '\n';
          }
          if( vm.count( "pos" ) )
          {
               std::cout << "* Positional parameters:\n";
               for( auto&& each: posOpts )
               {
                    std::cout << "  - " << each << '\n';
               }
          }
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return EXIT_SUCCESS;
}
