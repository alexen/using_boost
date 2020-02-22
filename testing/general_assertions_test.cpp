/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(
     BoostTestUsage,
     * boost::unit_test::label( "asserts" )
     )
{
     auto a = 1;
     auto b = 2;

     BOOST_TEST( a != b );
     BOOST_TEST( a - 1 > b );
     BOOST_TEST( a == b, "expected that a == b, but a (" << a << ") != b (" << b << ")" );
     BOOST_TEST( a == b, boost::test_tools::bitwise() );
     BOOST_TEST( a + 0.1 == b - 0.8, boost::test_tools::tolerance( 0.01 ) );
}


BOOST_AUTO_TEST_CASE(
     BoostTestLimitations,
     * boost::unit_test::label( "asserts" )
     * boost::unit_test::description( "BOOST_TEST require brackets around logical and ternary ops" )
     )
{
     auto a = 10;
     auto b = 24;

     BOOST_TEST( ((9 < a && a <= 10) || (b != 24)) );
     BOOST_TEST( (a + b > 25 ? true : false) );
}


BOOST_AUTO_TEST_CASE(
     FloatingPointComparison,
     * boost::unit_test::label( "asserts" )
     * boost::unit_test::description( "Floating point comparison with tolerance 0.001" )
     * boost::unit_test::tolerance( 0.00001 )
     )
{
     auto a = 10.0000000;
     auto b = 10.0000001;
     auto c = 10.001;

     BOOST_TEST( a == b );
     BOOST_TEST( a == c );
}


BOOST_AUTO_TEST_CASE(
     StringsComparison,
     * boost::unit_test::label( "asserts" )
     * boost::unit_test::description( "Compare text as std::string and C-string" )
     )
{
     auto a = "hello";
     auto b = "hello";
     auto c = "aloha";

     BOOST_TEST( a == b );
     BOOST_TEST( a == c );
     BOOST_TEST( std::string{ a } == c );
     BOOST_TEST( a == c, boost::test_tools::per_element() );
     BOOST_TEST( a < b, boost::test_tools::lexicographic() );
}


BOOST_AUTO_TEST_CASE(
     CollectionComparison,
     * boost::unit_test::label( "asserts" )
     * boost::unit_test::description( "Working with C-arrays and STL collections (vector, set, map etc.)" )
     )
{
     const std::vector< int > a { 1,2,3 };
     const std::vector< long > b { 1,5,3 };
     const std::vector< int > c { 1,5,3,4 };

     /// lexicographical comparison by default for STL collections
     BOOST_TEST( a < c );     // a == b will not compile and a <= b too
     BOOST_TEST( a >= c );
     BOOST_TEST( a == b, boost::test_tools::per_element() );
     BOOST_TEST( a != b, boost::test_tools::per_element() );

     const int x[] = { 1,2,3 };
     const int y[] = { 1,5,3 };
     const int z[] = { 1,5,3,4 };

     /// per-element comparison by default for C-arrays
     BOOST_TEST( x == x );
     BOOST_TEST( x == y );
     BOOST_TEST( x != z );
     BOOST_TEST( y == z );

     const std::string text = "Привет";
     const std::vector< char > chars ( text.cbegin(), text.cend() );
     const std::vector< std::uint8_t > bytes {
          0xd0, 0x9f, 0xd1, 0x80, 0xd0, 0xb8, 0xd0, 0xb2, 0xd0, 0xb5, 0xd1, 0x82, 0x00
     };

     BOOST_TEST( chars == bytes, boost::test_tools::per_element() );
}
