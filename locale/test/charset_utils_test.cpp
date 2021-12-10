/// @file
/// @brief


#include <boost/test/unit_test.hpp>

#include <locale/charset_utils.h>

#include <cstring>
#include <vector>
#include <fstream>


template< typename CharT = char >
static std::basic_string< CharT > makeString( std::initializer_list< unsigned char > ucc )
{
     return std::basic_string< CharT > {
          reinterpret_cast< const char* >( ucc.begin() ), ucc.size()
     };
}


BOOST_AUTO_TEST_SUITE( TestCharsetUtils )
BOOST_AUTO_TEST_CASE( TestUtf8ToFromUtf16 )
{
     static constexpr auto expectedUtf8Text  =  "Это строка русского текста.";
     static constexpr auto expectedUtf16Text = L"Это строка русского текста.";

     BOOST_TEST( expectedUtf8Text == edi::utf16ToUtf8( expectedUtf16Text ), boost::test_tools::per_element{} );
     BOOST_TEST( expectedUtf16Text == edi::utf8ToUtf16( expectedUtf8Text ), boost::test_tools::per_element{} );
}
BOOST_AUTO_TEST_CASE( TestUtf8ToFromCp1251 )
{
     using namespace std::literals;

     /// Строка текста на русском
     static const auto expectedUtf8Text = "Это строка русского текста."s;
     /// Эта же строка, только в кодировке cp1251
     static const auto expectedCp1251Text = makeString({
          0xdd, 0xf2, 0xee, 0x20, 0xf1, 0xf2, 0xf0, 0xee, 0xea, 0xe0, 0x20, 0xf0,
          0xf3, 0xf1, 0xf1, 0xea, 0xee, 0xe3, 0xee, 0x20, 0xf2, 0xe5, 0xea, 0xf1,
          0xf2, 0xe0, 0x2e
     });

     BOOST_TEST( expectedCp1251Text == edi::utf8ToCp1251( expectedUtf8Text ), boost::test_tools::per_element{} );
     BOOST_TEST( expectedUtf8Text == edi::cp1251ToUtf8( expectedCp1251Text ), boost::test_tools::per_element{} );
}


BOOST_AUTO_TEST_CASE( TestUtf8ToFromOem )
{
     using namespace std::literals;

     /// Строка текста на русском
     static const auto expectedUtf8Text = "Это строка русского текста."s;
     /// Эта же строка, только в кодировке cp866
     static const auto expectedCp866Text = makeString({
          0x9d, 0xe2, 0xae, 0x20, 0xe1, 0xe2, 0xe0, 0xae, 0xaa, 0xa0, 0x20, 0xe0,
          0xe3, 0xe1, 0xe1, 0xaa, 0xae, 0xa3, 0xae, 0x20, 0xe2, 0xa5, 0xaa, 0xe1,
          0xe2, 0xa0, 0x2e
     });

     BOOST_TEST( expectedCp866Text == edi::utf8ToOem( expectedUtf8Text ), boost::test_tools::per_element{} );
     BOOST_TEST( expectedUtf8Text == edi::oemToUtf8( expectedCp866Text ), boost::test_tools::per_element{} );
}
BOOST_AUTO_TEST_SUITE_END()
