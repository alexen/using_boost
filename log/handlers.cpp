/// @file
/// @brief

#include <log/handlers.h>

#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>


namespace using_boost {
namespace log {
namespace handler {
namespace starter {


void run()
{
     boost::thread_group tg;
     tg.create_thread( boost::bind( handler::loggingHandler40, 30 ) );
     tg.create_thread( boost::bind( handler::loggingHandler210, 10 ) );
     tg.create_thread( boost::bind( handler::loggingHandler100, 20 ) );
     tg.join_all();
}


} // namespace starter


void loggingHandler40( std::size_t n )
{
     auto& logger = mt_loggers::Logger::get();

     for( auto i = 0u; i < n; ++i )
     {
          BOOST_LOG( logger ) << "handler is working...";
          boost::this_thread::sleep( boost::posix_time::milliseconds{ 40 } );
     }
}


void loggingHandler100( std::size_t n )
{
     auto& logger = mt_loggers::Logger::get();

     for( auto i = 0u; i < n; ++i )
     {
          BOOST_LOG( logger ) << "handler is working...";
          boost::this_thread::sleep( boost::posix_time::milliseconds{ 100 } );
     }
}


void loggingHandler210( std::size_t n )
{
     auto& logger = mt_loggers::Logger::get();

     for( auto i = 0u; i < n; ++i )
     {
          BOOST_LOG( logger ) << "handler is working...";
          boost::this_thread::sleep( boost::posix_time::milliseconds{ 210 } );
     }
}


} // namespace handler
} // namespace log
} // namespace using_boost