/// @file main.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <stdexcept>
#include <iostream>

#include <boost/thread/thread.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/exceptions.hpp>


struct ShmemAutorm {
     ShmemAutorm( const char* name ) : name{ name } {}
     ~ShmemAutorm() try {
          std::cout << __PRETTY_FUNCTION__ << std::endl;
          boost::interprocess::shared_memory_object::remove( name );
     }
     catch( ... ) {}

     const char* const name;
};

int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          static constexpr auto shmemName = "singleton";

          boost::interprocess::shared_memory_object shmem{
               boost::interprocess::create_only,
               shmemName,
               boost::interprocess::read_write
          };
          ShmemAutorm _{ shmemName };
          std::cout << "Yep, I am working!" << std::endl;
          boost::this_thread::sleep( boost::posix_time::seconds{ 45 } );

     }
     catch( const boost::interprocess::interprocess_exception& e )
     {
          std::cout << "Sorry, I am fucked..." << '\n'
               << boost::diagnostic_information( e ) << std::endl;
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
