/// @file
/// @brief
/// @copyright Copyright (c) InfoTeCS. All Rights Reserved.

#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>


BOOST_AUTO_TEST_CASE( Generic )
{
     BOOST_TEST( true );
}


struct InfiniteSequence
{
     enum { arity = 1 };

     struct iterator
     {
          iterator() {}

          int operator*() const { return current_; }
          void operator++() { ++current_; }
     private:
          unsigned current_ = 0u;
     };

     InfiniteSequence() {}

     boost::unit_test::data::size_t size() const
     {
          return boost::unit_test::data::BOOST_TEST_DS_INFINITE_SIZE;
     }

     iterator begin() const
     {
          return iterator{};
     }
};


namespace boost {
namespace unit_test {
namespace data {
namespace monomorphic {

template<> struct is_dataset< InfiniteSequence > : mpl::true_ {};

} // namespace monomorphic
} // namespace data
} // namespace unit_test
} // namespace boost


BOOST_DATA_TEST_CASE(
     TestInfiniteSequence
     , InfiniteSequence() ^ boost::unit_test::data::make({ 0,1,2,3,4,5,6,7,8,9,10,11,12 })
     , generatedValue
     , expectedValue
     )
{
     BOOST_TEST( expectedValue == generatedValue );
}


static constexpr int INTEGERS[] = {
     0,1,2,3,4,5,6,7,8,9
};


BOOST_DATA_TEST_CASE(
     TestInfiniteSequenceUsingArray
     , InfiniteSequence() ^ boost::unit_test::data::make( INTEGERS )
     , generatedValue
     , expectedValue
     )
{
     BOOST_TEST( expectedValue == generatedValue );
}


static constexpr int XRANGE_5 = 5;


BOOST_DATA_TEST_CASE(
     TestUsingXrange
     , boost::unit_test::data::xrange( XRANGE_5 )
     , value
     )
{
     BOOST_TEST( ((0 <= value) && (value < XRANGE_5)) );
}


static constexpr int XRANGE_3 = 3;
static const char* const CSTRS_3[] = { "zero", "one", "two" };


BOOST_DATA_TEST_CASE(
     TestUsingZip
     , boost::unit_test::data::xrange( XRANGE_3 ) ^ boost::unit_test::data::make( CSTRS_3 )
     , index
     , text
     )
{
     BOOST_TEST( (0 <= index && index < XRANGE_3 ) );
     BOOST_TEST( std::string{ CSTRS_3[ index ] } == text );
}


BOOST_DATA_TEST_CASE(
     TestUsingJoin
     , boost::unit_test::data::xrange( XRANGE_3 ) + boost::unit_test::data::make({ 66, 55, 44, 99, 12 })
     , value
     )
{
     BOOST_TEST((
          value == 0
          || value == 1
          || value == 2
          || value == 66
          || value == 55
          || value == 44
          || value == 99
          || value == 12
          ));
}


static const char* CSTRS_4[] = { "cake", "owl", "radio", "top" };


BOOST_DATA_TEST_CASE(
     TestUsingCartesianProduct
     , boost::unit_test::data::xrange( 3 ) * boost::unit_test::data::make( CSTRS_4 )
     , first
     , second
     )
{
     static constexpr auto isPresent =
          []( const std::string& str )
          {
               static constexpr auto end = CSTRS_4 + 4;
               return end != std::find( CSTRS_4, end, str );
          };

     switch( first )
     {
          case 0:
               BOOST_TEST( isPresent( second ) );
               break;
          case 1:
               BOOST_TEST( isPresent( second ) );
               break;
          case 2:
               BOOST_TEST( isPresent( second ) );
               break;
          default:
               BOOST_TEST( false );
     }
}
