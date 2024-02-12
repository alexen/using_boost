/// @file alice.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <log/modules/alice/alice.h>

#include <log/logger/logger.h>


extern "C" {

using_boost::modules::IModule* create()
{
     return new using_boost::modules::Alice{};
}

}


namespace using_boost {
namespace modules {


Alice::Alice() : IModule{ "Alice" }
{}

const char* Alice::name() const noexcept
{
     return "Alice";
}

void Alice::init()
{
     LOGGER( info ) << "Hello, I am " << name() << " module, and I am initializing...";
}


void Alice::run()
{
     LOGGER( info ) << "Hi, I'm still " << name() << " and now I am running!";
}


} // namespace modules
} // namespace using_boost
