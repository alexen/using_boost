/// @file clark.cpp
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#include <log/modules/clark/clark.h>

#include <log/logger/log_source.h>


extern "C" {

using_boost::modules::IModule* create( using_boost::log::logger::LoggerSource& log )
{
     return new using_boost::modules::Clark{ log };
}

}


namespace using_boost {
namespace modules {

Clark::Clark( log::logger::LoggerSource& log )
     : log_{ log }
{
     LOGGER_INFO( log_ ) << "Clark object initialization";
}


const char* Clark::name() const noexcept
{
     return "Clark";
}

void Clark::init()
{
     LOGGER_INFO( log_ ) << "Hello, I am " << name() << " module, and I am initializing...";
}


void Clark::run()
{
     LOGGER_INFO( log_ ) << "Hi, I'm still " << name() << " and now I am running!";
}


} // namespace modules
} // namespace using_boost
