/// @file
/// @brief

#pragma once

#include <stdexcept>
#include <boost/exception/exception.hpp>


namespace using_boost {
namespace stacktrace {
namespace tools {


struct FuncError : std::runtime_error, virtual boost::exception
{
     explicit FuncError( const std::string& what )
          : std::runtime_error{ what } {}
};


void main();

void funcA();
void funcB();
void funcC();


} // namespace tools
} // namespace stacktrace
} // namespace using_boost
