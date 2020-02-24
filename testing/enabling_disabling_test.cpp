/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(
     EnablingDisablingTest1,
     * boost::unit_test::label( "enabling" )
     * boost::unit_test::enable_if< true >()
     )
{
     BOOST_TEST( 2 == 2 );
}


BOOST_AUTO_TEST_CASE(
     EnablingDisablingTest2,
     * boost::unit_test::label( "enabling" )
     * boost::unit_test::enable_if< false >()
     )
{
     BOOST_TEST( 2 == 1 );
}
