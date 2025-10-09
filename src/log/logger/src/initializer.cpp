/// @file initializer.cpp
/// @brief
/// @copyright Copyright 2024 InfoTeCS Internet Trust

#include <log/logger/initializer.h>

#include <signal.h>
#include <unistd.h>

#include <functional>

#include <boost/log/core/core.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sinks/text_file_backend.hpp>


BOOST_LOG_ATTRIBUTE_KEYWORD( Severity, "Severity", boost::log::trivial::severity_level )


namespace using_boost {
namespace log {
namespace logger {


namespace {

std::function< void( int ) > logRotator;
void signalHandler( int signum )
{
     logRotator( signum );
}


} // namespace {anonymous}


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

     signal( SIGINT, signalHandler );
     signal( SIGTERM, signalHandler );

     logRotator = [ sink ]( int n ){
          std::cout << "Caught signal #" << n << ", do force rotation!\n";
          sink->locked_backend()->rotate_file();
          std::cout << "Set default signal handler for #" << n << '\n';
          signal( n, SIG_DFL );
          std::cout << "Raise (rethrow) signal #" << n << '\n';
          raise( n );
     };

//     std::cout
//          << "Found logs: " << files
//          << ", current file: " << sink->locked_backend()->get_current_file_name().string()
//          << '\n';
//     //     BOOST_LOG_TRIVIAL( info ) << "Dumb message to initiate log rotation";
//     sink->locked_backend()->enable_final_rotation( false );
//     sink->locked_backend()->rotate_file();
}


} // namespace logger
} // namespace log
} // namespace using_boost
