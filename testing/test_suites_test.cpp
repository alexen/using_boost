/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE( RootTestSuite )
     BOOST_AUTO_TEST_CASE( TestCase1 )
     {
          BOOST_TEST_WARN( 0 != 1 );
     }
     BOOST_AUTO_TEST_CASE( TestCase2 )
     {
          BOOST_TEST_REQUIRE( 2 == 4 );
          BOOST_FAIL( "Unreachable code!" );
     }
     BOOST_AUTO_TEST_SUITE( ChildTestSuite )
          BOOST_AUTO_TEST_CASE( ChildTestCase1 )
          {
               BOOST_TEST( true );
          }
     BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
