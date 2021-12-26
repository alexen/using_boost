/// @file
/// @brief

#include <log/logger/logger.h>

#include <unistd.h>

#include <boost/log/expressions.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/attributes/current_thread_id.hpp>
#include <boost/log/sinks/syslog_constants.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>


BOOST_LOG_ATTRIBUTE_KEYWORD( ThreadId, "ThreadID", boost::log::attributes::current_thread_id::value_type )
BOOST_LOG_ATTRIBUTE_KEYWORD( FilePath, "File", boost::string_view )
BOOST_LOG_ATTRIBUTE_KEYWORD( FileLine, "Line", unsigned )
BOOST_LOG_ATTRIBUTE_KEYWORD( FuncName, "Function", boost::string_view )
BOOST_LOG_ATTRIBUTE_KEYWORD( PrettyFuncName, "PrettyFunction", boost::string_view )


namespace using_boost {
namespace log {
namespace logger {
namespace init {
namespace sinks {


void syslog()
{
     using Backend = boost::log::sinks::syslog_backend;
     using Sink = boost::log::sinks::synchronous_sink< Backend >;

     boost::log::formatter formatter =
          boost::log::expressions::stream
               << '{' << getpid()
               << '.' << ThreadId
               << '}'
               << '(' << FilePath
               << ':' << FileLine
               << ')'
               << '<' << boost::log::trivial::severity
               << '>'
               << ' ' << boost::log::expressions::message
               ;

     auto syslogBackend = boost::make_shared< Backend >(
          boost::log::keywords::facility = boost::log::sinks::syslog::user,
          boost::log::keywords::use_impl = boost::log::sinks::syslog::native
     );
     syslogBackend->set_severity_mapper(
          boost::log::sinks::syslog::direct_severity_mapping< int >( "Severity" )
          );
     auto sink = boost::make_shared< Sink >( syslogBackend );
     sink->set_formatter( formatter );
     boost::log::core::get()->add_sink( sink );
     boost::log::add_common_attributes();
}


} // namespace sinks
} // namespace init
} // namespace logger
} // namespace log
} // namespace using_boost
