/// @file
/// @brief

#pragma once

#include <boost/log/core/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/keywords/file_name.hpp>
#include <boost/log/keywords/rotation_size.hpp>
#include <boost/log/keywords/format.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/utility/string_view.hpp>


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


} // namespace init
} // namespace log
} // namespace using_boost
