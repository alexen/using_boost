/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <boost/test/unit_test.hpp>


struct PositiveInteger
{
     explicit PositiveInteger( int n ) : n_{ n }
     {
          if( n_ < 0 )
          {
               BOOST_THROW_EXCEPTION( std::runtime_error{ "positive value expected" } );
          }
     }

private:
     const int n_;
};


BOOST_AUTO_TEST_CASE( ThrowTest,
     * boost::unit_test::label( "throw" )
     )
{
     BOOST_REQUIRE_NO_THROW( PositiveInteger{ 12 } );
     BOOST_CHECK_THROW( PositiveInteger{ -7 }, std::runtime_error );
     BOOST_CHECK_THROW( PositiveInteger{ -3 }, std::exception );
}
