/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <sstream>

#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/algorithm/hex.hpp>
#include <boost/range/algorithm/equal.hpp>

#include <iostreams/filters.h>


namespace {
namespace test_env {
namespace text {


static constexpr auto source = R"txt(
Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod
tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam,
quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo
consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse
cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non
proident, sunt in culpa qui officia deserunt mollit anim id est laborum.
)txt";


static constexpr auto modified = R"txt(
Lrm psm dlr st mt, cnscttr dpscng lt, sd d smd
tmpr ncddnt t lbr t dlr mgn lq. t nm d mnm vnm,
qs nstrd xrcttn llmc lbrs ns t lqp x  cmmd
cnsqt. Ds t rr dlr n rprhndrt n vlptt vlt ss
cllm dlr  fgt nll prtr. xcptr snt ccct cpdtt nn
prdnt, snt n clp q ffc dsrnt mllt nm d st lbrm.
)txt";


} /// namespace text


static constexpr auto vowels = "aeiouAEIOU";


namespace aux {


template< typename Iter >
struct AsHex
{
     explicit AsHex( Iter beg, Iter end ) : beg{ beg }, end{ end } {}
     Iter beg, end;
};


template< typename Iter >
std::ostream& operator<<( std::ostream& os, const AsHex< Iter >& ah )
{
     boost::algorithm::hex( ah.beg, ah.end, std::ostreambuf_iterator< char >{ os } );
     return os;
}


} // namespace aux
} /// namespace test_env
} /// namespace {unnamed}


BOOST_AUTO_TEST_SUITE( IoFilters )
BOOST_AUTO_TEST_SUITE( NonModifying )
BOOST_AUTO_TEST_SUITE( SingleChar )
BOOST_AUTO_TEST_SUITE( Transparent )

using using_boost::iostreams::filters::single_char::Transparent;

BOOST_AUTO_TEST_SUITE( Input )
BOOST_AUTO_TEST_CASE( EmptyData )
{
     std::istringstream iss;

     boost::iostreams::filtering_istream is;
     is.push( Transparent{} );
     is.push( iss );

     boost::test_tools::output_test_stream os;
     boost::iostreams::copy( is, os );

     BOOST_TEST( os.is_empty() );
}
BOOST_AUTO_TEST_CASE( TextData )
{
     std::istringstream iss{ test_env::text::source };

     boost::iostreams::filtering_istream is;
     is.push( Transparent{} );
     is.push( iss );

     boost::test_tools::output_test_stream os;
     boost::iostreams::copy( is, os );

     BOOST_TEST( os.is_equal( test_env::text::source ) );
}
BOOST_AUTO_TEST_SUITE_END() /// Input
BOOST_AUTO_TEST_SUITE( Output )
BOOST_AUTO_TEST_CASE( EmptyData )
{
     std::istringstream is;
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_ostream fos;
     fos.push( Transparent{} );
     fos.push( os );

     boost::iostreams::copy( is, fos );

     BOOST_TEST( os.is_empty() );
}
BOOST_AUTO_TEST_CASE( TextData )
{
     std::istringstream is{ test_env::text::source };
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_ostream fos;
     fos.push( Transparent{} );
     fos.push( os );

     boost::iostreams::copy( is, fos );

     BOOST_TEST( os.is_equal( test_env::text::source ) );
}
BOOST_AUTO_TEST_SUITE_END() /// Output
BOOST_AUTO_TEST_SUITE_END() /// Transparent
BOOST_AUTO_TEST_SUITE_END() /// SingleChar
BOOST_AUTO_TEST_SUITE( Multichar )
BOOST_AUTO_TEST_SUITE( Transparent )

using using_boost::iostreams::filters::multichar::Transparent;

BOOST_AUTO_TEST_SUITE( Input )
BOOST_AUTO_TEST_CASE( EmptyData )
{
     std::istringstream iss;

     boost::iostreams::filtering_istream is;
     is.push( Transparent{} );
     is.push( iss );

     boost::test_tools::output_test_stream os;
     boost::iostreams::copy( is, os );

     BOOST_TEST( os.is_empty() );
}
BOOST_AUTO_TEST_CASE( TextData )
{
     std::istringstream iss{ test_env::text::source };

     boost::iostreams::filtering_istream is;
     is.push( Transparent{} );
     is.push( iss );

     boost::test_tools::output_test_stream os;
     boost::iostreams::copy( is, os );

     BOOST_TEST( os.is_equal( test_env::text::source ) );
}
BOOST_AUTO_TEST_SUITE_END() /// Input
BOOST_AUTO_TEST_SUITE( Output )
BOOST_AUTO_TEST_CASE( EmptyData )
{
     std::istringstream iss;
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_ostream fos;
     fos.push( Transparent{} );
     fos.push( os );

     boost::iostreams::copy( iss, fos );

     BOOST_TEST( os.is_empty() );
}
BOOST_AUTO_TEST_CASE( TextData )
{
     std::istringstream iss{ test_env::text::source };
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_ostream fos;
     fos.push( Transparent{} );
     fos.push( os );

     boost::iostreams::copy( iss, fos );

     BOOST_TEST( os.is_equal( test_env::text::source ) );
}
BOOST_AUTO_TEST_SUITE_END() /// Output
BOOST_AUTO_TEST_SUITE_END() /// Transparent
BOOST_AUTO_TEST_SUITE( Counter )

using using_boost::iostreams::filters::multichar::Counter;

BOOST_AUTO_TEST_SUITE( Input )
BOOST_AUTO_TEST_CASE( EmptyStream )
{
     Counter c;

     std::istringstream iss;
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( c ) );
     fis.push( iss );

     boost::test_tools::output_test_stream os;
     boost::iostreams::copy( fis, os );

     BOOST_TEST( os.is_empty() );
     BOOST_TEST( c.chars() == 0u );
}
BOOST_AUTO_TEST_CASE( TextStream )
{
     Counter c;

     std::istringstream iss{ test_env::text::source };
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( c ) );
     fis.push( iss );

     boost::test_tools::output_test_stream os;
     boost::iostreams::copy( fis, os );

     BOOST_TEST( !os.is_empty() );
     BOOST_TEST( c.chars() == std::strlen( test_env::text::source ) );
}
BOOST_AUTO_TEST_SUITE_END() /// Input
BOOST_AUTO_TEST_SUITE( Output )
BOOST_AUTO_TEST_CASE( EmptyStream )
{
     Counter c;

     std::istringstream iss;
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( c ) );
     fos.push( os );

     boost::iostreams::copy( iss, fos );

     BOOST_TEST( os.is_empty() );
     BOOST_TEST( c.chars() == 0u );
}
BOOST_AUTO_TEST_CASE( TextStream )
{
     Counter c;

     std::istringstream iss{ test_env::text::source };
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( c ) );
     fos.push( os );

     boost::iostreams::copy( iss, fos );

     BOOST_TEST( !os.is_empty() );
     BOOST_TEST( c.chars() == std::strlen( test_env::text::source ) );
}
BOOST_AUTO_TEST_SUITE_END() /// Output
BOOST_AUTO_TEST_SUITE_END() /// Counter
BOOST_AUTO_TEST_SUITE_END() /// Multichar
BOOST_AUTO_TEST_SUITE( Symmetric )
BOOST_AUTO_TEST_SUITE( Monitor )

