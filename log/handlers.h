/// @file
/// @brief

#pragma once

#include <cstddef>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>


namespace using_boost {
namespace log {
namespace handler {
namespace mt_loggers {


BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT( Logger, boost::log::sources::severity_logger_mt<> )


} // namespace mt_loggers
namespace starter {


void run();


} // namespace starter


void loggingHandler40( std::size_t n );
void loggingHandler100( std::size_t n );
void loggingHandler210( std::size_t n );


} // namespace handler
} // namespace log
} // namespace using_boost
