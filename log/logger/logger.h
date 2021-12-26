/// @file
/// @brief

#pragma once

#include <boost/log/trivial.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
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
namespace init {
namespace sinks {


void syslog();
void ostream( std::ostream& );


} // namespace sinks
} // namespace init
} // namespace logger
} // namespace log
} // namespace using_boost