using using_boost::iostreams::filters::symmetric::Monitor;

BOOST_AUTO_TEST_CASE( EmptyStream )
{
     std::istringstream is;
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_istream fis;
     fis.push( Monitor{} );
     fis.push( is );

     boost::iostreams::copy( fis, os );

     BOOST_TEST( os.is_empty() );
}
BOOST_AUTO_TEST_CASE( TextStream )
{
     std::istringstream is{ test_env::text::source };
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_istream fis;
     fis.push( Monitor{} );
     fis.push( is );

     boost::iostreams::copy( fis, os );

     BOOST_TEST( os.is_equal( test_env::text::source ) );
}
BOOST_AUTO_TEST_CASE( TextStreamWithMonitoring )
{
     std::istringstream is{ test_env::text::source };
     boost::test_tools::output_test_stream dest;
     boost::test_tools::output_test_stream mon;

     Monitor monit{ mon };

     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( monit ) );
     fis.push( is );

     boost::iostreams::copy( fis, dest );

     BOOST_TEST( dest.is_equal( test_env::text::source ) );
     BOOST_TEST( !mon.is_empty() );
}
BOOST_AUTO_TEST_SUITE_END() /// Monitor
BOOST_AUTO_TEST_SUITE( Counter )

using using_boost::iostreams::filters::symmetric::Counter;

