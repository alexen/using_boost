/// @file types.h
/// @brief
/// @copyright Copyright 2023 InfoTeCS Internet Trust

#pragma once

#include <memory>
#include <stdexcept>

#include <boost/exception/exception.hpp>
#include <boost/exception/error_info.hpp>


namespace using_boost {
namespace modules {
namespace dynlib {
namespace native {


using Handler = void*;
using HandlerDeleter = void(*)( Handler );


} // namespace native


using DynLibUptr = std::unique_ptr< std::remove_pointer_t< native::Handler >, native::HandlerDeleter >;


struct DynLibError : std::runtime_error, virtual boost::exception
{
     explicit DynLibError( const std::string& what ) : std::runtime_error{ what } {}
};


using DynLibErrorDetails = boost::error_info< struct DynLibErrorDetails_, std::string >;


} // namespace dynlib
} // namespace modules
} // namespace using_boost
