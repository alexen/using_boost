/// @file
/// @brief


#pragma once

#include <iosfwd>
#include <initializer_list>
#include <boost/utility/string_view.hpp>


namespace using_boost {
namespace iostreams {
namespace base64 {


void encode( std::istream&, std::ostream& );
void decode( std::istream&, std::ostream&, boost::string_view ignored = {} );


namespace url_safe {


void encode( std::istream&, std::ostream&, bool padding = false );
void decode( std::istream&, std::ostream&, boost::string_view ignored = {} );


} // namespace url_safe
} // namespace base64
} // namespace iostreams
} // namespace using_boost
