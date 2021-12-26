/// @file
/// @brief

#pragma once

#include <boost/log/trivial.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>


#define LOGGER( severity ) \
     BOOST_LOG_TRIVIAL( severity ) \
          << boost::log::add_value( "File", __FILE__ ) \
          << boost::log::add_value( "Line", __LINE__ ) \
          << boost::log::add_value( "Function", __FUNCTION__ ) \
          << boost::log::add_value( "PrettyFunction", __PRETTY_FUNCTION__ )


namespace using_boost {
namespace log {
namespace logger {
namespace init {
namespace sinks {


void syslog();


} // namespace sinks
} // namespace init
} // namespace logger
} // namespace log
} // namespace using_boost
