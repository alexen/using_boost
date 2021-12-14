/// @file
/// @brief

#include <log/application.h>

#include <iomanip>

#include <boost/utility/string_view.hpp>
#include <boost/log/trivial.hpp>
#include <boost/range/irange.hpp>
#include <boost/thread.hpp>


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
          BOOST_LOG_TRIVIAL( info ) << "Created handler " << std::quoted( name_ );
     }

     void run()
     {
          BOOST_LOG_TRIVIAL( info ) << "Handler " << std::quoted( name_ ) << " is running...";
          unsigned n = 10;
          for( auto i = 0u; i < n; ++i )
          {
               BOOST_LOG_TRIVIAL( debug ) << "Handler " << std::quoted( name_ )
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
     BOOST_LOG_TRIVIAL( info ) << "Application created";
}


Application::~Application()
{
     BOOST_LOG_TRIVIAL( info ) << "Application destroyed";
}


void Application::run()
{
     BOOST_LOG_TRIVIAL( info ) << "Application started";

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
          BOOST_LOG_TRIVIAL( info ) << "Starting handler \"" << name << '"';
          impl::Handler handler{ name };
          handler.run();
     }
     catch( const std::exception& e )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: " << boost::diagnostic_information( e );
     }
     catch( ... )
     {
          BOOST_LOG_TRIVIAL( error ) << "exception: unknown";
     }
}


} // namespace app
} // namespace log
} // namespace using_boost
