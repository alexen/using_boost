/// @file
/// @brief


#pragma once

#include <string>


namespace edi {


std::wstring utf8ToUtf16( const std::string& utf8 );
std::string utf16ToUtf8( const std::wstring& utf16 );

std::string utf8ToCp1251( const std::string& utf8 );
std::string cp1251ToUtf8( const std::string& utf16 );

std::string utf8ToOem( const std::string& utf8 );
std::string oemToUtf8( const std::string& utf16 );


} // namespace edi
