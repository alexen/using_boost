/// @file imodule.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <list>
#include <memory>


namespace using_boost {
namespace modules {


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