/// @file
/// @brief

#include <celero/Celero.h>

#include <syslog.h>
#include <sstream>
#include <iostream>

#include <boost/thread/thread.hpp>

#include <log/logger/logger.h>


#define LOGGER_( severity, strm ) \
     do { \
          std::ostringstream oss; \
          oss \
               << '{' \
               << getpid() \
               << '.' \
               << boost::this_thread::get_id() \
               << "} (" \
               << basename( __FILE__ ) \
               << ':' \
               << __LINE__ \
               << ") <" \
               << severity << "> (" << __FUNCTION__ << "): " << strm; \
               const auto& str = oss.str(); \
               syslog( severity, "%*s", static_cast< int >( str.size() ), str.data() ); \
     } while( false )


class LoggerBenchmark : public celero::TestFixture
{
public:
     static constexpr auto Samples = 3u;
     static constexpr auto Iterations = 5000u;

     LoggerBenchmark() : celero::TestFixture()
     {
          boost::log::core::get()->remove_all_sinks();
          boost::log::core::get()->add_sink( using_boost::log::logger::sinks::makeSyslogSink() );
     }

     void setUp( const celero::TestFixture::ExperimentValue& ) override
     {
          counter = 0;
     }

protected:
     unsigned counter = 0;
};


BASELINE_F( Logging, Syslog, LoggerBenchmark, LoggerBenchmark::Samples, LoggerBenchmark::Iterations )
{
     LOGGER_( LOG_INFO, "This is a text " << ++counter );
}


BENCHMARK_F( Logging, BoostLog, LoggerBenchmark, LoggerBenchmark::Samples, LoggerBenchmark::Iterations )
{
     LOGGER( info ) << "This is a text " << ++counter;
}
