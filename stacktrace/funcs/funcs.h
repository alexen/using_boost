/// @file
/// @brief



#pragma once

#include <stdexcept>
#include <boost/exception/exception.hpp>


struct FuncError : std::runtime_error, virtual boost::exception
{
     explicit FuncError( const std::string& what )
          : std::runtime_error{ what } {}
};


void funcA();
void funcB();
void funcC();
