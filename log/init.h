/// @file
/// @brief

#pragma once

#include <boost/make_shared.hpp>

#include <boost/utility/string_view.hpp>

#include <boost/log/core/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/keywords/file_name.hpp>
#include <boost/log/keywords/rotation_size.hpp>
#include <boost/log/keywords/format.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>


inline std::size_t operator ""_KB( long long unsigned kb )
{
     return kb * 1024;
}


namespace using_boost {
namespace log {
namespace init {


inline void limitSeverity(
     boost::log::trivial::severity_level severity
     , boost::log::core_ptr core = boost::log::core::get()
)
{
     core->set_filter( boost::log::trivial::severity >= severity );
}


inline void setLogRotation( boost::string_view logMask, const std::size_t maxBytes = 10_KB )
{
     boost::log::add_file_log(
          boost::log::keywords::file_name = logMask.to_string(),
          boost::log::keywords::rotation_size = maxBytes,
          boost::log::keywords::format = "[%TimeStamp%]: %Message%"
          );
     /// Эта запись нужна, чтобы работал плейсхолдер %TimeStamp%
     boost::log::add_common_attributes();
}


void addSyslogSink()
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
     sink->set_formatter(
          boost::log::expressions::format( "<%1%>: %2%" )
               % boost::log::trivial::severity
               % boost::log::expressions::smessage
          );
     boost::log::core::get()->add_sink( sink );
}


void addSink( boost::shared_ptr< std::ostream >&& os )
{
     using Backend = boost::log::sinks::text_ostream_backend;
     using Sink = boost::log::sinks::synchronous_sink< Backend >;

     auto sink = boost::make_shared< Sink >();
     sink->locked_backend()->add_stream( os );

     boost::log::core::get()->add_sink( sink );
}


} // namespace init
} // namespace log
} // namespace using_boost
