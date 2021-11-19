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
} /// namespace test_env
} /// namespace {unnamed}


BOOST_AUTO_TEST_SUITE( IoFilters )
BOOST_AUTO_TEST_SUITE( Modifying )
BOOST_AUTO_TEST_SUITE( SingleChar )
BOOST_AUTO_TEST_SUITE( VowelRemover )

using using_boost::iostreams::filters::single_char::VowelRemover;

BOOST_AUTO_TEST_CASE( EmptyData )
{
     std::istringstream iss;

     boost::iostreams::filtering_istream is;
     is.push( VowelRemover{} );
     is.push( iss );

     boost::test_tools::output_test_stream os;
     boost::iostreams::copy( is, os );

     BOOST_TEST( os.is_empty() );
}
BOOST_AUTO_TEST_CASE( TextData )
{
     std::istringstream iss{ test_env::text::source };

     boost::iostreams::filtering_istream is;
     is.push( VowelRemover{} );
     is.push( iss );

     boost::test_tools::output_test_stream os;
     boost::iostreams::copy( is, os );

     BOOST_TEST( os.is_equal( test_env::text::modified ) );
}
BOOST_AUTO_TEST_SUITE_END() /// VowelRemover
BOOST_AUTO_TEST_SUITE_END() /// SingleChar
BOOST_AUTO_TEST_SUITE( Multichar )
BOOST_AUTO_TEST_SUITE( VowelRemover )

using using_boost::iostreams::filters::multichar::VowelRemover;

BOOST_AUTO_TEST_CASE( EmptyData )
{
     std::istringstream iss;

     boost::iostreams::filtering_istream is;
     is.push( VowelRemover{} );
     is.push( iss );

     boost::test_tools::output_test_stream os;
     boost::iostreams::copy( is, os );

     BOOST_TEST( os.is_empty() );
}
BOOST_AUTO_TEST_CASE( TextData )
{
     std::istringstream iss{ test_env::text::source };

     boost::iostreams::filtering_istream is;
     is.push( VowelRemover{} );
     is.push( iss );

     boost::test_tools::output_test_stream os;
     boost::iostreams::copy( is, os );

     BOOST_TEST( os.is_equal( test_env::text::modified ) );
}
BOOST_AUTO_TEST_SUITE_END() /// VowelRemover
BOOST_AUTO_TEST_SUITE_END() /// Multichar
BOOST_AUTO_TEST_SUITE_END() /// Modifying
BOOST_AUTO_TEST_SUITE_END() /// IoFilters
