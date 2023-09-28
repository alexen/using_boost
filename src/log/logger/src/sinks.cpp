/// @file
/// @brief

#include <log/logger/sinks.h>

#include <boost/log/trivial.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/syslog_constants.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

#include <boost/make_shared.hpp>
#include <boost/core/null_deleter.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/thread/thread.hpp>
#include <boost/utility/string_view.hpp>

#include <boost/lambda/lambda.hpp>


BOOST_LOG_ATTRIBUTE_KEYWORD( TimeStamp, "TimeStamp", boost::log::attributes::local_clock::value_type )
BOOST_LOG_ATTRIBUTE_KEYWORD( ThreadId, "ThreadId", boost::thread::id )
BOOST_LOG_ATTRIBUTE_KEYWORD( FilePath, "File", boost::string_view )
BOOST_LOG_ATTRIBUTE_KEYWORD( FileLine, "Line", unsigned )
BOOST_LOG_ATTRIBUTE_KEYWORD( FuncName, "Function", boost::string_view )
BOOST_LOG_ATTRIBUTE_KEYWORD( PrettyFuncName, "PrettyFunction", boost::string_view )


namespace std {


boost::log::formatting_ostream& operator<<(
     boost::log::formatting_ostream& os
     , const boost::log::to_log_manip< boost::string_view, tag::FilePath >& file
)
{
     static constexpr auto filename =
          []( boost::log::formatting_ostream& os, const boost::filesystem::path& file ) -> boost::log::formatting_ostream&
          {
               return os << file.filename().string();
          };
     return filename( os, file.get().to_string() );
}


} // namespace std


namespace using_boost {
namespace log {
namespace logger {
namespace sinks {


namespace {
namespace aux {


void addCustomAttributes()
{
     const auto core = boost::log::core::get();

     core->add_global_attribute( "ThreadId", boost::log::attributes::make_function( &boost::this_thread::get_id ) );
}


} // namespace aux
} // namespace {unnamed}


SyslogSinkPtr makeSyslogSink()
{
     auto syslogBackend = boost::make_shared< SyslogBackend >(
          boost::log::keywords::facility = boost::log::sinks::syslog::user,
          boost::log::keywords::use_impl = boost::log::sinks::syslog::native
     );
     syslogBackend->set_severity_mapper(
          boost::log::sinks::syslog::direct_severity_mapping< int >( "Severity" )
          );
     auto sink = boost::make_shared< SyslogSink >( syslogBackend );
     sink->set_formatter(
          boost::log::expressions::stream
               << '{' << ThreadId
               << "} (" << FilePath
               << ':' << FileLine
               << ") <" << boost::log::trivial::severity
               << "> " << boost::log::expressions::message
          );
     boost::log::add_common_attributes();
     aux::addCustomAttributes();
     return sink;
}


OstreamSinkPtr makeOstreamSink( std::ostream& os )
{
     auto sink = boost::make_shared< OstreamSink >();
     sink->locked_backend()->add_stream(
          boost::shared_ptr< std::ostream >{ &os, boost::null_deleter{} }
     );
     sink->set_formatter(
          boost::log::expressions::stream
               << '[' << TimeStamp
               << "] {" << ThreadId
               << "} (" << FilePath
               << ':' << FileLine
               << ") <" << boost::log::trivial::severity
               << ">: " << boost::log::expressions::message
          );
     boost::log::add_common_attributes();
     aux::addCustomAttributes();
     return sink;
}


void initFileCollecting( const FileSinkPtr& file, const boost::filesystem::path& logDir )
{
     file->locked_backend()->set_file_collector(
          boost::log::sinks::file::make_collector(
               boost::log::keywords::target = logDir,
               boost::log::keywords::max_files = 25
               )
          );
}


FileSinkPtr makeFileSink( const LogFileOptions& options )
{
     static const auto limiterFor = []( boost::string_view eventName ){
          return [ eventName ]( std::ostream& ostr ){
               ostr
                    << "=====[ " << eventName << ": "
                    << boost::posix_time::second_clock::local_time()
                    << " ]====================\n";
          };
     };

     const auto sink = boost::make_shared< FileSink >(
          boost::log::keywords::file_name = options.logDir() / options.logFilePattern(),
          boost::log::keywords::rotation_size = options.logRotateSize(),
          boost::log::keywords::open_mode = std::ios_base::out | std::ios_base::app
          );

     initFileCollecting( sink, options.logDir() );

     sink->locked_backend()->scan_for_files();

     sink->set_formatter(
          boost::log::expressions::stream
               << TimeStamp
               << " [" << ThreadId
               << "] (" << FilePath
               << ':' << FileLine
               << ") <" << boost::log::trivial::severity
               << "> " << boost::log::expressions::message
          );

     sink->locked_backend()->set_open_handler( limiterFor( "log started" ) );
     sink->locked_backend()->set_close_handler( limiterFor( "log finished" ) );
     sink->locked_backend()->auto_flush( true );

     boost::log::add_common_attributes();
     aux::addCustomAttributes();

     return sink;
}

} // namespace sinks
} // namespace logger
} // namespace log
} // namespace using_boost
