/// @file
/// @brief

#pragma once

#include <boost/log/trivial.hpp>
#include <boost/log/utility/manipulators/add_value.hpp>
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
