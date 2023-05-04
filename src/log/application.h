/// @file
/// @brief

#pragma once

#include <boost/utility/string_view.hpp>


namespace using_boost {
namespace log {
namespace app {


class Application
{
public:
     Application();
     ~Application();

     void run();

private:
     void startHandler( boost::string_view );
};


} // namespace app
} // namespace log
} // namespace using_boost
