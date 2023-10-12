/// @file log_source.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <boost/log/trivial.hpp>
#include <boost/log/sources/logger.hpp>


#define LOG_SEV_( logger, severity ) \
     BOOST_LOG_SEV( logger, boost::log::trivial::severity )

#define LOG_TRACE( logger ) LOG_SEV_( logger, trace )
#define LOG_DEBUG( logger ) LOG_SEV_( logger, debug )
#define LOG_INFO( logger )  LOG_SEV_( logger, info )
#define LOG_WARN( logger )  LOG_SEV_( logger, warning )
#define LOG_ERROR( logger ) LOG_SEV_( logger, error )
#define LOG_FATAL( logger ) LOG_SEV_( logger, fatal )


namespace using_boost {
namespace log {
namespace logger {


using LoggerSource = boost::log::sources::severity_logger< boost::log::trivial::severity_level >;


} // namespace logger
} // namespace log
} // namespace using_boost
