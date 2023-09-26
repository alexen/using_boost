/// @file alice.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <log/modules/imodule.h>


namespace using_boost {
namespace module {


class Clark: public log::module::IModule
{
public:
     const char* name() const noexcept override;

     void init() override;
     void run() override;
};


} // namespace module
} // namespace using_boost
