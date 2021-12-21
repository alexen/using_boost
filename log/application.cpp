/// @file
/// @brief

#include <log/application.h>

#include <iomanip>

#include <boost/utility/string_view.hpp>
#include <boost/range/irange.hpp>
#include <boost/thread.hpp>

#include <log/logger.h>


namespace using_boost {
namespace log {
namespace app {

namespace {
namespace impl {


class Handler
{
public:
     explicit Handler( boost::string_view name )
          : name_{ name.cbegin(), name.cend() }
     {
          LOGGER( info ) << "Created handler " << std::quoted( name_ );
     }

     void run()
     {
          LOGGER( info ) << "Handler " << std::quoted( name_ ) << " is running...";
          unsigned n = 10;
          for( auto i = 0u; i < n; ++i )
          {
               LOGGER( debug ) << "Handler " << std::quoted( name_ )
                    << ": work step #" << i << '/' << n;
               boost::this_thread::sleep( boost::posix_time::milliseconds{ 123 } );
          }
     }
private:
     const std::string name_;
};


} // namespace impl
} // namespace {unnamed}


Application::Application()
{
     LOGGER( info ) << "Application created";
}


Application::~Application()
{
     LOGGER( info ) << "Application destroyed";
}


void Application::run()
{
     LOGGER( info ) << "Application started";

     boost::thread_group tg;
     tg.create_thread( [ this ]{ startHandler( "Alice" ); } );
     tg.create_thread( [ this ]{ startHandler( "Bob" ); } );
     tg.create_thread( [ this ]{ startHandler( "Clark" ); } );
     tg.join_all();
}


void Application::startHandler( boost::string_view name )
{
     try
     {
          LOGGER( info ) << "Starting handler \"" << name << '"';
          impl::Handler handler{ name };
          handler.run();
     }
     catch( const std::exception& e )
     {
          LOGGER( error ) << "exception: " << boost::diagnostic_information( e );
     }
     catch( ... )
     {
          LOGGER( error ) << "exception: unknown";
     }
}


} // namespace app
} // namespace log
} // namespace using_boost
