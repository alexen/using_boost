/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <boost/test/unit_test.hpp>


struct LocalFixture
{
     LocalFixture()
     {
          ++n_;
          BOOST_TEST_MESSAGE( __PRETTY_FUNCTION__ << ": set up (" << n_ << ")" );
     }
     ~LocalFixture()
     {
          BOOST_TEST_MESSAGE( __PRETTY_FUNCTION__ << ": tear down (" << n_ <<")" );
          --n_;
     }
     void setup()
     {
          BOOST_TEST_MESSAGE( __PRETTY_FUNCTION__ );
     }
     void teardown()
     {
          BOOST_TEST_MESSAGE( __PRETTY_FUNCTION__ );
     }
     int getN() const
     {
          BOOST_TEST_MESSAGE( __PRETTY_FUNCTION__ << ": getting n (" << n_ << ")" );
          return n_;
     }
private:
     static int n_;
};

int LocalFixture::n_ = 0;


struct GlobalFixture
{
     GlobalFixture()
     {
          ++n_;
          BOOST_TEST_MESSAGE( __PRETTY_FUNCTION__ << ": set up (" << n_ << ")" );
     }
     ~GlobalFixture()
     {
          --n_;
          BOOST_TEST_MESSAGE( __PRETTY_FUNCTION__ << ": tear down (" << n_ <<")" );
     }
     void setup()
     {
          BOOST_TEST_MESSAGE( __PRETTY_FUNCTION__ );
     }
     void teardown()
     {
          BOOST_TEST_MESSAGE( __PRETTY_FUNCTION__ );
     }
     int getN() const
     {
          BOOST_TEST_MESSAGE( __PRETTY_FUNCTION__ << ": getting n (" << n_ << ")" );
          return n_;
     }
private:
     static int n_;
};

int GlobalFixture::n_ = 0;


BOOST_TEST_DECORATOR(
     * boost::unit_test::label( "fixture" )
     * boost::unit_test::description( "Test fixtures (local and global) usage examples" )
     )
BOOST_TEST_GLOBAL_FIXTURE( GlobalFixture );
BOOST_AUTO_TEST_SUITE( FixtureTestExamples )
     BOOST_AUTO_TEST_SUITE( LocalFixtureTest )
          BOOST_FIXTURE_TEST_CASE( TestCaseA, LocalFixture )
          {
               BOOST_TEST_MESSAGE( "n is " << getN() );
               BOOST_TEST( true );
          }
          BOOST_FIXTURE_TEST_CASE( TestCaseB, LocalFixture )
          {
               BOOST_TEST_MESSAGE( "n is " << getN() );
               BOOST_TEST( true );
          }
     BOOST_AUTO_TEST_SUITE_END()

     BOOST_AUTO_TEST_SUITE( GlobalFixtureTest )
          BOOST_FIXTURE_TEST_CASE( TestCaseA, GlobalFixture )
          {
               BOOST_TEST_MESSAGE( "n is " << getN() );
               BOOST_TEST( true );
          }
          BOOST_FIXTURE_TEST_CASE( TestCaseB, GlobalFixture )
          {
               BOOST_TEST_MESSAGE( "n is " << getN() );
               BOOST_TEST( true );
          }
          BOOST_FIXTURE_TEST_CASE( TestCaseC, GlobalFixture )
          {
               BOOST_TEST_MESSAGE( "n is " << getN() );
               BOOST_TEST( true );
          }
          BOOST_FIXTURE_TEST_CASE( TestCaseD, GlobalFixture )
          {
               BOOST_TEST_MESSAGE( "n is " << getN() );
               BOOST_TEST( true );
          }
     BOOST_AUTO_TEST_SUITE_END()
BOOST_AUTO_TEST_SUITE_END()
