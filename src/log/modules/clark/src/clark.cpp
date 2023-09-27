/// @file clark.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <log/modules/clark/clark.h>

#include <log/logger/logger.h>


extern "C" {

using_boost::modules::IModule* create()
{
     return new using_boost::modules::Clark{};
}

}


namespace using_boost {
namespace modules {


const char* Clark::name() const noexcept
{
     return "Clark";
}

void Clark::init()
{
     LOGGER( info ) << "Hello, I am " << name() << " module, and I am initializing...";
}


void Clark::run()
{
     LOGGER( info ) << "Hi, I'm still " << name() << " and now I am running!";
}


} // namespace modules
} // namespace using_boost
