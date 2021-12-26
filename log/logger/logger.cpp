/// @file
/// @brief

#include <log/logger/logger.h>

#include <unistd.h>

#include <boost/log/attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/syslog_constants.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

#include <boost/make_shared.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/thread/thread.hpp>


BOOST_LOG_ATTRIBUTE_KEYWORD( TimeStamp, "TimeStamp", boost::log::attributes::local_clock::value_type )
BOOST_LOG_ATTRIBUTE_KEYWORD( Pid, "Pid", pid_t )
BOOST_LOG_ATTRIBUTE_KEYWORD( Ppid, "Ppid", pid_t )
BOOST_LOG_ATTRIBUTE_KEYWORD( Tid, "Tid", boost::thread::id )
BOOST_LOG_ATTRIBUTE_KEYWORD( FilePath, "File", boost::string_view )
BOOST_LOG_ATTRIBUTE_KEYWORD( FileLine, "Line", unsigned )
BOOST_LOG_ATTRIBUTE_KEYWORD( FuncName, "Function", boost::string_view )
BOOST_LOG_ATTRIBUTE_KEYWORD( PrettyFuncName, "PrettyFunction", boost::string_view )


namespace using_boost {
namespace log {
namespace logger {
namespace init {
namespace sinks {


namespace {
namespace aux {


void addCustomAttributes()
{
     const auto core = boost::log::core::get();

     core->add_global_attribute( "Pid", boost::log::attributes::make_function( &getpid ) );
     core->add_global_attribute( "Ppid", boost::log::attributes::make_function( &getppid ) );
     core->add_global_attribute( "Tid", boost::log::attributes::make_function( &boost::this_thread::get_id ) );
}


} // namespace aux
} // namespace {unnamed}



void syslog()
{
     using Backend = boost::log::sinks::syslog_backend;
     using Sink = boost::log::sinks::synchronous_sink< Backend >;

     auto syslogBackend = boost::make_shared< Backend >(
          boost::log::keywords::facility = boost::log::sinks::syslog::user,
          boost::log::keywords::use_impl = boost::log::sinks::syslog::native
     );
     syslogBackend->set_severity_mapper(
          boost::log::sinks::syslog::direct_severity_mapping< int >( "Severity" )
          );
     auto sink = boost::make_shared< Sink >( syslogBackend );
     boost::log::add_common_attributes();
     aux::addCustomAttributes();
     sink->set_formatter(
          boost::log::expressions::stream
               << '{' << Pid
               << '.' << Tid
               << "} (" << FilePath
               << ':' << FileLine
               << ") <" << boost::log::trivial::severity
               << "> " << boost::log::expressions::message
          );
     boost::log::core::get()->add_sink( sink );
}


void ostream( std::ostream& os )
{
     using Backend = boost::log::sinks::text_ostream_backend;
     using Sink = boost::log::sinks::synchronous_sink< Backend >;

     auto sink = boost::make_shared< Sink >();
     sink->locked_backend()->add_stream( boost::shared_ptr< std::ostream >{ &os, boost::null_deleter{} } );

     boost::log::add_common_attributes();
     aux::addCustomAttributes();
     sink->set_formatter(
          boost::log::expressions::stream
               << '[' << TimeStamp
               << "] {" << Pid
               << '.' << Tid
               << "} (" << FilePath
               << ':' << FileLine
               << ") <" << boost::log::trivial::severity
               << ">: " << boost::log::expressions::message
          );
     boost::log::core::get()->add_sink( sink );
}


} // namespace sinks
} // namespace init
} // namespace logger
} // namespace log
} // namespace using_boost
