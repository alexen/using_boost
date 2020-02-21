/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <boost/test/unit_test.hpp>


struct Fixture
{
     Fixture()
     {
          BOOST_TEST_MESSAGE( "fixture c-tor: " << __FUNCTION__ );
     }
     ~Fixture()
     {
          BOOST_TEST_MESSAGE( "fixture d-tor: " << __FUNCTION__ );
     }
     void operate() const
     {
          BOOST_TEST_MESSAGE( "operating..." );
     }
};


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
