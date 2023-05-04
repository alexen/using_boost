/// @file
/// @brief


#include <boost/test/unit_test.hpp>


BOOST_TEST_DECORATOR(
     * boost::unit_test::label( "suites" )
     * boost::unit_test::description( "Test suites usage examples" )
     )
BOOST_AUTO_TEST_SUITE( RootTestSuite )
     BOOST_AUTO_TEST_CASE( TestCase1 )
     {
          BOOST_TEST_WARN( 0 != 1 );
     }
     BOOST_AUTO_TEST_CASE( TestCase2 )
     {
          BOOST_TEST_REQUIRE( 4 == 4 );
//          BOOST_FAIL( "Unreachable code!" );
     }
     BOOST_AUTO_TEST_SUITE( ChildTestSuite )
          BOOST_AUTO_TEST_CASE( ChildTestCase1 )
          {
               BOOST_TEST( true );
          }
     BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
