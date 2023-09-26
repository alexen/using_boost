/// @file imodule.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

namespace using_boost {
namespace log {
namespace module {


class IModule {
public:
     virtual ~IModule() {}

     virtual const char* name() const noexcept = 0;

     virtual void init() = 0;
     virtual void run() = 0;
};


} // namespace module
} // namespace log
} // namespace using_boost