BOOST_AUTO_TEST_SUITE( Input )
BOOST_AUTO_TEST_CASE( EmptyStream )
{
     std::istringstream is;
     boost::test_tools::output_test_stream os;

     Counter c;
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( c ) );
     fis.push( is );

     boost::iostreams::copy( fis, os );

     BOOST_TEST( os.is_empty() );
     BOOST_TEST( c.filter().bytes() == 0u );
}
BOOST_AUTO_TEST_CASE( TextStream )
{
     std::istringstream is{ test_env::text::source };
     boost::test_tools::output_test_stream os;

     Counter c;
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( c ) );
     fis.push( is );

     boost::iostreams::copy( fis, os );

     BOOST_TEST( os.is_equal( test_env::text::source ) );
     BOOST_TEST( c.filter().bytes() == std::strlen( test_env::text::source )  );
}
BOOST_AUTO_TEST_CASE( BinaryStream )
{
     static constexpr std::uint8_t source[] = {
          0xd5, 0xc9, 0x90, 0x8b, 0xcd, 0x43, 0x00, 0x1d, 0x80, 0xa6, 0x68, 0x9a,
          0x9e, 0xbb, 0x9d, 0x2a, 0x91, 0xc6, 0x43, 0x52, 0x32, 0xc4, 0x65, 0x05,
          0x90, 0x84, 0xc7, 0xa1, 0x7b, 0x64, 0x2e, 0xee, 0x0e, 0xbe, 0x42, 0x69,
          0xef, 0x75, 0xda, 0x14, 0x22, 0xd9, 0xfd, 0x87, 0xcd, 0x3f, 0x5d, 0x62,
          0xa8, 0x78, 0x3b, 0xc4, 0x9b, 0x01, 0x3a, 0x58, 0x04, 0x3b, 0x1a, 0x9d,
          0x03, 0x8f, 0x3f, 0x38, 0x12, 0x39, 0x65, 0x29, 0x32, 0xa9, 0xb1, 0x27,
          0x62, 0x4c, 0x8c, 0x35, 0x1b, 0x81, 0x50, 0x88, 0x58, 0xee, 0x9a, 0x58,
          0xeb, 0x0b, 0x52, 0x01, 0xe1, 0x26, 0x59, 0xfc, 0xab, 0x4b, 0x2f, 0x9b,
          0xf6, 0x3c, 0x16, 0xd9, 0x51, 0x4a, 0xc3, 0x59, 0x7e, 0x2d, 0xc4, 0x87,
          0x57, 0x90, 0x23, 0x49, 0x1b, 0x54, 0x63, 0x0d, 0xe6, 0x90, 0x8d, 0x49,
          0x7e, 0xc6, 0x9a, 0x99, 0x76, 0x5f, 0x0c, 0x9d
     };
     boost::iostreams::stream< boost::iostreams::array_source > is{
          boost::iostreams::array_source{ reinterpret_cast< const char* >( source ), sizeof( source ) }
     };
     boost::test_tools::output_test_stream os;

     Counter c;
     boost::iostreams::filtering_istream fis;
     fis.push( boost::ref( c ) );
     fis.push( is );

     boost::iostreams::copy( fis, os );

     BOOST_TEST( os.is_equal( std::string{ source, source + sizeof( source ) } ) );
     BOOST_TEST( c.filter().bytes() == sizeof( source )  );
}
BOOST_AUTO_TEST_SUITE_END() /// Input
BOOST_AUTO_TEST_SUITE( Output )
BOOST_AUTO_TEST_CASE( EmptyStream )
{
     std::istringstream is;
     boost::test_tools::output_test_stream os;

     Counter c;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( c ) );
     fos.push( os );

     boost::iostreams::copy( is, fos );

     BOOST_TEST( os.is_empty() );
     BOOST_TEST( c.filter().bytes() == 0u );
}
BOOST_AUTO_TEST_CASE( TextStream )
{
     std::istringstream is{ test_env::text::source };
     boost::test_tools::output_test_stream os;

     Counter c;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( c ) );
     fos.push( os );

     boost::iostreams::copy( is, fos );

     BOOST_TEST( os.is_equal( test_env::text::source ) );
     BOOST_TEST( c.filter().bytes() == std::strlen( test_env::text::source ) );
}
BOOST_AUTO_TEST_CASE( BinaryStream )
{
     static constexpr std::uint8_t source[] = {
          0xd9, 0x04, 0x49, 0x9f, 0x41, 0x08, 0x77, 0xdb, 0x0b, 0x52, 0xff, 0x00,
          0x64, 0xde, 0x5d, 0x71, 0x79, 0xe7, 0xf5, 0x2e, 0xad, 0xb2, 0xe4, 0xf1,
          0xe8, 0x54, 0x22, 0x45, 0xe9, 0xa1, 0x10, 0xda, 0x7f, 0x8e, 0xd6, 0x04,
          0xfa, 0x14, 0x53, 0x3c, 0x63, 0x6f, 0x96, 0x82, 0x2c, 0xa8, 0xcb, 0x17,
          0x98, 0x59, 0xd7, 0x3a, 0x8e, 0x6a, 0x47, 0x0f, 0xef, 0x75, 0x63, 0xe9,
          0x1a, 0x27, 0x15, 0xd9, 0xa0, 0xd4, 0xde, 0x94, 0xba, 0xe5, 0xe1, 0x38,
          0x32, 0xdb, 0x32, 0xce, 0x9d, 0x44, 0x34, 0xac, 0x06, 0xf9, 0xaa, 0xe1,
          0x2a, 0x02, 0x08, 0x55, 0xd0, 0x84, 0x47, 0xbf, 0x02, 0xd1, 0x86, 0x86,
          0xc9, 0xd8, 0x64, 0x53, 0x03, 0x6b, 0xc6, 0xe7, 0x1b, 0x04, 0x2e, 0xac,
          0xbb, 0xb8, 0x4f, 0x70, 0xb0, 0x82, 0x6f, 0xb7, 0x14, 0xc9, 0x11, 0x38,
          0x82, 0x59, 0x0d, 0xdb, 0xce, 0x66, 0x0d, 0xb8, 0x77, 0xbc, 0x0d, 0x59,
          0x89, 0xcf, 0xc0, 0x7b, 0x72, 0xdf, 0xc8, 0x4c, 0xf1, 0x34, 0xf8, 0x7c,
          0x6d, 0x9e, 0x00, 0x2f, 0x9d, 0x21, 0xa7, 0xfb, 0xaf, 0x2a, 0xc4, 0xe9,
          0xac, 0x20, 0x51, 0x1e, 0x71, 0xa0, 0x40, 0x5d, 0x10, 0x88, 0x92, 0x60,
          0x4e, 0x33, 0x2b, 0xc4, 0xea, 0xc2, 0x83, 0xf1, 0x8e, 0x96, 0x97, 0x76,
          0x9f, 0x97, 0x24, 0xe2, 0x75, 0x76, 0x64, 0x1f, 0x23, 0x35, 0x14, 0x63,
          0xc2, 0xa5, 0xc5, 0xc7, 0xdc, 0xfc, 0xe5, 0xe9, 0x92, 0x62, 0x9c, 0x4e,
          0x44, 0x9a, 0x25, 0x5b, 0xd6, 0x1e, 0xb7, 0x53, 0x4d, 0xe4, 0xe5, 0x83,
          0x24, 0xe5, 0xf1, 0x75, 0x4e, 0x55, 0x48, 0xcd, 0x0c, 0x01, 0x3d, 0x45,
          0x22, 0x47, 0x0c, 0x50, 0x4e, 0x65, 0xb7, 0x96, 0xb2, 0x26, 0x26, 0x40,
          0xbd, 0x21, 0xfc, 0xa8, 0x4e, 0xb2, 0x72, 0xba, 0xcc, 0xe1, 0xb2, 0x51,
          0xd3, 0xbf, 0x81, 0xa2, 0xfa, 0x6a, 0x7d, 0xcf, 0xe8, 0x05, 0xe4, 0xce,
          0x4f, 0x13, 0xda, 0xae, 0x61, 0xbc, 0x3a, 0xf7, 0x64, 0xbb, 0x91, 0x6e,
          0x64, 0x6d, 0x11, 0x6b, 0x80, 0x65, 0x2d, 0x69, 0xf9, 0xc2, 0x9f, 0x24,
          0x07, 0x16, 0x4f, 0x4d, 0x87, 0x81, 0xe5, 0x69, 0x8b, 0xa4, 0x2e, 0x25,
          0x03, 0x38, 0x30, 0xc4, 0xed, 0x81, 0xd0, 0x6d, 0xd9, 0x62, 0x83, 0x43
     };
     boost::iostreams::stream< boost::iostreams::array_source > is{
          boost::iostreams::array_source{ reinterpret_cast< const char* >( source ), sizeof( source ) }
     };
     boost::test_tools::output_test_stream os;

     Counter c;
     boost::iostreams::filtering_ostream fos;
     fos.push( boost::ref( c ) );
     fos.push( os );

     boost::iostreams::copy( is, fos );

     BOOST_TEST( os.is_equal( std::string{ source, source + sizeof( source )} ) );
     BOOST_TEST( c.filter().bytes() == sizeof( source ) );
}
BOOST_AUTO_TEST_SUITE_END() /// Output
BOOST_AUTO_TEST_SUITE_END() /// Counter
BOOST_AUTO_TEST_SUITE_END() /// Symmetric
BOOST_AUTO_TEST_SUITE_END() /// NonModifying
BOOST_AUTO_TEST_SUITE( Modifying )
BOOST_AUTO_TEST_SUITE( SingleChar )
BOOST_AUTO_TEST_SUITE( CharRemover )

using using_boost::iostreams::filters::single_char::CharRemover;

BOOST_AUTO_TEST_SUITE( Input )
BOOST_AUTO_TEST_CASE( EmptyData )
{
     std::istringstream iss;

     boost::iostreams::filtering_istream is;
     is.push( CharRemover{ test_env::vowels } );
     is.push( iss );

     boost::test_tools::output_test_stream os;
     boost::iostreams::copy( is, os );

     BOOST_TEST( os.is_empty() );
}
BOOST_AUTO_TEST_CASE( TextData )
{
     std::istringstream iss{ test_env::text::source };

     boost::iostreams::filtering_istream is;
     is.push( CharRemover{ test_env::vowels } );
     is.push( iss );

     boost::test_tools::output_test_stream os;
     boost::iostreams::copy( is, os );

     BOOST_TEST( os.is_equal( test_env::text::modified ) );
}
BOOST_AUTO_TEST_SUITE_END() /// Input
BOOST_AUTO_TEST_SUITE( Output )
BOOST_AUTO_TEST_CASE( EmptyData )
{
     std::istringstream iss;
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_ostream fos;
     fos.push( CharRemover{ test_env::vowels } );
     fos.push( os );

     boost::iostreams::copy( iss, fos );

     BOOST_TEST( os.is_empty() );
}
BOOST_AUTO_TEST_CASE( TextData )
{
     std::istringstream iss{ test_env::text::source };
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_ostream fos;
     fos.push( CharRemover{ test_env::vowels } );
     fos.push( os );

     boost::iostreams::copy( iss, fos );

     BOOST_TEST( os.is_equal( test_env::text::modified ) );
}
BOOST_AUTO_TEST_SUITE_END() /// Output
BOOST_AUTO_TEST_SUITE_END() /// CharRemover
BOOST_AUTO_TEST_SUITE_END() /// SingleChar
BOOST_AUTO_TEST_SUITE( Multichar )
BOOST_AUTO_TEST_SUITE( CharRemover )

