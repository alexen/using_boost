/// @file main.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <stdexcept>
#include <iostream>

#include <boost/thread.hpp>
#include <boost/log/trivial.hpp>
#include <boost/core/ignore_unused.hpp>
#include <boost/exception/diagnostic_information.hpp>


struct StaticForThread
{
     StaticForThread()
     {
          BOOST_LOG_TRIVIAL( info ) << __PRETTY_FUNCTION__;
     }
};


void f()
{
     static boost::thread_specific_ptr< StaticForThread > ptr;
     if( !ptr.get() )
     {
          ptr.reset( new StaticForThread{} );
     }
     BOOST_LOG_TRIVIAL( info ) << "Called " << __PRETTY_FUNCTION__ << " from thread " << boost::this_thread::get_id();
     boost::this_thread::sleep( boost::posix_time::milliseconds{ 1 } );
}


void worker()
{
     for( auto i=0u; i<10; ++i )
     {
          f();
     }
}


void longWorker()
{
     try
     {
          std::cout << "Thread is starting\n";
          sleep( 600 );
     }
     catch( boost::thread_interrupted& )
     {
          std::cout << "Thread is finished!\n";
          throw;
     }
}


void stopThreadNoLock( boost::thread& th )
{
     th.interrupt();
     th.detach();
}

void stopThreadNoLock( const boost::shared_ptr< boost::thread >& th )
{
     if( th )
     {
          stopThreadNoLock( *th );
     }
}


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          boost::shared_ptr< boost::thread > t;
          while( true )
          {
               std::cout << "Enter command (tstart, tstop, exit): " << std::flush;
               std::string command;
               std::cin >> command;
               if( command == "tstart" )
               {
                    stopThreadNoLock( t );
                    t = boost::make_shared< boost::thread >( longWorker );
               }
               else if( command == "tstop" )
               {
                    stopThreadNoLock( t );
               }
               else if( command == "exit" )
               {
                    std::cout << "Good bye!\n";
                    return 0;
               }
               else
               {
                    std::cout << "Unrecognized command: " << command << ", please repeat enter\n";
               }
          }
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
