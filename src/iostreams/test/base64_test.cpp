/// @file
/// @brief


#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>

#include <iostreams/base64.h>


namespace {
namespace test_env {
namespace text {


static const auto decoded = {
     "",
     "1",
     "12",
     "123",
     "1234",
     "12345",
     "123456",
     "1234567",
     "12345678",
     "123456789",
     "1234567890",
};


static const auto encoded = {
     "",
     "MQ==",
     "MTI=",
     "MTIz",
     "MTIzNA==",
     "MTIzNDU=",
     "MTIzNDU2",
     "MTIzNDU2Nw==",
     "MTIzNDU2Nzg=",
     "MTIzNDU2Nzg5",
     "MTIzNDU2Nzg5MA==",
};


} // namespace text
namespace bin {


static constexpr std::uint8_t decoded[] = {
     0x08, 0x39, 0x65, 0xe9, 0xaa, 0x7f, 0x3d, 0xf3, 0xd1, 0xae, 0x90, 0xeb,
     0x42, 0x60, 0x15, 0x8c, 0xd0, 0x34, 0x42, 0x9c, 0xc7, 0x4a, 0x85, 0xb3,
     0x0f, 0x88, 0xc0, 0x83, 0x3f, 0x22, 0xcf, 0x63, 0xd0, 0xce, 0x8f, 0x1a,
     0xd2, 0x57, 0x97, 0x65, 0xe4, 0xee, 0xa3, 0xe5, 0x8c, 0x4b, 0x8c, 0xdc,
     0xf3, 0xe5, 0x86, 0x5a, 0x6a, 0xc1, 0xa8, 0xfc, 0x2f, 0x8b, 0x4d, 0x5c,
     0xbd, 0xd0, 0x62, 0x6d, 0xd1, 0xa6, 0xe6, 0xd5, 0x85, 0xfe, 0x3b, 0x09,
     0x73, 0x7b, 0x69, 0x1d, 0x09, 0xe9, 0x9a, 0xac, 0xf3, 0x30, 0x17, 0x68,
     0xfa, 0x20, 0x26, 0x15, 0x48, 0x27, 0xac, 0xfd, 0x08, 0x95, 0x80, 0xf8,
     0xf0, 0x85, 0xf8, 0x9d, 0x96, 0x53, 0xbd, 0x2e, 0x4b, 0xdb, 0xbc, 0xb2,
     0xe1, 0xc0, 0x31, 0xab, 0x98, 0x68, 0x6f, 0xba, 0x91, 0x02, 0x9a, 0x2e,
     0x31, 0x66, 0x38, 0x4e, 0xa1, 0xe7, 0xc2, 0x1f, 0xc8, 0x49, 0xbc, 0x65,
     0xde, 0xb4, 0x30, 0x97, 0x50, 0x67, 0x88, 0x39, 0xbd, 0x07, 0xdf, 0xe9,
     0xfc, 0x3d, 0xc9, 0x1b, 0x85, 0xda, 0x9f, 0x3c, 0x5d, 0x7f, 0x73, 0xb4,
     0xd8, 0xf7, 0xfd, 0xfa, 0x44, 0xf1, 0x38, 0x29, 0x03, 0x28, 0xec, 0x51,
     0x68, 0x27, 0x87, 0x90, 0x97, 0xe2, 0x0a, 0xe1, 0x2f, 0x9c, 0xd9, 0xfe,
     0x26, 0x0f, 0x13, 0x93, 0x70, 0xf7, 0x86, 0x60, 0x79, 0x51, 0x34, 0x66,
     0x4e, 0x14, 0x37, 0xa3, 0xcd, 0xdb, 0x58, 0x7e, 0xa5, 0xb7, 0x56, 0x5b,
     0x14, 0x23, 0x7f, 0x36, 0xed, 0x8b, 0xf5, 0x3c, 0xbe, 0xe4, 0x55, 0x0b,
     0xc2, 0xcf, 0x8c, 0x81, 0x6c, 0x6f, 0x75, 0x55, 0x15, 0xef, 0x24, 0x81,
     0x01, 0xbd, 0x7a, 0x6e, 0x61, 0x36, 0x28, 0xe7, 0x8c, 0xa3, 0x18, 0x9a,
     0x8b, 0x7f, 0xfd, 0x1d, 0x3e, 0xef, 0x1a, 0xa4, 0xef, 0xce, 0x81, 0x82,
     0xb0, 0x59, 0xfc, 0xfe
};


static constexpr auto encoded =
     "CDll6ap/PfPRrpDrQmAVjNA0QpzHSoWzD4jAgz8iz2PQzo8a0leXZeTuo+WMS4zc8+WGWmrBqPwv"
     "i01cvdBibdGm5tWF/jsJc3tpHQnpmqzzMBdo+iAmFUgnrP0IlYD48IX4nZZTvS5L27yy4cAxq5ho"
     "b7qRApouMWY4TqHnwh/ISbxl3rQwl1BniDm9B9/p/D3JG4Xanzxdf3O02Pf9+kTxOCkDKOxRaCeH"
     "kJfiCuEvnNn+Jg8Tk3D3hmB5UTRmThQ3o83bWH6lt1ZbFCN/Nu2L9Ty+5FULws+MgWxvdVUV7ySB"
     "Ab16bmE2KOeMoxiai3/9HT7vGqTvzoGCsFn8/g==";


namespace url_safe {
namespace with_padding {


static constexpr auto encoded =
     "CDll6ap_PfPRrpDrQmAVjNA0QpzHSoWzD4jAgz8iz2PQzo8a0leXZeTuo-WMS4zc8-WGWmrBqPwv"
     "i01cvdBibdGm5tWF_jsJc3tpHQnpmqzzMBdo-iAmFUgnrP0IlYD48IX4nZZTvS5L27yy4cAxq5ho"
     "b7qRApouMWY4TqHnwh_ISbxl3rQwl1BniDm9B9_p_D3JG4Xanzxdf3O02Pf9-kTxOCkDKOxRaCeH"
     "kJfiCuEvnNn-Jg8Tk3D3hmB5UTRmThQ3o83bWH6lt1ZbFCN_Nu2L9Ty-5FULws-MgWxvdVUV7ySB"
     "Ab16bmE2KOeMoxiai3_9HT7vGqTvzoGCsFn8_g==";


} // namespace with_padding
namespace without_padding {


static constexpr auto encoded =
     "CDll6ap_PfPRrpDrQmAVjNA0QpzHSoWzD4jAgz8iz2PQzo8a0leXZeTuo-WMS4zc8-WGWmrBqPwv"
     "i01cvdBibdGm5tWF_jsJc3tpHQnpmqzzMBdo-iAmFUgnrP0IlYD48IX4nZZTvS5L27yy4cAxq5ho"
     "b7qRApouMWY4TqHnwh_ISbxl3rQwl1BniDm9B9_p_D3JG4Xanzxdf3O02Pf9-kTxOCkDKOxRaCeH"
     "kJfiCuEvnNn-Jg8Tk3D3hmB5UTRmThQ3o83bWH6lt1ZbFCN_Nu2L9Ty-5FULws-MgWxvdVUV7ySB"
     "Ab16bmE2KOeMoxiai3_9HT7vGqTvzoGCsFn8_g";


} // namespace without_padding
} // namespace url_safe
} // namespace bin
namespace multiline {


static constexpr auto decoded =
     "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod "
     "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, "
     "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo "
     "consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse "
     "cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non "
     "proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";


static constexpr auto encoded = R"base64(
TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2ljaW5nIGVsaXQs
IHNlZCBkbyBlaXVzbW9kIHRlbXBvciBpbmNpZGlkdW50IHV0IGxhYm9yZSBldCBkb2xvcmUgbWFn
bmEgYWxpcXVhLiBVdCBlbmltIGFkIG1pbmltIHZlbmlhbSwgcXVpcyBub3N0cnVkIGV4ZXJjaXRh
dGlvbiB1bGxhbWNvIGxhYm9yaXMgbmlzaSB1dCBhbGlxdWlwIGV4IGVhIGNvbW1vZG8gY29uc2Vx
dWF0LiBEdWlzIGF1dGUgaXJ1cmUgZG9sb3IgaW4gcmVwcmVoZW5kZXJpdCBpbiB2b2x1cHRhdGUg
dmVsaXQgZXNzZSBjaWxsdW0gZG9sb3JlIGV1IGZ1Z2lhdCBudWxsYSBwYXJpYXR1ci4gRXhjZXB0
ZXVyIHNpbnQgb2NjYWVjYXQgY3VwaWRhdGF0IG5vbiBwcm9pZGVudCwgc3VudCBpbiBjdWxwYSBx
dWkgb2ZmaWNpYSBkZXNlcnVudCBtb2xsaXQgYW5pbSBpZCBlc3QgbGFib3J1bS4=
)base64";


} // namespace multiline
namespace formatted {


static constexpr auto decoded =
     "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod "
     "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, "
     "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo "
     "consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse "
     "cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non "
     "proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";


static constexpr auto encoded =
     "  TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQsIGNvbnNlY3RldHVyIGFkaXBpc2ljaW5nIGVsaXQs\r"
     "\tIHNlZCBkbyBlaXVzbW9kIHRlbXBvciBpbmNpZGlkdW50IHV0IGxhYm9yZSBldCBkb2xvcmUgbWFn\n"
     "  bmEgYWxpcXVhLiBVdCBlbmltIGFkIG1pbmltIHZlbmlhbSwgcXVpcyBub3N0cnVkIGV4ZXJjaXRh\r\n"
     "\tdGlvbiB1bGxhbWNvIGxhYm9yaXMgbmlzaSB1dCBhbGlxdWlwIGV4IGVhIGNvbW1vZG8gY29uc2Vx\n\r"
     "  dWF0LiBEdWlzIGF1dGUgaXJ1cmUgZG9sb3IgaW4gcmVwcmVoZW5kZXJpdCBpbiB2b2x1cHRhdGUg\r\n\r"
     "\tdmVsaXQgZXNzZSBjaWxsdW0gZG9sb3JlIGV1IGZ1Z2lhdCBudWxsYSBwYXJpYXR1ci4gRXhjZXB0\r\n\n"
     "\tZXVyIHNpbnQgb2NjYWVjYXQgY3VwaWRhdGF0IG5vbiBwcm9pZGVudCwgc3VudCBpbiBjdWxwYSBx\n\r\n"
     "  dWkgb2ZmaWNpYSBkZXNlcnVudCBtb2xsaXQgYW5pbSBpZCBlc3QgbGFib3J1bS4=\r\n";



} // namespace formatted
} // namespace test_env
} // namespace {unnamed}