using using_boost::iostreams::filters::multichar::CharRemover;

BOOST_AUTO_TEST_SUITE( Input )
BOOST_AUTO_TEST_CASE( EmptyData )
{
     std::istringstream iss;

     boost::iostreams::filtering_istream is;
     is.push( CharRemover{ test_env::vowels } );
     is.push( iss );

     boost::test_tools::output_test_stream os;
     boost::iostreams::copy( is, os );

     BOOST_TEST( os.is_empty() );
}
BOOST_AUTO_TEST_CASE( TextData )
{
     std::istringstream iss{ test_env::text::source };

     boost::iostreams::filtering_istream is;
     is.push( CharRemover{ test_env::vowels } );
     is.push( iss );

     boost::test_tools::output_test_stream os;
     boost::iostreams::copy( is, os );

     BOOST_TEST( os.is_equal( test_env::text::modified ) );
}
BOOST_AUTO_TEST_SUITE_END() /// Input
BOOST_AUTO_TEST_SUITE( Output )
BOOST_AUTO_TEST_CASE( EmptyData )
{
     std::istringstream iss;
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_ostream fos;
     fos.push( CharRemover{ test_env::vowels } );
     fos.push( os );

     boost::iostreams::copy( iss, fos );

     BOOST_TEST( os.is_empty() );
}
BOOST_AUTO_TEST_CASE( TextData )
{
     std::istringstream iss{ test_env::text::source };
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_ostream fos;
     fos.push( CharRemover{ test_env::vowels } );
     fos.push( os );

     boost::iostreams::copy( iss, fos );

     BOOST_TEST( os.is_equal( test_env::text::modified ) );
}
BOOST_AUTO_TEST_SUITE_END() /// Output
BOOST_AUTO_TEST_SUITE_END() /// CharRemover
BOOST_AUTO_TEST_SUITE( StreamInterrupter )

using using_boost::iostreams::filters::multichar::StreamInterrupter;

