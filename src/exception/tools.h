/// @file tools.h
/// @brief
/// @copyright Copyright 2022 InfoTeCS Internet Trust

#pragma once

#include <iosfwd>
#include <boost/exception/error_info.hpp>


namespace alexen {
namespace tools {


enum class Enum: char
{
     Field1 =  75,
     Field2 = 103,
     Field3 = 121
};


void doSmth( Enum );


} // namespace tools


namespace error {
namespace details {


using ErrorEnum = boost::error_info< struct ErrorEnum_, tools::Enum >;


} // namespace details
} // namespace error
} // namespace alexen
