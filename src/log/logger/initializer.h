/// @file initializer.h
/// @brief
/// @copyright Copyright 2024 InfoTeCS Internet Trust

#pragma once

#include <boost/filesystem/path.hpp>
#include <boost/log/trivial.hpp>


namespace using_boost {
namespace log {
namespace logger {


void initialize(
     const boost::filesystem::path& logDir = "./logs"
     , std::size_t rotationSize = 10u * 1024u
     , unsigned maxFiles = 5u
     , boost::log::trivial::severity_level minLevel = boost::log::trivial::info
);


} // namespace logger
} // namespace log
} // namespace using_boost
