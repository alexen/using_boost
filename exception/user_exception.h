/// @file user_exception.h
/// @brief
/// @copyright Copyright 2022 InfoTeCS Internet Trust

#pragma once

#include <exception>
#include <boost/exception/exception.hpp>
#include <boost/exception/info_tuple.hpp>


namespace alexen {
namespace error {


struct Exception : virtual std::exception, virtual boost::exception {};


namespace details {


using ErrorDescription = boost::error_info< struct ErrorDescription_, std::string >;
using ErrorCode        = boost::error_info< struct ErrorCode_, int >;


} // namespace details
} // namespace error
} // namespace alexen
