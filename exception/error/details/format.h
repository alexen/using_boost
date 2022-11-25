/// @file format.h
/// @brief
/// @copyright Copyright 2022 InfoTeCS Internet Trust

#pragma once

#include <boost/format.hpp>


namespace alexen {
namespace error {
namespace details {


using ErrorTextFormat = boost::error_info< struct TextFormat_, boost::format >;


} // namespace details
} // namespace error
} // namespace alexen
