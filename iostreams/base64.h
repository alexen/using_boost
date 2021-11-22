/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#pragma once

#include <iosfwd>
#include <initializer_list>
#include <boost/utility/string_view.hpp>


namespace using_boost {
namespace iostreams {
namespace base64 {


void encode( std::istream&, std::ostream& );
void decode( std::istream&, std::ostream&, boost::string_view ignored = {} );


} // namespace base64
} // namespace iostreams
} // namespace using_boost
