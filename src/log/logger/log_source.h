/// @file log_source.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <boost/log/trivial.hpp>
#include <boost/log/sources/logger.hpp>


#define LOGGER_SRC_SEV( logger, severity ) \
     BOOST_LOG_SEV( logger, boost::log::trivial::severity )

#define LOGGER_TRACE( logger ) LOGGER_SRC_SEV( logger, trace )
#define LOGGER_DEBUG( logger ) LOGGER_SRC_SEV( logger, debug )
#define LOGGER_INFO( logger )  LOGGER_SRC_SEV( logger, info )
#define LOGGER_WARN( logger )  LOGGER_SRC_SEV( logger, warning )
#define LOGGER_ERROR( logger ) LOGGER_SRC_SEV( logger, error )
#define LOGGER_FATAL( logger ) LOGGER_SRC_SEV( logger, fatal )


namespace using_boost {
namespace log {
namespace logger {


using LoggerSource = boost::log::sources::severity_logger< boost::log::trivial::severity_level >;


} // namespace logger
} // namespace log
} // namespace using_boost
