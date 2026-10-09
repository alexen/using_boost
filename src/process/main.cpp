/// @file main.cpp
/// @brief Демонстрация Boost.Process: запуск внешней команды, перенаправление ввода/вывода
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <stdexcept>
#include <iostream>
#include <string>
#include <vector>

#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <boost/process/child.hpp>
#include <boost/process/io.hpp>
#include <boost/process/search_path.hpp>
#include <boost/process/system.hpp>


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          // 1. Поиск исполняемого файла в PATH
          auto lsPath = boost::process::search_path( "ls" );
          std::cout << "--- search_path ---\n"
               << "ls found at: " << lsPath << '\n';

          // 2. Запуск команды без захвата вывода (system — синхронный вызов)
          std::cout << "\n--- boost::process::system ---\n"
               << "Running 'echo Hello from Boost.Process':\n";
          int ret = boost::process::system(
               boost::process::search_path( "echo" ),
               "Hello from Boost.Process"
          );
          std::cout << "Exit code: " << ret << '\n';

          // 3. Запуск с захватом stdout
          std::cout << "\n--- pipe stdout ---\n"
               << "Running 'ls -la /tmp' and capturing output:\n" << std::flush;

          boost::process::ipstream out;
          boost::process::child ls(
               boost::process::search_path( "ls" ),
               "-la", "/tmp",
               boost::process::std_out > out
          );

          std::string line;
          std::vector< std::string > entries;
          while( std::getline( out, line ) )
          {
               entries.push_back( line );
          }
          ls.wait();

          std::cout << "Captured " << entries.size() << " lines, exit code: " << ls.exit_code() << '\n';
          for( const auto& entry : entries )
          {
               std::cout << "  " << entry << '\n';
          }
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}