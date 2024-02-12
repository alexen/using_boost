/// @file alice.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <log/modules/imodule.h>


namespace using_boost {
namespace modules {


class Clark: public IModule
{
public:
     Clark();

     const char* name() const noexcept override;

     void init() override;
     void run() override;
};


} // namespace modules
} // namespace using_boost


extern "C" {


using_boost::modules::IModule* create();


}
