/// @file initializer.cpp
/// @brief
/// @copyright Copyright 2024 InfoTeCS Internet Trust

#include <log/logger/initializer.h>

#include <boost/log/core/core.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sinks/text_file_backend.hpp>


BOOST_LOG_ATTRIBUTE_KEYWORD( Severity, "Severity", boost::log::trivial::severity_level )


namespace using_boost {
namespace log {
namespace logger {


void initialize(
     const boost::filesystem::path& logDir
     , const std::size_t rotationSize
     , const unsigned maxFiles
     , boost::log::trivial::severity_level minLevel
)
{
     boost::log::core::get()->remove_all_sinks();
     boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >( "Severity" );
     boost::log::add_common_attributes();
     boost::log::add_console_log(
          std::cerr
          , boost::log::keywords::format = "%TimeStamp% {%ThreadID%} <%Severity%>: %Message%"
          )
          ->set_filter( Severity >= minLevel );

     auto sink = boost::log::add_file_log(
          boost::log::keywords::file_name = logDir / "using_boost_%Y-%m-%d_%4N.log"
          , boost::log::keywords::format = "[%TimeStamp%] *%Severity%*: %Message%"
          , boost::log::keywords::rotation_size = rotationSize
          , boost::log::keywords::open_mode = std::ios_base::out | std::ios_base::app
          , boost::log::keywords::auto_flush = true
          );

     sink->locked_backend()->set_file_collector(
          boost::log::sinks::file::make_collector(
               boost::log::keywords::target = logDir
               , boost::log::keywords::max_files = maxFiles
               )
          );

     sink->set_filter( Severity >= minLevel );
     sink->locked_backend()->scan_for_files();
}


} // namespace logger
} // namespace log
} // namespace using_boost
