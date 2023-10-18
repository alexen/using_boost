/// @file alice.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <log/modules/alice/alice.h>

#include <log/logger/logger.h>


namespace using_boost {
namespace modules {


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