BOOST_AUTO_TEST_SUITE( Base64 )

using namespace using_boost::iostreams;

BOOST_AUTO_TEST_SUITE( Encoding )
BOOST_DATA_TEST_CASE( Text
     , boost::unit_test::data::make( test_env::text::decoded )
          ^ boost::unit_test::data::make( test_env::text::encoded )
     , source
     , expected
)
{
     std::istringstream is{ source };
     boost::test_tools::output_test_stream os;
     base64::encode( is, os );
     BOOST_TEST( os.is_equal( expected ) );
}
BOOST_AUTO_TEST_SUITE( UsualAlphabet )
BOOST_AUTO_TEST_CASE( Binary )
{
     std::istringstream is{ std::ios::binary };
     is.rdbuf()->pubsetbuf(
          const_cast< char* >( reinterpret_cast< const char* >( test_env::bin::decoded ) )
          , sizeof( test_env::bin::decoded )
          );
     boost::test_tools::output_test_stream os;
     base64::encode( is, os );
     BOOST_TEST( os.is_equal( test_env::bin::encoded ) );
}
BOOST_AUTO_TEST_SUITE_END() /// UsualAlphabet
BOOST_AUTO_TEST_SUITE( UrlSafeAlphabet )
BOOST_AUTO_TEST_SUITE( WithPadding )
BOOST_AUTO_TEST_CASE( Binary )
{
     static constexpr auto withPadding = true;

     std::istringstream is{ std::ios::binary };
     is.rdbuf()->pubsetbuf(
          const_cast< char* >( reinterpret_cast< const char* >( test_env::bin::decoded ) )
          , sizeof( test_env::bin::decoded )
          );
     boost::test_tools::output_test_stream os;
     base64::url_safe::encode( is, os, withPadding );
     BOOST_TEST( os.is_equal( test_env::bin::url_safe::with_padding::encoded ) );
}
BOOST_AUTO_TEST_SUITE_END() /// WithPadding
BOOST_AUTO_TEST_SUITE( WithoutPadding )
BOOST_AUTO_TEST_CASE( Binary )
{
     std::istringstream is{ std::ios::binary };
     is.rdbuf()->pubsetbuf(
          const_cast< char* >( reinterpret_cast< const char* >( test_env::bin::decoded ) )
          , sizeof( test_env::bin::decoded )
          );
     boost::test_tools::output_test_stream os;
     base64::url_safe::encode( is, os );
     BOOST_TEST( os.is_equal( test_env::bin::url_safe::without_padding::encoded ) );
}
BOOST_AUTO_TEST_SUITE_END() /// WithoutPadding
BOOST_AUTO_TEST_SUITE_END() /// UrlSafeAlphabet
BOOST_AUTO_TEST_SUITE_END() // Encoding
BOOST_AUTO_TEST_SUITE( Decoding )
BOOST_DATA_TEST_CASE( Text
     , boost::unit_test::data::make( test_env::text::encoded )
          ^ boost::unit_test::data::make( test_env::text::decoded )
     , source
     , expected
)
{
     std::istringstream is{ source };
     boost::test_tools::output_test_stream os;
     base64::decode( is, os );
     BOOST_TEST( os.is_equal( expected ) );
}
BOOST_AUTO_TEST_SUITE( UsualAlphabet )
BOOST_AUTO_TEST_CASE( Binary )
{
     std::istringstream is{ test_env::bin::encoded };
     boost::test_tools::output_test_stream os;
     base64::decode( is, os );
     BOOST_TEST( os.is_equal(
          std::string{
               test_env::bin::decoded
               , test_env::bin::decoded + sizeof( test_env::bin::decoded )
               }
          ));
}
BOOST_AUTO_TEST_SUITE_END() /// UsualAlphabet
BOOST_AUTO_TEST_SUITE( UrlSafeAlphabet )
BOOST_AUTO_TEST_SUITE( WithPadding )
BOOST_AUTO_TEST_CASE( Binary )
{
     std::istringstream is{ test_env::bin::url_safe::with_padding::encoded };
     boost::test_tools::output_test_stream os;
     base64::url_safe::decode( is, os );
     BOOST_TEST( os.is_equal(
          std::string{
               test_env::bin::decoded
               , test_env::bin::decoded + sizeof( test_env::bin::decoded )
               }
          ));
}
BOOST_AUTO_TEST_SUITE_END() /// WithPadding
BOOST_AUTO_TEST_SUITE( WithoutPadding )
BOOST_AUTO_TEST_CASE( Binary )
{
     std::istringstream is{ test_env::bin::url_safe::without_padding::encoded };
     boost::test_tools::output_test_stream os;
     base64::url_safe::decode( is, os );
     BOOST_TEST( os.is_equal(
          std::string{
               test_env::bin::decoded
               , test_env::bin::decoded + sizeof( test_env::bin::decoded )
               }
          ));
}
BOOST_AUTO_TEST_SUITE_END() /// WithoutPadding
BOOST_AUTO_TEST_SUITE_END() /// UrlSafeAlphabet
BOOST_AUTO_TEST_CASE( MultilineText )
{
     std::istringstream is{ test_env::multiline::encoded };
     boost::test_tools::output_test_stream os;
     base64::decode( is, os, "\r\n" );
     BOOST_TEST( os.is_equal( test_env::multiline::decoded ) );
}
BOOST_AUTO_TEST_CASE( FormattedText )
{
     std::istringstream is{ test_env::formatted::encoded };
     boost::test_tools::output_test_stream os;
     base64::decode( is, os, "\r\n\t " );
     BOOST_TEST( os.is_equal( test_env::formatted::decoded ) );
}
BOOST_AUTO_TEST_SUITE_END() /// Decoding
BOOST_AUTO_TEST_SUITE_END() /// Base64
