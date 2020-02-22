/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <boost/test/unit_test.hpp>

#include <testing/func.h>


BOOST_AUTO_TEST_CASE(
     TestConcat
     ,
     * boost::unit_test::label( "first" )
     * boost::unit_test::description( "My first test case" )
     )
{
     static constexpr auto lhs = "left hand side";
     static constexpr auto rhs = " right hand side";

     static const std::string expected = "left hand side right hand side";

     BOOST_TEST( expected == concat( lhs, rhs ) );
}
