/// @file alice.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <log/modules/alice/alice.h>

#include <log/logger/log_source.h>


extern "C" {

using_boost::modules::IModule* create( using_boost::log::logger::LoggerSource& logger )
{
     return new using_boost::modules::Alice{ logger };
}

}


namespace using_boost {
namespace modules {


Alice::Alice( log::logger::LoggerSource& lg )
     : log_{ lg }
{
     LOGGER_INFO( log_ ) << "Alice object creation";
}


const char* Alice::name() const noexcept
{
     return "Alice";
}

void Alice::init()
{
     LOGGER_INFO( log_ ) << "Hello, I am " << name() << " module, and I am initializing...";
}


void Alice::run()
{
     LOGGER_INFO( log_ ) << "Hi, I'm still " << name() << " and now I am running!";
}


} // namespace modules
} // namespace using_boost
