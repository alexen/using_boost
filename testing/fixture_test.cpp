/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <boost/test/unit_test.hpp>


struct Fixture
{
     Fixture()
     {
          BOOST_TEST_MESSAGE( "fixture c-tor: " << __PRETTY_FUNCTION__ );
     }
     ~Fixture()
     {
          BOOST_TEST_MESSAGE( "fixture d-tor: " << __PRETTY_FUNCTION__ );
     }
     void operate() const
     {
          BOOST_TEST_MESSAGE( "operating..." );
     }
};


struct Fixture2
{
     void setup()
     {
          BOOST_TEST_MESSAGE( "setup test fixture " << __PRETTY_FUNCTION__ );
     }
     void teardown()
     {
          BOOST_TEST_MESSAGE( "teardown fixture " << __PRETTY_FUNCTION__ );
     }

     void doSmth()
     {
          BOOST_TEST_MESSAGE( "doing smth..." );
     }
};


BOOST_TEST_DECORATOR(
     * boost::unit_test::label( "fixture" )
     * boost::unit_test::description( "Test fixtures usage examples" )
     )
BOOST_AUTO_TEST_SUITE( FixtureTestExamples )
     BOOST_AUTO_TEST_SUITE( FixtureTest )
          BOOST_FIXTURE_TEST_CASE( TestCaseA, Fixture )
          {
               operate();
               BOOST_TEST( true );
          }
          BOOST_FIXTURE_TEST_CASE( TestCaseB, Fixture )
          {
               operate();
               BOOST_TEST( true );
          }
     BOOST_AUTO_TEST_SUITE_END()

     BOOST_AUTO_TEST_SUITE( Fixture2Test )
          BOOST_FIXTURE_TEST_CASE( TestCase001, Fixture2 )
          {
               doSmth();
               BOOST_TEST( true );
          }
     BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
