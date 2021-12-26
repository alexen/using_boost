/// @file
/// @brief

#pragma once

#include <boost/log/trivial.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/utility/string_view.hpp>


#define LOGGER( severity ) \
     BOOST_LOG_TRIVIAL( severity ) \
          << boost::log::add_value< boost::string_view >( "File", __FILE__ ) \
          << boost::log::add_value< unsigned >( "Line", __LINE__ ) \
          << boost::log::add_value< boost::string_view >( "Function", __FUNCTION__ ) \
          << boost::log::add_value< boost::string_view >( "PrettyFunction", __PRETTY_FUNCTION__ )


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
