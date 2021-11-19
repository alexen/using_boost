/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <sstream>

#include <boost/test/unit_test.hpp>
#include <boost/test/tools/output_test_stream.hpp>
#include <boost/mpl/list.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filtering_stream.hpp>

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


constexpr auto vowels = { 'a','e','i','o','u','A','E','I','O','U' };


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
BOOST_AUTO_TEST_SUITE_END() /// NonModifying
BOOST_AUTO_TEST_SUITE( Modifying )
BOOST_AUTO_TEST_SUITE( SingleChar )
BOOST_AUTO_TEST_SUITE( CharRemover )

using using_boost::iostreams::filters::single_char::CharRemover;

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
BOOST_AUTO_TEST_SUITE_END() /// CharRemover
BOOST_AUTO_TEST_SUITE_END() /// SingleChar
BOOST_AUTO_TEST_SUITE( Multichar )
BOOST_AUTO_TEST_SUITE( CharRemover )

using using_boost::iostreams::filters::multichar::CharRemover;

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
BOOST_AUTO_TEST_SUITE_END() /// CharRemover
BOOST_AUTO_TEST_SUITE_END() /// Multichar
BOOST_AUTO_TEST_SUITE_END() /// Modifying
BOOST_AUTO_TEST_SUITE_END() /// IoFilters
