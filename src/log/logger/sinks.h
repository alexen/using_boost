/// @file sinks.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/syslog_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>


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


using FileBackend = boost::log::sinks::text_file_backend;
using FileSink = boost::log::sinks::synchronous_sink< FileBackend >;
using FileSinkPtr = boost::shared_ptr< FileSink >;


class LogFileOptions {
public:
     LogFileOptions()
          : logRotateSize_{ 100u * 1024u * 1024u }
          , logDir_{ "logs" }
          , logFilePattern_{ "using_boost_%Y-%m-%d_%4N.log" }
          , maxLogFiles_{ 25 }
     {}

     LogFileOptions& logRotateSize( const std::size_t bytes )
     {
          logRotateSize_ = bytes;
          return *this;
     }
     LogFileOptions& logDir( const boost::filesystem::path& dir )
     {
          logDir_ = dir;
          return *this;
     }
     LogFileOptions& logFilePattern( const std::string& pattern )
     {
          logFilePattern_ = pattern;
          return *this;
     }
     LogFileOptions& maxLogFiles( const std::size_t n )
     {
          maxLogFiles_ = n;
          return *this;
     }

     std::size_t logRotateSize() const noexcept { return logRotateSize_; }
     const boost::filesystem::path& logDir() const noexcept { return logDir_; }
     const std::string& logFilePattern() const noexcept { return logFilePattern_; }
     std::size_t maxLogFiles() const noexcept { return maxLogFiles_; }

private:
     std::size_t logRotateSize_;
     boost::filesystem::path logDir_;
     std::string logFilePattern_;
     std::size_t maxLogFiles_;
};


FileSinkPtr makeFileSink( const LogFileOptions& options = {} );


} // namespace sinks
} // namespace logger
} // namespace log
} // namespace using_boost
