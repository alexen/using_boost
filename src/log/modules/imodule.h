/// @file imodule.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <list>
#include <memory>
#include <boost/log/trivial.hpp>


namespace using_boost {
namespace modules {


class IModule {
public:
     explicit IModule( const std::string& name )
          : name_{ name }
     {
          BOOST_LOG_TRIVIAL( info )
               << "Module created: " << name_;
     }
     virtual ~IModule() {
          BOOST_LOG_TRIVIAL( info )
               << "Module destroyed: " << name_;
     }

     virtual const char* name() const noexcept = 0;

     virtual void init() = 0;
     virtual void run() = 0;

private:
     const std::string name_;
};


using IModuleUptr = std::unique_ptr< IModule >;
using ModuleCreatorFn = IModule*(*)();


} // namespace modules
} // namespace using_boost