/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#pragma once

#include <iosfwd>
#include <initializer_list>


namespace base64 {


void encode( std::istream&, std::ostream& );
void decode( std::istream&, std::ostream&, std::initializer_list< char > ignored = {'\r','\n'} );


} // namespace base64
