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


int main( int argc, char** argv )
{
     boost::ignore_unused( argc, argv );
     try
     {
          boost::thread_group tg;
          tg.create_thread( worker );
          tg.create_thread( worker );
          tg.create_thread( worker );
          tg.create_thread( worker );
          tg.join_all();
     }
     catch( const std::exception& e )
     {
          std::cerr << "exception: " << boost::diagnostic_information( e ) << '\n';
          return 1;
     }
     return 0;
}
