/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <boost/test/unit_test.hpp>


BOOST_AUTO_TEST_CASE(
     TimeoutTest,
     * boost::unit_test::label( "timeout" )
     * boost::unit_test::timeout( 3 )
     )
{
#ifdef BOOST_SIGACTION_BASED_SIGNAL_HANDLING
     while( true );
     BOOST_TEST( true );
#else
     BOOST_TEST( false );
#endif
}
