/// @file
/// @brief

#pragma once

#include <boost/log/trivial.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/utility/string_view.hpp>


#define LOGGER_SEV( lvl ) \
     BOOST_LOG_STREAM_WITH_PARAMS(::boost::log::trivial::logger::get(),\
        (::boost::log::keywords::severity = lvl)) \
          << boost::log::add_value< boost::string_view >( "File", __FILE__ ) \
          << boost::log::add_value< unsigned >( "Line", __LINE__ ) \
          << boost::log::add_value< boost::string_view >( "Function", __FUNCTION__ ) \
          << boost::log::add_value< boost::string_view >( "PrettyFunction", __PRETTY_FUNCTION__ )

#define LOGGER( lvl ) \
     LOGGER_SEV( ::boost::log::trivial::lvl )

namespace using_boost {
namespace log {
namespace logger {
namespace sinks {


using SyslogBackend = boost::log::sinks::syslog_backend;
using SyslogSink = boost::log::sinks::synchronous_sink< SyslogBackend >;
using SyslogSinkPtr = boost::shared_ptr< SyslogSink >;


SyslogSinkPtr makeSyslogSink();


using OstreamBackend = boost::log::sinks::text_ostream_backend;
using OstreamSink = boost::log::sinks::synchronous_sink< OstreamBackend >;
using OstreamSinkPtr = boost::shared_ptr< OstreamSink >;


OstreamSinkPtr makeOstreamSink( std::ostream& );


} // namespace sinks
} // namespace logger
} // namespace log
} // namespace using_boost