BOOST_AUTO_TEST_SUITE( Input )
BOOST_AUTO_TEST_CASE( EmptyStream )
{
     std::istringstream is;
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_istream fis;
     fis.push( StreamInterrupter{ "xYz" } );
     fis.push( is );

     boost::iostreams::copy( fis, os );

     BOOST_TEST( os.is_empty() );
}
BOOST_AUTO_TEST_CASE( TextStream )
{
     static constexpr auto source =
          "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod "
          "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, "
          "quis nostrud exercitation ullamco ^ laboris nisi ut aliquip ex ea commodo "
          "consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse "
          "cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non "
          "proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

     static constexpr auto interrupters = "^";

     static constexpr auto expected =
          "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod "
          "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, "
          "quis nostrud exercitation ullamco ";

     std::istringstream is{ source };
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_istream fis;
     fis.push( StreamInterrupter{ interrupters } );
     fis.push( is );

     boost::iostreams::copy( fis, os );

     BOOST_TEST( os.is_equal( expected ) );
}
BOOST_AUTO_TEST_CASE( BinaryStream )
{
     static constexpr std::uint8_t source[] = {
          0x63, 0x41, 0x67, 0xdb, 0x7f, 0x57, 0x23, 0x2a, 0xb9, 0xf1, 0xec, 0xba,
          0xea, 0x31, 0x00, 0xd3, 0xec, 0xd3, 0xb6, 0x85, 0x1d, 0xc4, 0x31, 0xdd,
          0xa3, 0x32, 0x27, 0x36, 0x09, 0xba, 0xc7, 0x1e, 0xf3, 0xea, 0x8a, 0x34,
          0x6b, 0x79, 0x88, 0xb0, 0x79, 0x92, 0xb8, 0x23, 0x1c, 0xac, 0xc0, 0xb6,
          0x41, 0xba, 0xab, 0x3b, 0x68, 0xe3, 0x78, 0x68, 0x75, 0x01, 0x98, 0x74,
          0xaf, 0x2b, 0xc4, 0xd3, 0x98, 0xd2, 0xca, 0x1b, 0x1a, 0xc8, 0x97, 0x92,
          0x1b, 0xe8, 0x98, 0x1b, 0x3d, 0x22, 0xa9, 0xfc, 0xda, 0x39, 0xe3, 0xf3,
          0xde, 0xe6, 0xf8, 0x6f, 0xc8, 0xa7, 0xf0, 0x93, 0xcb, 0x83, 0x5a, 0xde,
          0x44, 0x64, 0x40, 0x32, 0x86, 0x33, 0x9c, 0xd8, 0xbf, 0xc2, 0x90, 0x7f,
          0xc3, 0xf4, 0x10, 0x8f, 0x71, 0x31, 0x8f, 0x63, 0x0f, 0x8c, 0xe2, 0x72,
          0x04, 0x37, 0x39, 0x5e, 0x17, 0x02, 0x80, 0xf4
     };
     static constexpr std::uint8_t interrupters[] = { 0x68, 0xd2, 0x6f };
     static constexpr std::uint8_t expected[] = {
          0x63, 0x41, 0x67, 0xdb, 0x7f, 0x57, 0x23, 0x2a, 0xb9, 0xf1, 0xec, 0xba,
          0xea, 0x31, 0x00, 0xd3, 0xec, 0xd3, 0xb6, 0x85, 0x1d, 0xc4, 0x31, 0xdd,
          0xa3, 0x32, 0x27, 0x36, 0x09, 0xba, 0xc7, 0x1e, 0xf3, 0xea, 0x8a, 0x34,
          0x6b, 0x79, 0x88, 0xb0, 0x79, 0x92, 0xb8, 0x23, 0x1c, 0xac, 0xc0, 0xb6,
          0x41, 0xba, 0xab, 0x3b
     };

     boost::iostreams::stream< boost::iostreams::array_source > is{
          boost::iostreams::array_source{
               reinterpret_cast< const char* >( source )
               , sizeof( source )
          }
     };
     std::ostringstream oss{ std::ios::binary };

     boost::iostreams::filtering_istream fis;
     fis.push( StreamInterrupter{ interrupters, interrupters + sizeof( interrupters ) } );
     fis.push( is );

     boost::iostreams::copy( fis, oss );

     const auto& actual = oss.str();

     BOOST_TEST( actual == std::string( expected, expected + sizeof( expected ) ),
          "binary sequences is not equal"
          << "\n- expected: " << test_env::aux::AsHex( actual.cbegin(), actual.cend() )
          << "\n-   actual: " << test_env::aux::AsHex( expected, expected + sizeof( expected ) )
          );
}
BOOST_AUTO_TEST_SUITE_END() /// Input
BOOST_AUTO_TEST_SUITE( Output )
BOOST_AUTO_TEST_CASE( EmptyStream )
{
     std::istringstream is;
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_ostream fos;
     fos.push( StreamInterrupter{ "xYz" } );
     fos.push( os );

     boost::iostreams::copy( is, fos );

     BOOST_TEST( os.is_empty() );
}
BOOST_AUTO_TEST_CASE( TextStream )
{
     static constexpr auto source =
          "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod "
          "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, "
          "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo "
          "consequat. Duis aute irure # dolor in reprehenderit in voluptate velit esse "
          "cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non "
          "proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

     static constexpr auto interrupters = "%$#";

     static constexpr auto expected =
          "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod "
          "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, "
          "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo "
          "consequat. Duis aute irure ";

     std::istringstream is{ source };
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_ostream fos;
     fos.push( StreamInterrupter{ interrupters } );
     fos.push( os );

     boost::iostreams::copy( is, fos );

     BOOST_TEST( os.is_equal( expected ) );
}
BOOST_AUTO_TEST_CASE( BinaryStream )
{
     static constexpr std::uint8_t source[] = {
          0x8b, 0x8f, 0xd7, 0x3a, 0xb0, 0x29, 0x41, 0x64, 0x44, 0x92, 0xdf, 0x0c,
          0x39, 0x6a, 0x09, 0x6b, 0x67, 0xb5, 0x71, 0x25, 0x15, 0x68, 0x92, 0xa2,
          0x09, 0x9a, 0x44, 0x3e, 0x3c, 0x99, 0xb6, 0x44, 0x41, 0xa4, 0x26, 0x40,
          0x0b, 0x2b, 0xf3, 0xc8, 0x8c, 0xba, 0xd9, 0x61, 0x03, 0x6c, 0xb7, 0x25,
          0x1f, 0x2e, 0xda, 0xcd, 0x59, 0x97, 0xe6, 0x25, 0x92, 0x53, 0x1a, 0xf4,
          0xdc, 0xfb, 0x40, 0x98, 0xcd, 0xc6, 0x96, 0x21, 0x2c, 0xdc, 0x3c, 0x8d,
          0xd1, 0x9e, 0x25, 0x2f, 0x34, 0x0e, 0xc8, 0x8c, 0x22, 0xb9, 0x17, 0x49,
          0x0b, 0x18, 0x91, 0x9c, 0x19, 0x17, 0x7c, 0x90, 0x46, 0x06, 0x34, 0x53,
          0x97, 0x97, 0xce, 0x55, 0xc6, 0xef, 0x0e, 0x20, 0xb6, 0x8b, 0x12, 0x1b,
          0x05, 0x72, 0x1c, 0x91, 0xb3, 0x32, 0x79, 0xa1, 0x19, 0x05, 0xb3, 0x2c,
          0x7e, 0xfa, 0xd3, 0x75, 0x83, 0x37, 0x85, 0xf6
     };
     static constexpr std::uint8_t interrupters[] = { 0xb6 };
     static constexpr std::uint8_t expected[] = {
          0x8b, 0x8f, 0xd7, 0x3a, 0xb0, 0x29, 0x41, 0x64, 0x44, 0x92, 0xdf, 0x0c,
          0x39, 0x6a, 0x09, 0x6b, 0x67, 0xb5, 0x71, 0x25, 0x15, 0x68, 0x92, 0xa2,
          0x09, 0x9a, 0x44, 0x3e, 0x3c, 0x99
     };

     boost::iostreams::stream< boost::iostreams::array_source > is{
          boost::iostreams::array_source{
               reinterpret_cast< const char* >( source )
               , sizeof( source )
          }
     };
     std::ostringstream oss{ std::ios::binary };

     boost::iostreams::filtering_ostream fos;
     fos.push( StreamInterrupter{ interrupters, interrupters + sizeof( interrupters ) } );
     fos.push( oss );

     boost::iostreams::copy( is, fos );

     const auto& actual = oss.str();

     BOOST_TEST( actual == std::string( expected, expected + sizeof( expected ) ),
          "binary sequences is not equal"
          << "\n- expected: " << test_env::aux::AsHex( actual.cbegin(), actual.cend() )
          << "\n-   actual: " << test_env::aux::AsHex( expected, expected + sizeof( expected ) )
          );
}
BOOST_AUTO_TEST_SUITE_END() /// Output
BOOST_AUTO_TEST_SUITE_END() /// StreamInterrupter
BOOST_AUTO_TEST_SUITE( CharMultiplier )

using using_boost::iostreams::filters::multichar::CharMultiplier;

