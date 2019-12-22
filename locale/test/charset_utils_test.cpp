/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <locale/charset_utils.h>

#include <gtest/gtest.h>
#include <cstring>
#include <vector>
#include <fstream>


using UcharBuffer = std::vector< unsigned char >;


std::string toString( const UcharBuffer& buffer )
{
     return std::string{ reinterpret_cast< const char* >( buffer.data() ), buffer.size() };
}


template< typename CharT >
bool isEqual( const std::basic_string< CharT >& str, const UcharBuffer& buffer )
{
     /// Для сравнения char и unsigned char используем низкоуровневую функцию
     /// std::memcmp() из string.h (он же cstring). Алгоритм std::equal() и прочие
     /// здесь не годятся.
     return str.size() == buffer.size()
          && 0 == std::memcmp( str.data(), buffer.data(), str.size() );
}


TEST( CharsetUtils, Utf8ToFromUtf16 )
{
     static constexpr auto expectedUtf8Text  =  "Это строка русского текста.";
     static constexpr auto expectedUtf16Text = L"Это строка русского текста.";

     ASSERT_EQ( expectedUtf8Text, edi::utf16ToUtf8( expectedUtf16Text ) );
     ASSERT_EQ( expectedUtf16Text, edi::utf8ToUtf16( expectedUtf8Text ) );
}


TEST( CharsetUtils, Utf8ToFromCp1251 )
{
     using namespace std::literals;

     /// Строка текста на русском
     static const auto expectedUtf8Text = "Это строка русского текста."s;
     /// Эта же строка, только в кодировке cp1251
     static const UcharBuffer expectedCp1251Text {
          0xdd, 0xf2, 0xee, 0x20, 0xf1, 0xf2, 0xf0, 0xee, 0xea, 0xe0, 0x20, 0xf0,
          0xf3, 0xf1, 0xf1, 0xea, 0xee, 0xe3, 0xee, 0x20, 0xf2, 0xe5, 0xea, 0xf1,
          0xf2, 0xe0, 0x2e
     };

     ASSERT_TRUE( isEqual( edi::utf8ToCp1251( expectedUtf8Text ), expectedCp1251Text ) );
     ASSERT_EQ( expectedUtf8Text, edi::cp1251ToUtf8( toString( expectedCp1251Text ) ) );
}


TEST( CharsetUtils, Utf8ToFromOem )
{
     using namespace std::literals;

     /// Строка текста на русском
     static const auto expectedUtf8Text = "Это строка русского текста."s;
     /// Эта же строка, только в кодировке cp866
     static const UcharBuffer expectedCp866Text {
          0x9d, 0xe2, 0xae, 0x20, 0xe1, 0xe2, 0xe0, 0xae, 0xaa, 0xa0, 0x20, 0xe0,
          0xe3, 0xe1, 0xe1, 0xaa, 0xae, 0xa3, 0xae, 0x20, 0xe2, 0xa5, 0xaa, 0xe1,
          0xe2, 0xa0, 0x2e
     };

     ASSERT_TRUE( isEqual( edi::utf8ToOem( expectedUtf8Text ), expectedCp866Text ) );
     ASSERT_EQ( expectedUtf8Text, edi::oemToUtf8( toString( expectedCp866Text ) ) );
}
