/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.


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
