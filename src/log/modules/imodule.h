/// @file imodule.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <list>
#include <memory>

#include <boost/log/trivial.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>


namespace using_boost {
namespace modules {


using LoggerSource = boost::log::sources::severity_logger< boost::log::trivial::severity_level >;


class IModule {
public:
     virtual ~IModule() {}

     virtual const char* name() const noexcept = 0;

     virtual void init() = 0;
     virtual void run() = 0;
};


using IModuleUptr = std::unique_ptr< IModule >;
using ModuleCreatorFn = IModule*(*)();


} // namespace modules
} // namespace using_boost


#define LOGGER_SRC_SEV( logger, severity ) \
     BOOST_LOG_SEV( logger, boost::log::trivial::severity )

#define LOGGER_TRACE( logger ) LOGGER_SRC_SEV( logger, trace )
#define LOGGER_DEBUG( logger ) LOGGER_SRC_SEV( logger, debug )
#define LOGGER_INFO( logger )  LOGGER_SRC_SEV( logger, info )
#define LOGGER_WARN( logger )  LOGGER_SRC_SEV( logger, warning )
#define LOGGER_ERROR( logger ) LOGGER_SRC_SEV( logger, error )
#define LOGGER_FATAL( logger ) LOGGER_SRC_SEV( logger, fatal )
