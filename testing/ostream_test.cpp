/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <boost/test/unit_test.hpp>
#include <boost/test/output_test_stream.hpp>


BOOST_AUTO_TEST_CASE(
     OutputStreamSimpleTest,
     * boost::unit_test::label( "ostream" )
     )
{
     boost::test_tools::output_test_stream os;

     int val = 12;
     os << "val == " << val;

     const bool cleanStream = true;

     BOOST_TEST( !os.is_empty( !cleanStream ) );
     BOOST_TEST( os.check_length( 9, !cleanStream ) );
     BOOST_TEST( os.is_equal( "val == 12", !cleanStream ) );
}