BOOST_AUTO_TEST_SUITE( Input )
BOOST_AUTO_TEST_CASE( EmptyStream )
{
     std::istringstream is;
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_istream fis;
     fis.push( CharMultiplier{ 50 } );
     fis.push( is );

     boost::iostreams::copy( fis, os );

     BOOST_TEST( os.is_empty() );
}
BOOST_AUTO_TEST_CASE( ShortTextStream )
{
     static constexpr auto source = "0123456789abcdefg";
     static constexpr auto expected = "000111222333444555666777888999aaabbbcccdddeeefffggg";

     std::istringstream is{ source };
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_istream fis;
     fis.push( CharMultiplier{ 3 } );
     fis.push( is );

     boost::iostreams::copy( fis, os );

     BOOST_TEST( os.is_equal( expected ) );
}
BOOST_AUTO_TEST_CASE( LongTextStream )
{
     static constexpr auto source =
          "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod "
          "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, "
          "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo "
          "consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse "
          "cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non "
          "proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

     static constexpr auto expected =
          "LLLLLooooorrrrreeeeemmmmm     iiiiipppppsssssuuuuummmmm     dddddooooolllllo"
          "oooorrrrr     sssssiiiiittttt     aaaaammmmmeeeeettttt,,,,,     cccccooooonn"
          "nnnssssseeeeecccccttttteeeeetttttuuuuurrrrr     aaaaadddddiiiiipppppiiiiisss"
          "ssiiiiiccccciiiiinnnnnggggg     eeeeellllliiiiittttt,,,,,     ssssseeeeedddd"
          "d     dddddooooo     eeeeeiiiiiuuuuusssssmmmmmoooooddddd     ttttteeeeemmmmm"
          "pppppooooorrrrr     iiiiinnnnnccccciiiiidddddiiiiiddddduuuuunnnnnttttt     u"
          "uuuuttttt     lllllaaaaabbbbbooooorrrrreeeee     eeeeettttt     dddddoooooll"
          "lllooooorrrrreeeee     mmmmmaaaaagggggnnnnnaaaaa     aaaaallllliiiiiqqqqquuu"
          "uuaaaaa.....     UUUUUttttt     eeeeennnnniiiiimmmmm     aaaaaddddd     mmmm"
          "miiiiinnnnniiiiimmmmm     vvvvveeeeennnnniiiiiaaaaammmmm,,,,,     qqqqquuuuu"
          "iiiiisssss     nnnnnooooossssstttttrrrrruuuuuddddd     eeeeexxxxxeeeeerrrrrc"
          "cccciiiiitttttaaaaatttttiiiiiooooonnnnn     uuuuullllllllllaaaaammmmmcccccoo"
          "ooo     lllllaaaaabbbbbooooorrrrriiiiisssss     nnnnniiiiisssssiiiii     uuu"
          "uuttttt     aaaaallllliiiiiqqqqquuuuuiiiiippppp     eeeeexxxxx     eeeeeaaaa"
          "a     cccccooooommmmmmmmmmooooodddddooooo     cccccooooonnnnnssssseeeeeqqqqq"
          "uuuuuaaaaattttt.....     DDDDDuuuuuiiiiisssss     aaaaauuuuuttttteeeee     i"
          "iiiirrrrruuuuurrrrreeeee     dddddooooolllllooooorrrrr     iiiiinnnnn     rr"
          "rrreeeeeppppprrrrreeeeehhhhheeeeennnnndddddeeeeerrrrriiiiittttt     iiiiinnn"
          "nn     vvvvvooooollllluuuuuppppptttttaaaaattttteeeee     vvvvveeeeellllliiii"
          "ittttt     eeeeesssssssssseeeee     ccccciiiiilllllllllluuuuummmmm     ddddd"
          "ooooolllllooooorrrrreeeee     eeeeeuuuuu     fffffuuuuugggggiiiiiaaaaattttt "
          "    nnnnnuuuuullllllllllaaaaa     pppppaaaaarrrrriiiiiaaaaatttttuuuuurrrrr.."
          "...     EEEEExxxxxccccceeeeepppppttttteeeeeuuuuurrrrr     sssssiiiiinnnnnttt"
          "tt     oooooccccccccccaaaaaeeeeecccccaaaaattttt     cccccuuuuupppppiiiiidddd"
          "daaaaatttttaaaaattttt     nnnnnooooonnnnn     ppppprrrrroooooiiiiidddddeeeee"
          "nnnnnttttt,,,,,     sssssuuuuunnnnnttttt     iiiiinnnnn     cccccuuuuulllllp"
          "ppppaaaaa     qqqqquuuuuiiiii     oooooffffffffffiiiiiccccciiiiiaaaaa     dd"
          "dddeeeeessssseeeeerrrrruuuuunnnnnttttt     mmmmmooooolllllllllliiiiittttt   "
          "  aaaaannnnniiiiimmmmm     iiiiiddddd     eeeeesssssttttt     lllllaaaaabbbb"
          "booooorrrrruuuuummmmm.....";

     std::istringstream is{ source };
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_istream fis;
     fis.push( CharMultiplier{ 5 } );
     fis.push( is );

     boost::iostreams::copy( fis, os );

     BOOST_TEST( os.is_equal( expected ) );
}
BOOST_AUTO_TEST_CASE( BinaryStream )
{
     static constexpr std::uint8_t source[] = {
          0xdb, 0x54, 0x74, 0x7b, 0xf2, 0x03, 0xbd, 0x69, 0x7e, 0xff, 0xd4, 0x75,
          0x9c, 0x16, 0x5a, 0xf1, 0xaa, 0x95, 0x5e, 0x0b, 0x59, 0xa9, 0x71, 0x18,
          0x0a, 0x22, 0x57, 0x75, 0xc6, 0xbf, 0xdc, 0xa2, 0x1f, 0xba, 0xf5, 0x46,
          0x50, 0xa3, 0x79, 0x8c, 0x0a, 0x41, 0x9c, 0x47, 0x95, 0x10, 0xbd, 0x15,
          0xa8, 0x49, 0x26, 0xf2, 0xcc, 0x98, 0x83, 0xb6, 0xfc, 0xf0, 0xd2, 0x0d,
          0xc4, 0x99, 0xad, 0xa9
     };
     static constexpr std::uint8_t expected[] = {
          0xdb, 0xdb, 0xdb, 0xdb, 0x54, 0x54, 0x54, 0x54, 0x74, 0x74, 0x74, 0x74,
          0x7b, 0x7b, 0x7b, 0x7b, 0xf2, 0xf2, 0xf2, 0xf2, 0x03, 0x03, 0x03, 0x03,
          0xbd, 0xbd, 0xbd, 0xbd, 0x69, 0x69, 0x69, 0x69, 0x7e, 0x7e, 0x7e, 0x7e,
          0xff, 0xff, 0xff, 0xff, 0xd4, 0xd4, 0xd4, 0xd4, 0x75, 0x75, 0x75, 0x75,
          0x9c, 0x9c, 0x9c, 0x9c, 0x16, 0x16, 0x16, 0x16, 0x5a, 0x5a, 0x5a, 0x5a,
          0xf1, 0xf1, 0xf1, 0xf1, 0xaa, 0xaa, 0xaa, 0xaa, 0x95, 0x95, 0x95, 0x95,
          0x5e, 0x5e, 0x5e, 0x5e, 0x0b, 0x0b, 0x0b, 0x0b, 0x59, 0x59, 0x59, 0x59,
          0xa9, 0xa9, 0xa9, 0xa9, 0x71, 0x71, 0x71, 0x71, 0x18, 0x18, 0x18, 0x18,
          0x0a, 0x0a, 0x0a, 0x0a, 0x22, 0x22, 0x22, 0x22, 0x57, 0x57, 0x57, 0x57,
          0x75, 0x75, 0x75, 0x75, 0xc6, 0xc6, 0xc6, 0xc6, 0xbf, 0xbf, 0xbf, 0xbf,
          0xdc, 0xdc, 0xdc, 0xdc, 0xa2, 0xa2, 0xa2, 0xa2, 0x1f, 0x1f, 0x1f, 0x1f,
          0xba, 0xba, 0xba, 0xba, 0xf5, 0xf5, 0xf5, 0xf5, 0x46, 0x46, 0x46, 0x46,
          0x50, 0x50, 0x50, 0x50, 0xa3, 0xa3, 0xa3, 0xa3, 0x79, 0x79, 0x79, 0x79,
          0x8c, 0x8c, 0x8c, 0x8c, 0x0a, 0x0a, 0x0a, 0x0a, 0x41, 0x41, 0x41, 0x41,
          0x9c, 0x9c, 0x9c, 0x9c, 0x47, 0x47, 0x47, 0x47, 0x95, 0x95, 0x95, 0x95,
          0x10, 0x10, 0x10, 0x10, 0xbd, 0xbd, 0xbd, 0xbd, 0x15, 0x15, 0x15, 0x15,
          0xa8, 0xa8, 0xa8, 0xa8, 0x49, 0x49, 0x49, 0x49, 0x26, 0x26, 0x26, 0x26,
          0xf2, 0xf2, 0xf2, 0xf2, 0xcc, 0xcc, 0xcc, 0xcc, 0x98, 0x98, 0x98, 0x98,
          0x83, 0x83, 0x83, 0x83, 0xb6, 0xb6, 0xb6, 0xb6, 0xfc, 0xfc, 0xfc, 0xfc,
          0xf0, 0xf0, 0xf0, 0xf0, 0xd2, 0xd2, 0xd2, 0xd2, 0x0d, 0x0d, 0x0d, 0x0d,
          0xc4, 0xc4, 0xc4, 0xc4, 0x99, 0x99, 0x99, 0x99, 0xad, 0xad, 0xad, 0xad,
          0xa9, 0xa9, 0xa9, 0xa9
     };

     boost::iostreams::stream< boost::iostreams::array_source > is{
          boost::iostreams::array_source{
               reinterpret_cast< const char* >( source )
               , sizeof( source )
          }
     };
     std::ostringstream oss{ std::ios::binary };

     boost::iostreams::filtering_istream fis;
     fis.push( CharMultiplier{ 4 } );
     fis.push( is );

     boost::iostreams::copy( fis, oss );

     const auto& actual = oss.str();

     BOOST_TEST( actual == std::string( expected, expected + sizeof( expected ) ),
          "binary sequences is not equal"
          << "\n- expected: " << test_env::aux::AsHex( actual.cbegin(), actual.cend() )
          << "\n-   actual: " << test_env::aux::AsHex( expected, expected + sizeof( expected ) )
          );
}
BOOST_AUTO_TEST_SUITE_END() /// Input
BOOST_AUTO_TEST_SUITE( Output )
BOOST_AUTO_TEST_CASE( EmptyStream )
{
     std::istringstream is;
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_ostream fos;
     fos.push( CharMultiplier{ 37 } );
     fos.push( os );

     boost::iostreams::copy( is, fos );

     BOOST_TEST( os.is_empty() );
}
BOOST_AUTO_TEST_CASE( TextStream )
{
     static constexpr auto source =
          "Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do eiusmod "
          "tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, "
          "quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo "
          "consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse "
          "cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non "
          "proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";

     static constexpr auto expected =
          "LLLLLooooorrrrreeeeemmmmm     iiiiipppppsssssuuuuummmmm     dddddooooolllllo"
          "oooorrrrr     sssssiiiiittttt     aaaaammmmmeeeeettttt,,,,,     cccccooooonn"
          "nnnssssseeeeecccccttttteeeeetttttuuuuurrrrr     aaaaadddddiiiiipppppiiiiisss"
          "ssiiiiiccccciiiiinnnnnggggg     eeeeellllliiiiittttt,,,,,     ssssseeeeedddd"
          "d     dddddooooo     eeeeeiiiiiuuuuusssssmmmmmoooooddddd     ttttteeeeemmmmm"
          "pppppooooorrrrr     iiiiinnnnnccccciiiiidddddiiiiiddddduuuuunnnnnttttt     u"
          "uuuuttttt     lllllaaaaabbbbbooooorrrrreeeee     eeeeettttt     dddddoooooll"
          "lllooooorrrrreeeee     mmmmmaaaaagggggnnnnnaaaaa     aaaaallllliiiiiqqqqquuu"
          "uuaaaaa.....     UUUUUttttt     eeeeennnnniiiiimmmmm     aaaaaddddd     mmmm"
          "miiiiinnnnniiiiimmmmm     vvvvveeeeennnnniiiiiaaaaammmmm,,,,,     qqqqquuuuu"
          "iiiiisssss     nnnnnooooossssstttttrrrrruuuuuddddd     eeeeexxxxxeeeeerrrrrc"
          "cccciiiiitttttaaaaatttttiiiiiooooonnnnn     uuuuullllllllllaaaaammmmmcccccoo"
          "ooo     lllllaaaaabbbbbooooorrrrriiiiisssss     nnnnniiiiisssssiiiii     uuu"
          "uuttttt     aaaaallllliiiiiqqqqquuuuuiiiiippppp     eeeeexxxxx     eeeeeaaaa"
          "a     cccccooooommmmmmmmmmooooodddddooooo     cccccooooonnnnnssssseeeeeqqqqq"
          "uuuuuaaaaattttt.....     DDDDDuuuuuiiiiisssss     aaaaauuuuuttttteeeee     i"
          "iiiirrrrruuuuurrrrreeeee     dddddooooolllllooooorrrrr     iiiiinnnnn     rr"
          "rrreeeeeppppprrrrreeeeehhhhheeeeennnnndddddeeeeerrrrriiiiittttt     iiiiinnn"
          "nn     vvvvvooooollllluuuuuppppptttttaaaaattttteeeee     vvvvveeeeellllliiii"
          "ittttt     eeeeesssssssssseeeee     ccccciiiiilllllllllluuuuummmmm     ddddd"
          "ooooolllllooooorrrrreeeee     eeeeeuuuuu     fffffuuuuugggggiiiiiaaaaattttt "
          "    nnnnnuuuuullllllllllaaaaa     pppppaaaaarrrrriiiiiaaaaatttttuuuuurrrrr.."
          "...     EEEEExxxxxccccceeeeepppppttttteeeeeuuuuurrrrr     sssssiiiiinnnnnttt"
          "tt     oooooccccccccccaaaaaeeeeecccccaaaaattttt     cccccuuuuupppppiiiiidddd"
          "daaaaatttttaaaaattttt     nnnnnooooonnnnn     ppppprrrrroooooiiiiidddddeeeee"
          "nnnnnttttt,,,,,     sssssuuuuunnnnnttttt     iiiiinnnnn     cccccuuuuulllllp"
          "ppppaaaaa     qqqqquuuuuiiiii     oooooffffffffffiiiiiccccciiiiiaaaaa     dd"
          "dddeeeeessssseeeeerrrrruuuuunnnnnttttt     mmmmmooooolllllllllliiiiittttt   "
          "  aaaaannnnniiiiimmmmm     iiiiiddddd     eeeeesssssttttt     lllllaaaaabbbb"
          "booooorrrrruuuuummmmm.....";

     std::istringstream is{ source };
     boost::test_tools::output_test_stream os;

     boost::iostreams::filtering_ostream fos;
     fos.push( CharMultiplier{ 5 } );
     fos.push( os );

     boost::iostreams::copy( is, fos );

     BOOST_TEST( os.is_equal( expected ) );
}
BOOST_AUTO_TEST_CASE( BinaryStream )
{
     static constexpr std::uint8_t source[] = {
          0x93, 0xbf, 0xef, 0xfb, 0xe9, 0xbb, 0x32, 0x73, 0x93, 0x63, 0x41, 0xb7,
          0xb9, 0x7f, 0x9c, 0x60, 0xd8, 0xd6, 0x5e, 0x85, 0xc0, 0x1c, 0x2b, 0xab,
          0xfc, 0x43, 0xe5, 0x30, 0x76, 0xbf, 0xa2, 0x7d, 0xcd, 0x43, 0x98, 0x2f,
          0x52, 0xee, 0x14, 0xa4, 0xff, 0xac, 0x09, 0xf0, 0x40, 0x79, 0x91, 0x27,
          0x13, 0xa6, 0x92, 0x7e, 0x54, 0x99, 0x32, 0x90, 0xaf, 0xb5, 0x3a, 0x79,
          0x5c, 0x50, 0x78, 0x86
     };
     static constexpr std::uint8_t expected[] = {
          0x93, 0x93, 0x93, 0xbf, 0xbf, 0xbf, 0xef, 0xef, 0xef, 0xfb, 0xfb, 0xfb,
          0xe9, 0xe9, 0xe9, 0xbb, 0xbb, 0xbb, 0x32, 0x32, 0x32, 0x73, 0x73, 0x73,
          0x93, 0x93, 0x93, 0x63, 0x63, 0x63, 0x41, 0x41, 0x41, 0xb7, 0xb7, 0xb7,
          0xb9, 0xb9, 0xb9, 0x7f, 0x7f, 0x7f, 0x9c, 0x9c, 0x9c, 0x60, 0x60, 0x60,
          0xd8, 0xd8, 0xd8, 0xd6, 0xd6, 0xd6, 0x5e, 0x5e, 0x5e, 0x85, 0x85, 0x85,
          0xc0, 0xc0, 0xc0, 0x1c, 0x1c, 0x1c, 0x2b, 0x2b, 0x2b, 0xab, 0xab, 0xab,
          0xfc, 0xfc, 0xfc, 0x43, 0x43, 0x43, 0xe5, 0xe5, 0xe5, 0x30, 0x30, 0x30,
          0x76, 0x76, 0x76, 0xbf, 0xbf, 0xbf, 0xa2, 0xa2, 0xa2, 0x7d, 0x7d, 0x7d,
          0xcd, 0xcd, 0xcd, 0x43, 0x43, 0x43, 0x98, 0x98, 0x98, 0x2f, 0x2f, 0x2f,
          0x52, 0x52, 0x52, 0xee, 0xee, 0xee, 0x14, 0x14, 0x14, 0xa4, 0xa4, 0xa4,
          0xff, 0xff, 0xff, 0xac, 0xac, 0xac, 0x09, 0x09, 0x09, 0xf0, 0xf0, 0xf0,
          0x40, 0x40, 0x40, 0x79, 0x79, 0x79, 0x91, 0x91, 0x91, 0x27, 0x27, 0x27,
          0x13, 0x13, 0x13, 0xa6, 0xa6, 0xa6, 0x92, 0x92, 0x92, 0x7e, 0x7e, 0x7e,
          0x54, 0x54, 0x54, 0x99, 0x99, 0x99, 0x32, 0x32, 0x32, 0x90, 0x90, 0x90,
          0xaf, 0xaf, 0xaf, 0xb5, 0xb5, 0xb5, 0x3a, 0x3a, 0x3a, 0x79, 0x79, 0x79,
          0x5c, 0x5c, 0x5c, 0x50, 0x50, 0x50, 0x78, 0x78, 0x78, 0x86, 0x86, 0x86
     };

     boost::iostreams::stream< boost::iostreams::array_source > is{
          boost::iostreams::array_source{
               reinterpret_cast< const char* >( source )
               , sizeof( source )
          }
     };
     std::ostringstream oss{ std::ios::binary };

     boost::iostreams::filtering_ostream fos;
     fos.push( CharMultiplier{ 3 } );
     fos.push( oss );

     boost::iostreams::copy( is, fos );

     const auto& actual = oss.str();

     BOOST_TEST( actual == std::string( expected, expected + sizeof( expected ) ),
          "binary sequences is not equal"
          << "\n- expected: " << test_env::aux::AsHex( actual.cbegin(), actual.cend() )
          << "\n-   actual: " << test_env::aux::AsHex( expected, expected + sizeof( expected ) )
          );
}
BOOST_AUTO_TEST_SUITE_END() /// Output
BOOST_AUTO_TEST_SUITE_END() /// CharMultiplier
BOOST_AUTO_TEST_SUITE_END() /// Multichar
BOOST_AUTO_TEST_SUITE_END() /// Modifying
BOOST_AUTO_TEST_SUITE_END() /// IoFilters
